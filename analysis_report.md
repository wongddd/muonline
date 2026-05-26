# MU Online 私服项目深度分析报告

**生成时间**: 2026-05-26
**分析工具**: Claude Code (Agent x4 + 直接文件分析)
**使用工具**: Bash, PowerShell

---

## 一、系统总览

该仓库是 **MU Online (奇迹MU) Season 8 / EX502 私服** 的全套源码和部署配置，包含：

| 项目 | 语言 | 构建工具 | 说明 |
|------|------|----------|------|
| GameServer | C++ | VS2019 (v142) | 核心游戏逻辑服务器 |
| ConnectServer | C++ | VS2019 (v142) | 连接/服务器列表服务器 |
| DataServer | C++ | VS2019 (v142) | 数据库网关层 |
| JoinServer | C++ | VS2019 (v142) | 账号认证服务器 |
| Client (Main) | C++ | VS2019 (v142) | 游戏客户端 (OpenGL 1.x) |
| MuServer | - | - | 部署配置和 SQL 数据库 |

---

## 二、项目依赖关系

```
                    +------------------+
                    |   客户端 (Main)   |
                    |  C++ / OpenGL    |
                    |  源: Source Client|
                    +--------+---------+
                             | TCP
                             v
                    +------------------+
                    |  ConnectServer   |  <-- 客户端入口，端口 44405
                    |  IOCP + UDP      |  <-- 接收 GameServer/JoinServer 心跳(UDP 55557)
                    +--------+---------+
                             |
                     TCP     |     UDP (心跳)
               +-------------+-------------+
               |                           |
               v                           v
        +-----------+             +------------------+
        | JoinServer|             |   GameServer     |  <-- 核心游戏逻辑
        | ODBC: SQL | <--TCP-->   |  IOCP/ASIO       |  <-- 客户端连接端口 55901
        | 端口 55970|             |  Lua 5.1 脚本    |
        +-----------+             +--------+---------+
               |                           |
               | TCP                       | TCP
               +-------------+-------------+
                             |
                             v
                     +-------------+
                     | DataServer  |  <-- 数据库网关
                     | ODBC: SQL   |  <-- 端口 55960
                     +------+------+
                            |
                      +-----+-----+
                      | SQL Server |
                      | MuOnline   |
                      +-----------+

内部通信:
  GameServer <-> JoinServer : TCP, 自定义协议 (账号认证/地图移动)
  GameServer <-> DataServer : TCP, 自定义协议 (角色/物品/任务数据)
  GameServer -> ConnectServer : UDP (心跳/在线人数上报)
  JoinServer -> ConnectServer : UDP (心跳上报)
  Client -> ConnectServer : TCP (获取服务器列表/IP)
  Client -> GameServer : TCP (游戏通信)

数据库 (SQL Server):
  GameServer 从不直接访问数据库
  JoinServer 通过 ODBC 访问 MEMB_INFO 等账号表
  DataServer 通过 ODBC 访问 Character/Inventory/Guild/Warehouse 等 72+ 张表
```

---

## 三、通信协议深度解析

### 3.1 数据包格式

经典协议使用两种基本帧类型，通过首字节区分：

**固定长度包 (0xC1 / 0xC3 加密):**
```
[0xC1] [Size:1B] [Head:1B] [Data...]
```

**可变长度包 (0xC2 / 0xC4 加密):**
```
[0xC2] [SizeH:1B] [SizeL:1B] [Head:1B] [Data...]
```

加密变体 0xC3/0xC4 在 header 后增加 1 字节序列号，数据区用 SimpleModulus 加密。

新协议系统 (ASIO) 使用：
```
[Header: {ID:2B, Size:4B}] [Body: Size bytes]
```

### 3.2 客户端 -> ConnectServer

| 方向 | 数据包 | 说明 |
|------|--------|------|
| C->CS | `C1:F4:03` + ServerCode | 请求指定服务器信息 |
| CS->C | `C1:F4:03` + IP[16] + Port | 返回目标服务器地址 |
| C->CS | `C1:F4:06` | 请求服务器列表 |
| CS->C | `C2:F4:06` + count + entries[] | 返回服务器列表 |

### 3.3 客户端 -> GameServer (核心协议)

**Protocol.h** 定义了 100+ 种协议结构体。关键消息头范围：

| Head | 方向 | 功能 |
|------|------|------|
| 0x00 | C<->S | 聊天消息 |
| 0x03 | C->S | Main 检查 (反作弊) |
| 0x0E | C->S | 客户端存活 (含速度检测) |
| PROTOCOL_CODE2 (0x11) | C->S | 攻击 |
| PROTOCOL_CODE1 (0xD4) | C->S | 移动 |
| PROTOCOL_CODE3 (0x15) | C->S | 位置同步 |
| 0x18 | C->S | 动作 |
| 0x19 | C->S | 技能攻击 |
| 0x1C | C->S | 瞬移 |
| 0x22-0x26 | C->S | 物品(拾取/丢弃/移动/使用) |
| 0x30-0x34 | C->S | NPC 对话/购买/出售/修理 |
| 0x36-0x3D | C->S | 交易系统 |
| 0x3F | C->S | 个人商店(摆摊) |
| 0x40-0x43 | C->S | 队伍系统 |
| 0x50-0x53 | C->S | 公会系统 |
| 0xF1 | C<->S | 登录/认证系列 |
| 0xF3 | C<->S | 角色管理系列 |

### 3.4 GameServer <-> JoinServer

GameServer -> JoinServer (通过 `CConnection` TCP):

| Head | 功能 |
|------|------|
| 0x00 | GameServer 注册信息 |
| 0x01 | 连接账户请求 (含密码验证) |
| 0x02 | 断开账户请求 |
| 0x03 | 地图服务器移动请求 |
| 0x04 | 地图移动认证请求 |
| 0x05/0x06 | 账户等级查询 |
| 0x11 | 账户等级/锁定保存 |

### 3.5 GameServer <-> DataServer

GameServer -> DataServer (通过 `CConnection` TCP):

| Head | 功能 | 读写 |
|------|------|------|
| 0x01-0x04 | 角色列表/创建/删除/信息 | 读 |
| 0x05 | 仓库操作 | 读 |
| 0x07 | 创建物品序列号 | 读 |
| 0x08-0x29 | 系统数据(任务/技能/商城/Gens等) | 读 |
| 0x30-0x31 | 角色/背包信息保存 | 写 |
| 0x33-0x34 | 选项/宠物保存 | 写 |
| 0x39-0x42 | 排行榜/角色卡保存 | 写 |
| 0x80-0x89 | 罗兰攻城数据 | 读/写 |
| 0xF0-0xF7 | 自定义功能数据 | 读/写 |

### 3.6 加密体系

```
客户端发送: BuxConvert(账号密码) -> SimpleModulus Encrypt(8B->11B) -> TCP
客户端接收: TCP -> SimpleModulus Decrypt -> 明文协议
服务器发送: 明文协议 -> (可选加密) -> TCP
服务器接收: TCP -> (可选解密) -> ProtocolCore(head, data, ...)
```

多语言混淆码: PROTOCOL_CODE1~CODE4 根据 GAMESERVER_LANGUAGE 宏设置不同的字节值。

---

## 四、脑图 (Mind Map)

```mermaid
mindmap
  root((MU Online 私服))
    Source_Server
      GameServer
        核心管理器
          ObjectManager
          MonsterManager
          ItemManager
          SkillManager
          EffectManager
        网络层
          SocketManager_IOCP
          SocketManagerModern_ASIO
          SocketManagerUDP
          CConnection_JoinServer
          CConnection_DataServer
        协议系统
          Protocol.h/cpp
          DSProtocol.h/cpp
          JSProtocol.h/cpp
          CSProtocol.h/cpp
          ESProtocol.h/cpp
        事件系统
          BloodCastle
          DevilSquare
          ChaosCastle
          Crywolf
          Kanturu
          CastleSiege
          IllusionTemple
          ImperialGuardian
          DoubleGoer
          ArcaBattle
        脚本扩展
          Lua_5.1
          MonsterDie.lua
          NpcTalk.lua
          EventDrop.lua
        自定义扩展
          CustomArena
          CustomJewel
          CustomWing
          CustomQuest
          CustomStore
          CustomOnlineLottery
          CustomRanking
          28个Custom模块
      ConnectServer
        ClientManager
        ServerList
        IOCP_TCP_44405
        UDP_55557
      JoinServer
        AccountManager
        QueryManager_ODBC
        IOCP_TCP_55970
        账号认证
        地图移动
      DataServer
        QueryManager_ODBC
        ServerManager
        IOCP_TCP_55960
        角色数据
        物品数据
        公会数据
        仓库数据
        商城数据
        排行榜
        城堡数据
    Source_Client
      主循环
        Winmain
        Scene状态机
        ProtocolCompiler
      网络层
        WSclient_经典协议
        ProtocolAsio_新协议ASIO
        SimpleModulus_加密
        StreamPacketEngine
      渲染引擎
        OpenGL_1.x固定管线
        ZzzOpenglUtil
        ZzzTexture
        GlobalBitmap
      3D系统
        ZzzBMD_SMD模型
        ZzzObject
        ZzzCharacter
        ZzzEffect
      场景管理
        ZzzScene
        ZzzInterface
        ZzzLodTerrain
      新UI系统
        CNewUIManager
        INewUIBase接口
        70+UI组件
      安全系统
        SimpleModulus
        NProtect
        Themida_VM
        ProtectSysKey
        内存反扫描
    MuServer_部署
      配置数据
        Data目录_15个子目录
        GameServerInfo_DAT文件
        MapServerInfo.dat
      数据库
        MEMB_INFO
        Character
        AccountCharacter
        Guild/GuildMember
        Warehouse/ExtWarehouse
        CashShopData
        MasterSkillTree
        72+张表
      服务器
        ConnectServer.exe
        JoinServer.exe
        DataServer.exe
        GameServer.exe
        GameServerCS.exe
  ```

---

## 五、二次开发可行性分析

### 5.1 有利因素

| 因素 | 说明 |
|------|------|
| **源码完整** | 所有服务器组件和客户端均有完整 C++ 源码 |
| **协议完整定义** | Protocol.h 和 WSclient.h 完整定义了所有通信结构 |
| **模块化架构** | 管理器模式、UI 接口化、协议和逻辑分离 |
| **已有的自定义系统** | 28 个 Custom* 模块可直接扩展 |
| **Lua 脚本接口** | 事件掉落、NPC 对话可通过 Lua 定制 |
| **双协议系统** | 经典 IOCP 和新 ASIO 均可选，兼容性好 |
| **配置热重载** | 大部分配置支持运行时重载 |
| **VS2019 标准工具链** | v142 + C++17，开发者环境易于搭建 |

### 5.2 挑战

| 挑战 | 详情 |
|------|------|
| **无测试体系** | 没有任何单元测试或集成测试 |
| **全局变量泛滥** | 大量 extern 全局单例，线程安全需谨慎 |
| **硬编码路径** | 文件路径、端口号、协议代码硬编码 |
| **加密依赖** | SimpleModulus 的密钥文件和 Themida 保护可能丢失 |
| **ODBC 耦合** | 数据库操作强依赖 SQL Server ODBC |
| **客户端老旧** | OpenGL 1.x 固定管线，无 Shader 支持 |
| **代码质量不均** | 部分文件(如 wsclientinline.h) 使用 __forceinline 宏和内联大段代码 |
| **文档缺失** | 除了自动生成的 ReadMe.txt 外无开发文档 |

### 5.3 推荐二次开发方向 (难度从低到高)

| 优先级 | 方向 | 所需修改 | 难度 |
|--------|------|----------|------|
| 1 | 新增自定义物品/技能/怪物 | Custom*.cpp + Data 配置 | ★☆☆ |
| 2 | 新增游戏事件 | 参考 CustomArena 实现 | ★★☆ |
| 3 | 修改经济系统/掉落/合成 | Data 配置 + CItemDrop/CustomMix | ★☆☆ |
| 4 | 扩展数据库(新表) | DataServerProtocol + SQL | ★★☆ |
| 5 | 新增 Lua 脚本 | lua/ 目录添加 .lua 文件 | ★★☆ |
| 6 | 自定义客户端 UI | 添加 NewUI*.cpp/h | ★★☆ |
| 7 | 迁移至 ASIO 新协议 | 启用 NEW_PROTOCOL_SYSTEM | ★★★ |
| 8 | 接入 OSS/Redis/新数据库 | 替换 QueryManager | ★★★ |
| 9 | 客户端渲染升级(Shader) | OpenGL3/Shader 目录 | ★★★★ |
| 10 | 全平台服务端(Linux) | IOCP->epoll, ODBC->连接池 | ★★★★★ |
| 11 | Web 管理面板/API | 新增 HTTP 服务模块 | ★★★ |
| 12 | 微服务化拆分 | 重构 GameServer 事件模块 | ★★★★★ |

### 5.4 协议修改注意事项

1. **修改协议结构体**时必须同步更新客户端和服务端的结构体定义
2. **SimpleModulus 密钥** 文件 `Enc1.dat` / `Dec2.dat` 需要在客户端和服务器配对
3. **协议版本控制**: `GAMESERVER_UPDATE` (401/502/603/803) 控制协议兼容性
4. **新增协议头**时需要在 ProtocolCore() switch 中添加，并在客户端 TranslateProtocol() 中添加
5. **加密开关**: `PROTOCOL_CODE1-4` 随语言版本变化，新增协议时注意
6. **序列同步**: 物品序列号通过 `WZ_GetItemSerial` 存储过程统一分配

---

## 六、分析日志

```
[分析开始] 2026-05-26
  -> Agent: 分析GameServer架构 [后台]
  -> Agent: 分析Client架构 [后台]
  -> Agent: 分析ConnectServer和JoinServer [后台]
  -> Agent: 分析MuServer配置和数据库 [后台]
  -> 直接文件读取: Protocol.h, SocketManager.h, ProtocolAsio.h,
     SocketManagerModern.h, SocketConnection.h, DSProtocol.h,
     WSclient.h (2710行), Connection.h, GameMain.cpp, Winmain.cpp,
     Protocol.cpp, SimpleModulus.h, SocketManagerEnum.h
  -> 工具: Bash 用于 ls/head 目录探索
  -> 工具: PowerShell 未使用
[分析完成] 脑图和报告已生成
```

---

## 七、关键端口汇总

| 服务 | 端口 | 协议 | 方向 |
|------|------|------|------|
| ConnectServer | 44405 | TCP | 客户端入站 |
| ConnectServer | 55557 | UDP | 心跳入站 (GameServer/JoinServer) |
| JoinServer | 55970 | TCP | GameServer 入站 |
| DataServer | 55960 | TCP | GameServer 入站 |
| GameServer | 55901 | TCP | 客户端入站 (GS) |
| GameServerCS | 55919 | TCP | 客户端入站 (GSCS) |
| SQL Server | 1433 | TCP | 本机数据库 |
