/*
 * VDU conduit: local event writer for the telemetry visualiser.
 *
 * Writes one JSON object per line to vdu/events.jsonl, but only while the
 * flag file vdu/listen.on exists. Nothing here depends on TrinityCore types,
 * so the writer can be tested on its own.
 */

#ifndef VDU_CONDUIT_H
#define VDU_CONDUIT_H

#include <cstdint>
#include <string>

namespace Vdu
{
    enum class Status : std::uint8_t
    {
        Ok,
        Warn,
        Fail
    };

    enum class Direction : std::uint8_t
    {
        In,     // client -> server
        Out,    // server -> client
        Server  // server-side outcome check, not a packet
    };

    struct Event
    {
        std::int64_t timestamp = 0;      // unix seconds
        std::uint64_t chain = 0;         // links related events together
        std::string session;             // listen-on session id
        Direction direction = Direction::Server;
        std::string opcode;              // e.g. CMSG_CAST_SPELL, or empty
        std::string module;              // e.g. spell, quest, npc
        std::string action;              // e.g. CAST, ACCEPT, APPLY
        Status status = Status::Ok;
        std::string reason;              // e.g. SPELL_FAILED_OUT_OF_RANGE
        std::string source;              // e.g. Spell.cpp:5733
        std::uint64_t playerGuid = 0;
        std::string payloadHex;          // short preview, may be empty
    };

    // Returns a JSON object (no trailing newline) for one event.
    std::string ToJson(Event const& event);

    // Escapes a string for inclusion inside JSON double quotes.
    std::string JsonEscape(std::string const& text);

    // True when vdu/listen.on exists under the given base directory.
    bool IsListening(std::string const& baseDir);

    // Appends one event as one line to <baseDir>/events.jsonl if listening.
    // Returns true if the line was written. Thread-safe.
    bool Record(std::string const& baseDir, Event const& event);

    // Returns a new non-zero chain id. Thread-safe.
    std::uint64_t NewChain();

    // Folder the server writes to, relative to the worldserver working directory.
    inline constexpr char const* kBaseDir = "vdu";

    // First maxBytes of a packet as lowercase hex, space separated. Adds "..." when cut short.
    std::string HexPreview(std::uint8_t const* data, std::size_t size, std::size_t maxBytes = 96);

    // How many bytes of a packet the tap keeps by default. Packets listed in the table
    // in VduConduit.cpp get their own, larger size.
    inline constexpr std::size_t kDefaultPreviewBytes = 96;

    // The bytes to keep for one opcode. The tap should call
    // HexPreview(data, size, PreviewBytesFor(opcodeName)).
    std::size_t PreviewBytesFor(std::string const& opcodeName);

    // True for the packets kept even when not listening (quests, spells, pets, names).
    // The tap checks this before it builds a packet's preview, so other packets cost nothing.
    bool WantedWithoutListening(std::string const& opcodeName);

    // Appends one event as one line to <baseDir>/<fileName>, whether or not listening.
    // Used for the small player file, so names survive a session that was not listened to.
    bool AppendLine(std::string const& baseDir, char const* fileName, Event const& event);

    // Records who is logged in: name and class, keyed by the player's GUID counter.
    // Written to players.jsonl every time, and to events.jsonl while listening, so the
    // viewer can name players even when listening was off at login.
    bool RecordCharacter(std::string const& baseDir, std::uint64_t playerGuid,
                         std::string const& name, std::uint8_t classId);

    // Packet-level helper used by the tap. Takes only primitives so it has no
    // TrinityCore dependencies. Returns true if the line was written.
    bool RecordPacket(std::string const& baseDir, Direction direction,
                      std::string const& opcodeName, std::uint64_t playerGuid,
                      std::size_t size, std::string const& payloadHex);
}

#endif // VDU_CONDUIT_H
