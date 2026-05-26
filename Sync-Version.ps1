param(
    [ValidateSet("show", "sync-to-server", "sync-to-client")]
    [string]$Action = "show",

    [string]$MainInfoPath = "E:\23\Client\Main.info",
    [string]$ServerCommonDatPath = "E:\23\MuServer\GameServer\DATA\GameServerInfo - Common.dat",
    [string]$MainInfoCopyPath = "E:\23\MuServer\Tools\MAIN_INFO\Main.info"
)

$XorKey = @(0xFC, 0xCF, 0xAB)

function Read-MainInfo {
    param([string]$Path)
    if (-not (Test-Path $Path)) { Write-Error "File not found: $Path"; return $null }

    $bytes = [System.IO.File]::ReadAllBytes($Path)
    for ($i = 0; $i -lt $bytes.Length; $i++) {
        $bytes[$i] = $bytes[$i] -bxor $XorKey[$i % 3]
    }

    $info = @{
        CustomerName  = [System.Text.Encoding]::ASCII.GetString($bytes, 0, 32).TrimEnd("`0")
        IpAddress     = [System.Text.Encoding]::ASCII.GetString($bytes, 32, 32).TrimEnd("`0")
        IpAddressPort = [System.BitConverter]::ToUInt16($bytes, 64)
        ClientVersion = [System.Text.Encoding]::ASCII.GetString($bytes, 66, 8).TrimEnd("`0")
        ClientSerial  = [System.Text.Encoding]::ASCII.GetString($bytes, 74, 17).TrimEnd("`0")
        WindowName    = [System.Text.Encoding]::ASCII.GetString($bytes, 91, 32).TrimEnd("`0")
        ScreenShotPath = [System.Text.Encoding]::ASCII.GetString($bytes, 123, 50).TrimEnd("`0")
        RawBytes      = $bytes
    }
    return $info
}

function Write-MainInfo {
    param([string]$Path, [byte[]]$DecodedBytes)
    $encoded = $DecodedBytes.Clone()
    for ($i = 0; $i -lt $encoded.Length; $i++) {
        $encoded[$i] = $encoded[$i] -bxor $XorKey[$i % 3]
    }
    [System.IO.File]::WriteAllBytes($Path, $encoded)
    Write-Host "  [OK] Wrote $Path" -ForegroundColor Green
}

function Read-ServerVersion {
    param([string]$Path)
    if (-not (Test-Path $Path)) { Write-Error "File not found: $Path"; return $null }

    $content = Get-Content $Path -Raw
    $result = @{}

    if ($content -match '(?m)^ServerVersion\s*=\s*(\S+)') {
        $result.ServerVersion = $matches[1]
    }
    if ($content -match '(?m)^ServerSerial\s*=\s*(\S+)') {
        $result.ServerSerial = $matches[1]
    }
    return $result
}

function Write-ServerVersion {
    param([string]$Path, [string]$Version, [string]$Serial)
    $content = Get-Content $Path
    $changed = $false

    $newContent = $content | ForEach-Object {
        if ($_ -match '^(ServerVersion\s*=\s*).*') {
            if ($matches[1].Trim() -ne "ServerVersion = $Version") {
                $changed = $true
                "ServerVersion = $Version"
            } else { $_ }
        } elseif ($_ -match '^(ServerSerial\s*=\s*).*') {
            if ($matches[1].Trim() -ne "ServerSerial = $Serial") {
                $changed = $true
                "ServerSerial = $Serial"
            } else { $_ }
        } else { $_ }
    }

    if ($changed) {
        $newContent | Set-Content $Path -Encoding ASCII
        Write-Host "  [OK] Updated $Path" -ForegroundColor Green
    } else {
        Write-Host "  [--] No change needed for $Path" -ForegroundColor Yellow
    }
}

# ===== Main =====
Write-Host "============================================" -ForegroundColor Cyan
Write-Host "  MU Version Synchronization Tool" -ForegroundColor Cyan
Write-Host "============================================" -ForegroundColor Cyan
Write-Host ""

# Read both configs
$clientInfo = Read-MainInfo -Path $MainInfoPath
$serverInfo = Read-ServerVersion -Path $ServerCommonDatPath

if (-not $clientInfo -or -not $serverInfo) { exit 1 }

Write-Host "--- Current State ---" -ForegroundColor Yellow
Write-Host ("  Client Version : {0}" -f $clientInfo.ClientVersion)
Write-Host ("  Client Serial  : {0}" -f $clientInfo.ClientSerial)
Write-Host ("  Client IP      : {0}:{1}" -f $clientInfo.IpAddress, $clientInfo.IpAddressPort)
Write-Host ("  Server Version : {0}" -f $serverInfo.ServerVersion)
Write-Host ("  Server Serial  : {0}" -f $serverInfo.ServerSerial)
Write-Host ""

# Compare
if ($clientInfo.ClientVersion -eq $serverInfo.ServerVersion -and $clientInfo.ClientSerial -eq $serverInfo.ServerSerial) {
    Write-Host "  [OK] Version and Serial match." -ForegroundColor Green
} else {
    Write-Host "  [!!] Version MISMATCH!" -ForegroundColor Red
}
Write-Host ""

switch ($Action) {
    "show" {
        Write-Host "Usage: .\Sync-Version.ps1 -Action <show|sync-to-server|sync-to-client>"
        Write-Host "  sync-to-server : Copy Client version/serial -> Server Common.dat"
        Write-Host "  sync-to-client : Copy Server version/serial -> Client Main.info"
    }
    "sync-to-server" {
        Write-Host "--- Syncing client -> server ---" -ForegroundColor Yellow
        Write-ServerVersion -Path $ServerCommonDatPath -Version $clientInfo.ClientVersion -Serial $clientInfo.ClientSerial
    }
    "sync-to-client" {
        Write-Host "--- Syncing server -> client ---" -ForegroundColor Yellow
        $bytes = $clientInfo.RawBytes
        $verBytes = [System.Text.Encoding]::ASCII.GetBytes($serverInfo.ServerVersion.PadRight(8, "`0").Substring(0,8))
        $serBytes = [System.Text.Encoding]::ASCII.GetBytes($serverInfo.ServerSerial.PadRight(17, "`0").Substring(0,17))
        for ($i = 0; $i -lt 8; $i++) { $bytes[66 + $i] = $verBytes[$i] }
        for ($i = 0; $i -lt 17; $i++) { $bytes[74 + $i] = $serBytes[$i] }

        Write-MainInfo -Path $MainInfoPath -DecodedBytes $bytes
        if ($MainInfoCopyPath -and (Test-Path $MainInfoCopyPath)) {
            Write-MainInfo -Path $MainInfoCopyPath -DecodedBytes $bytes
        }
    }
}
