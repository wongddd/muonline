# MU Online 私服第二版修复日志

**修复日期**: 2026-05-26
**修复版本**: v2.0
**编译配置**: GAMESERVER_UPDATE=502, GAMESERVER_LANGUAGE=1, NEW_PROTOCOL_SYSTEM=1

---

## 一、总览

v2 修复在第一版的基础上，针对已知剩余问题和后续修复建议进行了处理。

**本次新增修改：**

| # | 类型 | 修改/新增 | 对应问题 |
|---|------|----------|---------|
| 1 | 服务端 | `Protocol.h` 添加 PROTOCOL_CODE 编译期 static_assert 验证 | P0#1 |
| 2 | 工具脚本 | 新建 `Sync-Version.ps1` 版本同步工具 | 后续建议#3 |
| 3 | 文档 | 新建 `docs/feature_flags.md` 功能标志映射文档 | P2#5 |
| 4 | 日志 | 输出 v2 修复日志 | — |

---

## 二、详细修改

### 2.1 PROTOCOL_CODE 编译期验证

**文件**: `Source Server/GameServer/GameServer/Protocol.h:49-60`

在 Protocol.h 的 LANGUAGE 条件编译块之后，添加了 `static_assert` 验证：

```cpp
#if GAMESERVER_LANGUAGE == 1
static_assert(PROTOCOL_CODE1 == 0xD4, "PROTOCOL_CODE1 mismatch: client expects 0xD4 (PACKET_MOVE)");
static_assert(PROTOCOL_CODE2 == 0x11, "PROTOCOL_CODE2 mismatch: client expects 0x11 (PACKET_ATTACK)");
static_assert(PROTOCOL_CODE3 == 0x15, "PROTOCOL_CODE3 mismatch: client expects 0x15 (PACKET_POSITION)");
static_assert(PROTOCOL_CODE4 == 0xDB, "PROTOCOL_CODE4 mismatch with client");
#elif GAMESERVER_LANGUAGE != 1
#pragma message("WARNING: GAMESERVER_LANGUAGE is not 1 - PROTOCOL_CODE may not match client!")
#endif
```

**效果**: 编译时即发现 PROTOCOL_CODE 不匹配，防止误编译。避免 v1 修复前 LANGUAGE=0 导致协议不匹配的问题再次发生。

**客户端验证**: 客户端使用 `PACKET_MOVE=0xD4`, `PACKET_ATTACK=0x11`, `PACKET_POSITION=0x15`（定义在 `wsclientinline.h:23-26`），与 LANGUAGE=1 的 PROTOCOL_CODE1-3 完全一致。

---

### 2.2 版本同步脚本

**新建**: `Sync-Version.ps1`

PowerShell 脚本，用于同步客户端 `Main.info` 和服务端 `GameServerInfo - Common.dat` 的版本号与序列号。

**功能：**

| 参数 | 说明 |
|------|------|
| `-Action show` | 显示当前客户端和服务端的版本/序列号（默认） |
| `-Action sync-to-server` | 将客户端版本/序列号写入服务端 Common.dat |
| `-Action sync-to-client` | 将服务端版本/序列号写入客户端 Main.info（含 Tools 副本） |

**Main.info 编码细节:**
- XOR 密钥: `{ 0xFC, 0xCF, 0xAB }`（循环 XOR）
- 结构体大小: 166 字节 (`sizeof(MAIN_FILE_INFO)`)
- 版本偏移: bytes[66-73] (8 字节)
- 序列号偏移: bytes[74-90] (17 字节)

**服务端版本读取细节:**
- `ServerVersion` 字符串 → 取字符 0,2,3,5,6 → 例如 "1.04.05" → "10405"
- `ServerSerial` 直接读取 16 字节

**当前版本：**

| 来源 | Version | Serial |
|------|---------|--------|
| Client Main.info | 1.04.05 | TbYehR2hFUPBKgZj |
| Server Common.dat | 1.04.05 | TbYehR2hFUPBKgZj |

**当前状态**: 两端一致 ✅

---

### 2.3 功能标志映射文档

**新建**: `docs/feature_flags.md`

完整的服务端 28 个 Custom 功能与客户端支持状态对照表。包含：

- **编译期宏定义**: 11 个宏的当前值、含义、客户端需求
- **运行时功能**: 28 个 Custom 功能的开关、配置、协议、客户端状态
- **优先级建议**: 按 P1-P3 排列需要的客户端 UI 开发顺序
- **协议操作码对照**: PROTOCOL_CODE 和关键普通操作码

**客户端状态统计：**

| 状态 | 数量 | 说明 |
|------|------|------|
| ✅ 不需要 | 15 | 通过聊天命令/NPC 对话/现有 UI 工作 |
| ⚠️ 部分需要 | 3 | 部分功能可用 |
| ❌ 缺失 | 5 | 需要客户端 UI 或协议处理 |
| 不适用 | 3 | 服务端已禁用 |

**优先实现建议：**

| 优先级 | 功能 | 原因 |
|--------|------|------|
| P1 | CustomRanking | 服务端发送排行数据，客户端无显示 |
| P1 | CustomEventTime | 服务端发送事件倒计时，客户端无显示 |
| P2 | CustomBuyVip | 玩家无法看到 VIP 购买界面 |
| P2 | CustomArena | 玩家无法看到竞技场界面 |
| P2 | CustomWing | 自定义翅膀物品无客户端模型渲染 |

---

## 三、剩余问题状态更新

| 原编号 | 问题 | v2 状态 | 说明 |
|--------|------|---------|------|
| P0#1 | PROTOCOL_CODE 匹配 | ✅ 已修复 | 添加编译期 static_assert |
| P0#2 | SimpleModulus XOR 验证 | ⚠️ 已验证（文档记录） | 服务端 XOR 值 `0x3F08A79B,0xE25CC287,0x93D27AB9,0x20DEA7BF` 在 PacketManager.cpp:37-40；客户端 `s_dwSaveLoadXOR` 在闭源 `SimpleModulus.lib` 中，需运行期测试验证 |
| P1#3 | 28 个 Custom 功能无客户端 UI | 📋 已建档 | 见 `docs/feature_flags.md`，标注了每项的状态和优先级 |
| P1#4 | 渲染以固定管线为主 | ⏳ 基础设施就绪 | 着色器类已就绪（ZzzShader.cpp），内置地形/纹理着色器可用，需后续逐步迁移 |
| P2#5 | 功能标志命名独立 | ✅ 已建档 | 见 `docs/feature_flags.md` |
| P2#6 | 无自动化测试 | ⏳ 待规划 | 超出本次范围 |
| 后续建议#1 | Custom 客户端 UI | 📋 已建档 | 优先级见上表 |
| 后续建议#2 | 着色器迁移 | ⏳ 渐进进行 | 地形 VBO+Shader 需较大重构，建议从简单渲染路径开始 |
| 后续建议#3 | 版本号同步脚本 | ✅ 已创建 | `Sync-Version.ps1` |

---

## 四、项目文件结构变化

### v1 → v2 新增文件

```
E:\23\
├── Sync-Version.ps1                    # 新增：版本同步工具
├── docs/
│   └── feature_flags.md                # 新增：功能标志映射文档
├── build_server.bat                    # 新增：服务端构建脚本
└── fix_log_v2.md                       # 本文件
```

---

## 五、编译状态

| 组件 | v1 状态 | v2 状态 | 说明 |
|------|---------|---------|------|
| GameServer (Release_EX502) | ✅ 通过 | ✅ 通过 | Protocol.h static_assert 使用纯常量表达式，不影响编译 |
| 客户端 (Global Release x86) | ✅ 通过 | ✅ 通过 | 无源码修改 |

---

## 六、运行注意事项

1. **首次运行前**：确认 `Main.info` 与 `GameServerInfo - Common.dat` 版本一致（运行 `Sync-Version.ps1 -Action show` 检查）
2. **修改版本后**：使用 `Sync-Version.ps1 -Action sync-to-server` 或 `sync-to-client` 保持两端一致
3. **Custom 功能**：目前 Custom 功能大多通过聊天命令工作（如 `/move`, `/reset`, `/add` 等），无需客户端 UI 的功能可直接使用。需要 UI 的功能（如 Ranking、Arena）请参考 `docs/feature_flags.md` 的优先级

---

## 七、联调测试结果

**测试时间**: 2026-05-26 03:33–03:45

### 7.1 运行环境修复

| 问题 | 修复措施 | 结果 |
|------|---------|------|
| SQL Server 2008 R2 Express 服务不存在 | `sc create MSSQLSERVER` 新建服务 | ✅ |
| SQL Server 启动失败 (error 17058) | 修复注册表 Parameters (SQLArg0/1/2) + ImagePath 含完整 -d -e -l 参数 | ✅ |
| NETWORK SERVICE 权限不足 | `icacls` 授予 DATA/LOG/Binn 目录 Full Control | ✅ |
| SQL Server TCP/IP 已禁用 | 确认 localhost 连接使用 Named Pipes 正常工作 | ✅（不影响本地通信） |
| 服务非自动启动 | `sc config MSSQLSERVER start= auto` | ✅ |

### 7.2 服务器启动流程

| 步骤 | 组件 | 端口 | 启动顺序 | 状态 |
|------|------|------|---------|------|
| 0 | MSSQLSERVER | 1433 | 最先 | ✅ RUNNING |
| 1 | DataServer | 55960 | 第 1 | ✅ LISTEN |
| 2 | JoinServer | 55970 | 第 2 | ✅ LISTEN |
| 3 | ConnectServer | 44405 | 第 3 | ✅ LISTEN |
| 4 | GameServer | 55901 | 最后（需 ConnectServer 就绪） | ✅ LISTEN |

### 7.3 客户端连接验证

| 阶段 | 客户端行为 | 服务端日志 | 结果 |
|------|-----------|-----------|------|
| 1. 连接 ConnectServer | 连接到 127.0.0.1:44405 | — | ✅ |
| 2. 获取服务器列表 | 请求服务器列表 | — | ✅ |
| 3. 选择服务器 | 选择 GameServer (197.159.75.59:55901) | `AddClientNew (197.159.75.59)` | ✅ |
| 4. 登录认证 | 发送账号密码到 JoinServer | `AddAccount (111)` | ✅ |
| 5. 角色选择 | 请求角色列表，选择角色 | `LogInCharacter` | ✅ |
| 6. 进入游戏世界 | 角色出现在游戏中 | `PacketIN: 0x11` (攻击) | ✅ |

### 7.4 GameServer 通信日志（节选）

```
03:40:02 AddClientNew (197.159.75.59)     ← 客户端连接
03:40:21 AddAccount (111)                  ← 账号认证通过
03:40:23 PacketIN: C10331                  ← 登录协议包
03:40:47 PacketIN: C10EF303...             ← 角色选择
03:40:47 LogInCharacter (角色名)            ← 角色进入游戏
03:40:47 PacketIN: C10803... (PACKET_MOVE)  ← 角色移动
03:40:48 PacketIN: C10866...               ← 角色动作
03:43:07 — 03:44:09 持续 0x11 PACKET_ATTACK 和 0x1E ← 持续游戏操作
```

### 7.5 验证结论

| 验证项 | 结果 | 说明 |
|--------|------|------|
| 编译配置一致性 | ✅ | GAMESERVER_UPDATE=502, LANGUAGE=1, PROTOCOL_CODE1=0xD4 等均匹配 |
| 登录包结构 | ✅ | NEW_PROTOCOL_SYSTEM 启用，password[20] 对齐 |
| SimpleModulus 加密 | ✅ | UPDATE=502 < 701 自动走 SimpleModulus 路径 |
| 客户端版本/序列号 | ✅ | 两端均为 1.04.05 / TbYehR2hFUPBKgZj |
| 账号注册与认证 | ✅ | 账号 111/111, 222/222 等在 MuOnline 数据库 MEMB_INFO 表中，认证通过 |
| 游戏数据包通信 | ✅ | 客户端发送的 0x11 (攻击), 0x1E (动作) 被服务端正常接收处理 |
| 服务稳定性 | ✅ | 所有 4 个服务端组件连续运行 10+ 分钟无崩溃 |

### 7.6 已知问题

| 问题 | 严重度 | 说明 |
|------|--------|------|
| 角色名显示乱码 | 低 | 服务端日志显示角色名为乱码（编码问题），不影响游戏逻辑 |
| 渲染为 OpenGL 1.x | 中 | 着色器基础设施已添加（ZzzShader.cpp），但尚未完全迁移 |
| 5 个 Custom 功能需客户端 UI | 中 | 见 `docs/feature_flags.md` P1-P3 优先级 |
| 无自动化测试 | 低 | 超出本次范围 |

---

*本日志由 Claude Code 自动生成*
*生成时间: 2026-05-26*
