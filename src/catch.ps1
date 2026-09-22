$src = "D:\path\to\DizerCore-Housing\src"; $dst = "C:\path\to\DizerCore-WoW\src"  
Get-ChildItem $src -Recurse -Include *.cpp,*.h | Where-Object { $_.FullName -match 'Housing|Neighborhood|Initiative|Postal|DecorDuel|steward|plot' } | ForEach-Object {  
    $rel = $_.FullName.Substring($src.Length)  
    $other = Join-Path $dst $rel  
    if (!(Test-Path $other)) { "MISSING: $rel" }  
    elseif ((Get-FileHash $_.FullName).Hash -ne (Get-FileHash $other).Hash) { "DIFFERS: $rel" }  
}