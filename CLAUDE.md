# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Language

请用中文回复。所有对话、解释、代码注释和文档请使用中文。

## Project Overview

This is an **MU Online private server** (Season 8 / EX502) with four server components and a game client, all written in C++. The server infrastructure is configured for deployment in `MuServer/`, with source code in `Source Server/` and `Source Client/`.

## Project Structure

```
E:\23\
├── Source Server/           # Server-side C++ source code
│   ├── GameServer/          # Main game logic (combat, items, monsters, events, etc.)
│   │   └── GameServer/      # VC++ project (GameServer.sln)
│   ├── ConnectServer/       # Server list & connection routing
│   │   └── ConnectServer/   # VC++ project
│   ├── DataServer/          # Database layer (SQL queries for characters, guilds, items, etc.)
│   │   └── DataServer/      # VC++ project
│   ├── JoinServer/          # Account authentication
│   │   └── JoinServer/      # VC++ project
│   └── Util/                # Utilities (CRC32, Crypto++ library)
├── Source Client/           # Game client C++ source code
│   ├── Main.sln             # Visual Studio 2019 solution
│   ├── Main.vcxproj         # Client project
│   ├── source/              # Client source files (~500+ .cpp/.h files)
│   │   ├── Winmain.cpp      # Entry point
│   │   ├── Zzz*.cpp/h       # Core subsystems (AI, BMD, Character, Effect, Scene, Texture, etc.)
│   │   ├── NewUI*.cpp/h     # UI system (inventory, shops, party, guild, quests, etc.)
│   │   ├── GM*.cpp/h        # Game management / event maps (BloodCastle, DevilSquare, etc.)
│   │   ├── w_*.cpp/h        # Pet system, buff system, map processing
│   │   └── Custom*.cpp/h    # Custom modifications
│   ├── dependencies/        # Third-party libs (Boost 1.75, ASIO, etc.)
│   └── sdk-directx9/        # DirectX 9 SDK
├── MuServer/                # Deployed server (binaries + configuration + SQL)
│   ├── Data/                # Config files (items, monsters, shops, events, maps, quests)
│   ├── GameServer/          # GameServer binary + DATA config .dat files
│   ├── DataServer/          # DataServer binary + config
│   ├── ConnectServer/       # ConnectServer binary
│   ├── JoinServer/          # JoinServer binary
│   ├── DB/                  # SQL database files (MuOnline.sql, MuOnlineS4.mdf/.ldf)
│   └── StartUp/             # Server startup configuration
└── Client/                  # Pre-built client
```

## Architecture

### Server Components

All servers are Win32 C++ applications using raw WinSock2 for networking, built with Visual Studio 2019 (v142 toolset, Windows Target 10.0, MultiByte charset):

- **GameServer**: Core game simulation. Handles player connections, combat, items, skills, AI, quests, events, monster management, parties, guilds, and all game systems. Uses Lua 5.1 for scripting (event drops, NPC talk, monster death). Communicates with DataServer and JoinServer via custom protocol.
- **DataServer**: Database abstraction layer. Manages SQL connections and queries for character data, items, guilds, warehouses, etc.
- **ConnectServer**: Entry point for clients. Maintains server list and routes connections to appropriate GameServer instances.
- **JoinServer**: Handles account login/authentication.

### Client

Win32 application with DirectX 9 / OpenGL 3 rendering. Built with Visual Studio 2019 (v142). Dependencies: Boost 1.75, ASIO, DirectX SDK 6.1, OpenGL, custom libs (SimpleModulus, wzAudio, etc.). Key subsystems:
- **NewUI_***: UI framework (inventory, shops, party, guild, quests, minimap, etc.)
- **Zzz* files**: Core engine (AI, BMD models, character rendering, effects, scene management, texture loading, terrain)
- **GM_***: Event/game-mode specific logic (Blood Castle, Devil Square, Crywolf, Kanturu, etc.)
- **w_***: Pet system, buff system, map processing
- **Custom***: Custom modifications/extensions

### Build Configurations

- **GameServer**: Debug | Release_EX502 | Release_EX502CS (Win32)
- **Client**: Global Debug | Global Release (Win32/x64)

## Build Commands

All projects require Visual Studio 2019 (v142 toolset) on Windows.

### GameServer
```
# Open Source Server\GameServer\GameServer.sln and build in VS2019
# Or via command line:
msbuild "Source Server\GameServer\GameServer.sln" /p:Configuration=Release_EX502;Platform=Win32
```

### Client
```
# Open Source Client\Main.sln and build in VS2019
msbuild "Source Client\Main.sln" /p:Configuration="Global Release";Platform=Win32
```

### ConnectServer / DataServer / JoinServer
Each has its own `.sln` in `Source Server\{Component}\` with a matching `.vcxproj`.

## Key Configuration Files

- `MuServer\Data\` - Server-side game data (items, monsters, shops, events, maps, quests)
- `MuServer\GameServer\DATA\GameServerInfo - *.dat` - Server behavior configs
- `MuServer\DB\MuOnline.sql` - Database schema
- Client `.bmd` / `.ozj` files in `Client\Data\` - 3D models, textures, terrain

## Networking

Custom binary protocol over TCP. Packets are serialized with custom structures (no JSON/Protobuf). Protocol handlers are in `Protocol.cpp/.h`, `DSProtocol.cpp/.h`, `JSProtocol.cpp/.h`, `ESProtocol.cpp/.h`, `CSProtocol.cpp/.h`.
