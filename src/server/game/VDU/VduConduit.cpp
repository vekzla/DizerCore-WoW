/*
 * VDU conduit implementation. See VduConduit.h.
 */

#include "VduConduit.h"

#include <atomic>
#include <chrono>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <mutex>

namespace Vdu
{
    namespace
    {
        std::mutex g_writeMutex;
        std::atomic<std::uint64_t> g_nextChain{1};

        char const* StatusName(Status status)
        {
            switch (status)
            {
                case Status::Ok:   return "OK";
                case Status::Warn: return "WARN";
                case Status::Fail: return "FAIL";
            }
            return "OK";
        }

        char const* DirectionName(Direction direction)
        {
            switch (direction)
            {
                case Direction::In:     return "IN";
                case Direction::Out:    return "OUT";
                case Direction::Server: return "SERVER";
            }
            return "SERVER";
        }
    }

    std::string JsonEscape(std::string const& text)
    {
        std::string out;
        out.reserve(text.size());
        for (char c : text)
        {
            switch (c)
            {
                case '"':  out += "\\\""; break;
                case '\\': out += "\\\\"; break;
                case '\n': out += "\\n";  break;
                case '\r': out += "\\r";  break;
                case '\t': out += "\\t";  break;
                default:
                    if (static_cast<unsigned char>(c) < 0x20)
                    {
                        char buf[7];
                        std::snprintf(buf, sizeof(buf), "\\u%04x", static_cast<unsigned>(c));
                        out += buf;
                    }
                    else
                        out += c;
            }
        }
        return out;
    }

    std::string ToJson(Event const& event)
    {
        std::string json;
        json.reserve(256);
        json += "{\"timestamp\":" + std::to_string(event.timestamp);
        json += ",\"chain\":" + std::to_string(event.chain);
        json += ",\"session\":\"" + JsonEscape(event.session) + "\"";
        json += ",\"direction\":\"" + std::string(DirectionName(event.direction)) + "\"";
        json += ",\"opcode\":\"" + JsonEscape(event.opcode) + "\"";
        json += ",\"module\":\"" + JsonEscape(event.module) + "\"";
        json += ",\"action\":\"" + JsonEscape(event.action) + "\"";
        json += ",\"status\":\"" + std::string(StatusName(event.status)) + "\"";
        json += ",\"reason\":\"" + JsonEscape(event.reason) + "\"";
        json += ",\"source\":\"" + JsonEscape(event.source) + "\"";
        json += ",\"player_guid\":" + std::to_string(event.playerGuid);
        json += ",\"payload_hex\":\"" + JsonEscape(event.payloadHex) + "\"}";
        return json;
    }

    bool IsListening(std::string const& baseDir)
    {
        std::error_code ec;
        return std::filesystem::exists(std::filesystem::path(baseDir) / "listen.on", ec);
    }

    bool AppendLine(std::string const& baseDir, char const* fileName, Event const& event)
    {
        std::lock_guard<std::mutex> lock(g_writeMutex);

        std::error_code ec;
        std::filesystem::create_directories(baseDir, ec);

        std::ofstream out(std::filesystem::path(baseDir) / fileName, std::ios::app);
        if (!out)
            return false;

        out << ToJson(event) << '\n';
        return static_cast<bool>(out);
    }

    // Quest, spell, pet and character events are kept whether or not listening is on, so the
    // viewer can rebuild their state on the next start. Everything else is only kept while listening.
    bool AlwaysRecordedOpcode(std::string const& opcode, std::string const& module)
    {
        static char const* const kModules[] = { "character", "quest", "spell", "pet" };
        static char const* const kOpcodePrefixes[] = {
            "CMSG_QUEST_", "SMSG_QUEST_", "CMSG_QUERY_QUEST", "SMSG_QUERY_QUEST",
            "CMSG_CAST_SPELL", "CMSG_CANCEL_CAST", "SMSG_SPELL_", "SMSG_AURA_UPDATE",
            "CMSG_PET_", "SMSG_PET_", "SMSG_QUERY_PET",
            "SMSG_QUERY_PLAYER_NAMES_RESPONSE", "SMSG_ENUM_CHARACTERS_RESULT",
        };
        for (char const* name : kModules)
            if (module == name)
                return true;
        for (char const* prefix : kOpcodePrefixes)
            if (opcode.rfind(prefix, 0) == 0)
                return true;
        return false;
    }

    bool WantedWithoutListening(std::string const& opcodeName)
    {
        return AlwaysRecordedOpcode(opcodeName, "packet");
    }

    bool AlwaysRecorded(Event const& event)
    {
        return AlwaysRecordedOpcode(event.opcode, event.module);
    }

    bool Record(std::string const& baseDir, Event const& event)
    {
        if (!IsListening(baseDir) && !AlwaysRecorded(event))
            return false;
        return AppendLine(baseDir, "events.jsonl", event);
    }

    std::uint64_t NewChain()
    {
        return g_nextChain.fetch_add(1, std::memory_order_relaxed);
    }

    // Per-opcode preview sizes. Only add a line when the viewer needs more of a packet
    // than the default. Every byte kept is written to events.jsonl, so keep the list short.
    //
    // Sizing notes, so future changes have a buffer:
    //   - The largest character list seen so far is about 3.5 KB for 4 characters, so
    //     each entry is sized well above what one packet needs: 4096 bytes.
    //   - If a packet is ever cut short, the viewer shows "..." on the end of the hex.
    //     Raise the number here, not in the viewer.
    //   - Keep the limit per opcode, never the default for all packets.
    std::size_t PreviewBytesFor(std::string const& opcodeName)
    {
        struct Entry
        {
            char const* opcode;
            std::size_t bytes;
        };
        static constexpr Entry kFullPreview[] = {
            { "SMSG_ENUM_CHARACTERS_RESULT", 4096 },        // character list: names, class, race
            { "SMSG_QUERY_PLAYER_NAMES_RESPONSE", 4096 },   // player names, class, race, level
            // Spell packets: the hit and miss lists after the cast header are what the viewer
            // decodes for damage and buffs, so they need room beyond the default.
            { "SMSG_SPELL_GO", 4096 },        // hit and miss lists: damage per target
            { "SMSG_AURA_UPDATE", 4096 },     // every aura in the packet, on any unit
            // Quest packets: the quest ID sits near the start, but the details and reward
            // packets carry strings and item lists that run past the default size.
            { "SMSG_QUEST_GIVER_QUEST_DETAILS", 2048 },
            { "SMSG_QUEST_GIVER_OFFER_REWARD_MESSAGE", 2048 },
            { "SMSG_QUEST_GIVER_QUEST_COMPLETE", 1024 },
            { "SMSG_QUEST_UPDATE_ADD_CREDIT", 512 },
            { "SMSG_QUEST_UPDATE_ADD_CREDIT_SIMPLE", 512 },
            { "SMSG_QUEST_UPDATE_COMPLETE", 256 },
            { "SMSG_QUEST_UPDATE_FAILED", 256 },
            { "CMSG_QUEST_GIVER_ACCEPT_QUEST", 256 },
            { "CMSG_QUEST_GIVER_COMPLETE_QUEST", 256 },
            { "CMSG_QUEST_GIVER_CHOOSE_REWARD", 256 },
            { "CMSG_QUEST_LOG_REMOVE_QUEST", 256 },
        };
        for (Entry const& entry : kFullPreview)
            if (opcodeName == entry.opcode)
                return entry.bytes;
        return kDefaultPreviewBytes;
    }

    std::string HexPreview(std::uint8_t const* data, std::size_t size, std::size_t maxBytes)
    {
        static constexpr char digits[] = "0123456789abcdef";
        std::string out;
        std::size_t shown = size < maxBytes ? size : maxBytes;
        out.reserve(shown * 3 + 3);
        for (std::size_t i = 0; i < shown; ++i)
        {
            if (i != 0)
                out += ' ';
            out += digits[data[i] >> 4];
            out += digits[data[i] & 0x0F];
        }
        if (size > shown)
            out += " ...";
        return out;
    }

    bool RecordPacket(std::string const& baseDir, Direction direction,
                      std::string const& opcodeName, std::uint64_t playerGuid,
                      std::size_t size, std::string const& payloadHex)
    {
        Event event;
        event.timestamp = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
        event.chain = 0;
        event.direction = direction;
        event.opcode = opcodeName;
        event.module = "packet";
        event.action = std::to_string(size) + " bytes";
        event.status = Status::Ok;
        event.playerGuid = playerGuid;
        event.payloadHex = payloadHex;
        // Name packets also go to players.jsonl, which is written whether or not listening.
        if (opcodeName == "SMSG_QUERY_PLAYER_NAMES_RESPONSE" || opcodeName == "SMSG_ENUM_CHARACTERS_RESULT")
            AppendLine(baseDir, "players.jsonl", event);
        return Record(baseDir, event);
    }

    bool RecordCharacter(std::string const& baseDir, std::uint64_t playerGuid,
                         std::string const& name, std::uint8_t classId)
    {
        Event event;
        event.timestamp = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
        event.chain = 0;
        event.direction = Direction::Server;
        event.module = "character";
        event.action = "LOGIN";
        event.status = Status::Ok;
        event.reason = name;                                   // the character name
        event.source = "class=" + std::to_string(classId);     // ChrClasses ID
        event.playerGuid = playerGuid;
        AppendLine(baseDir, "players.jsonl", event);  // always, so the name sticks
        return Record(baseDir, event);                 // and again in the event log while listening
    }
}
