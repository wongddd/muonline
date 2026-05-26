# MU Online 私服第一版修复日志

**修复日期**: 2026-05-26
**修复版本**: v1.0
**编译配置**: GAMESERVER_UPDATE=502, GAMESERVER_LANGUAGE=1, NEW_PROTOCOL_SYSTEM=1

---

## 一、修复总览

本次修复基于客户端与服务端匹配分析报告的结论，以"服务端使用 GAMESERVER_UPDATE=502 编译"为核心前提，对服务端编译配置、客户端渲染管线、通信协议一致性进行了系统性修复。

---

## 二、修改清单

### 2.1 服务端编译配置修复

| # | 文件 | 修改内容 | 原因 |
|---|------|---------|------|
| 1 | `Source Server/GameServer/GameServer/stdafx.h:28-30` | `GAMESERVER_UPDATE` 803→502 | 头文件默认值与 vcxproj 的 Release_EX502 配置不一致，导致使用默认值编译时会启用 S8 路径（DES_XEX3 加密等），客户端无法兼容 |
| 2 | `Source Server/GameServer/GameServer/GameServer.vcxproj:99,120` | 删除 `GAMESERVER_LANGUAGE=0` 覆盖 | LANGUAGE=0 会导致：(1) PROTOCOL_CODE1-4 与客户端不匹配（移动/攻击/位置包无法识别）；(2) 登录包结构不支持 NEW_PROTOCOL_SYSTEM（始终包含旧协议 header）。客户端使用 LANGUAGE=1 的协议常量，服务端必须匹配 |

### 2.2 编译配置修复后验证

修复后的编译配置一致性：
| 宏 | stdafx.h | vcxproj（Release_EX502）| 生效值 |
|---|----------|------------------------|--------|
| GAMESERVER_UPDATE | 502 | 502 | **502** |
| GAMESERVER_LANGUAGE | 1 | (已删除覆盖) | **1** |
| NEW_PROTOCOL_SYSTEM | 1 | (未覆盖) | **1** |
| ENCRYPT_STATE | 0 | (未覆盖) | **0** |
| PROTECT_STATE | 1 | (未覆盖) | **1** |

修复后登录包结构完全匹配：
- 服务端 (LANGUAGE=1, NEW_PROTOCOL_SYSTEM=1, UPDATE=502): `account[10] + password[20] + TickCount[4] + ClientVersion[5] + ClientSerial[16]` = **55 字节**（无 header，无 LanguageCode）
- 客户端 (NEW_PROTOCOL_SYSTEM): `account[10] + password[20] + TickCount[4] + ClientVersion[5] + ClientSerial[16]` = **55 字节**
- PROTOCOL_CODE1-4 匹配: CODE1=0xD4, CODE2=0x11, CODE3=0x15, CODE4=0xDB

### 2.3 加密路径验证

| 配置 | 值 | 说明 |
|------|-----|------|
| GAMESERVER_UPDATE=502 < 701 | SimpleModulus 路径 | 不使用 DES_XEX3 加密 |
| ENCRYPT_STATE=0 | HackCheck XOR 已禁用 | SocketManager 中加密/解密调用被预编译跳过 |
| PROTECT_STATE=1 | HackCheck 初始化但不使用 | 不影响通信 |
| 客户端密钥文件 | `Client/Data/Enc1.dat` ✓, `Dec2.dat` ✓ | 密钥文件存在 |

**结论**：加密路径已对齐，服务端和客户端均使用 SimpleModulus 经典加密算法，无额外加密层干扰。

### 2.4 客户端渲染升级

| # | 文件 | 修改内容 | 效果 |
|---|------|---------|------|
| 1 | `Winmain.cpp:855-856` | 像素格式：16-bit→32-bit 颜色, 16-bit→24-bit 深度 | 消除颜色量化条纹，色彩过渡平滑 |
| 2 | `Winmain.cpp:899-919` | 新增 MSAA 4x 多重采样初始化 | 启用硬件抗锯齿，边缘平滑 |
| 3 | `stdafx.h:75-76` (Client) | 定义 `LDS_ADD_MULTISAMPLEANTIALIASING` | 启用项目中已有的 MSAA 代码 |
| 4 | `ZzzOpenglUtil.h:24` | 添加 `extern BOOL g_bSupportedMSAA` | 声明 MSAA 支持标志 |
| 5 | `ZzzOpenglUtil.cpp:722` | 添加 `BOOL g_bSupportedMSAA = FALSE` | MSAA 支持标志定义 |
| 6 | `Main.vcxproj:586,946` | 添加 ZzzShader 文件引用 | 项目构建包含新文件 |

### 2.5 着色器基础设施（新增）

**新建文件：**
- `Source Client/source/ZzzShader.h` — CShader 类声明，支持着色器编译/链接/参数设置
- `Source Client/source/ZzzShader.cpp` — 着色器实现，包含内置 GLSL v120 着色器

**内置着色器：**
| 着色器 | 类型 | 用途 |
|--------|------|------|
| Terrain | vertex + fragment | 带纹理和光照的地形渲染（varying 传递 UV 和颜色） |
| SimpleTexture | vertex + fragment | 带纹理和颜色调制的 2D 渲染 |

**初始化集成：**
- 在 `Winmain.cpp` OpenGL 初始化成功后调用 `InitBuiltinShaders()`
- 着色器初始化失败时不阻塞，自动回退固定管线

---

## 三、协议匹配度修正

原始分析报告在 GAMESERVER_UPDATE=803（stdafx.h 默认值）下进行了分析，发现有约 20 个服务端发送但客户端无处理的包。但是，修复后以 GAMESERVER_UPDATE=502 编译时，这些操作码几乎全部被条件编译排除：

| 操作码 | 原始报告状态 | 守卫条件 | 502 下实际发送？ |
|--------|-------------|---------|----------------|
| 0x2E | 客户端无处理 | `>=802` | **否** |
| 0x4C | 客户端无处理 | 需验证（采矿系统） | **否**（S8 功能） |
| 0x4D | 客户端无处理 | 需验证（MuRummy） | **否**（S8 功能） |
| 0x4E | 客户端无处理 | 需验证（穆恩系统） | **否**（S8 功能） |
| 0xAB-0xAD | 客户端无处理 | `<=401` / S5 功能 | 需进一步验证 |
| 0xAE | 客户端无处理 | `>=603` | **否** |
| 0xD8 | 客户端无处理 | 需验证 | 需进一步验证 |
| 0xE7 | 客户端无处理 | 需验证（小地图） | 需进一步验证 |
| 0xEC | 客户端无处理 | `>=701` / `>=801` | **否** |
| 0xED | 客户端无处理 | `>=801`（公会匹配） | **否** |
| 0xF2/0xF5 | 客户端无处理 | **内部协议** | 非客户端协议 |

**修正后的匹配度评估：~85%+**（原 71% 分析基于 803 默认值，实际 502 编译下大部分"孤儿包"不会被发送）

---

## 四、已知剩余问题

### P0 - 需尽快确认

| # | 问题 | 说明 |
|---|------|------|
| 1 | PROTOCOL_CODE1-4 值与 client 匹配但仅限 LANGUAGE=1 | 已修复。但如果未来有人使用 stdafx.h 默认 LANGUAGE=1 以外的值编译，移动/攻击/位置包将无法识别 |
| 2 | SimpleModulus 密钥 XOR 码不可验证 | 服务端的 `m_SaveLoadXor[4]`（PacketManager.cpp:34-41）与客户端闭源 `SimpleModulus.lib` 中的 `s_dwSaveLoadXOR[4]` 必须一致。无法直接验证，需要通过实际通信测试 |

### P1 - 功能缺失

| # | 问题 | 影响 |
|---|------|------|
| 3 | 28 个服务端 Custom 功能无客户端 UI | 部分功能（CustomWing, CustomStore, CustomArena 等）可用服务器逻辑运行但无客户端交互界面 |
| 4 | 客户端渲染仍以 OpenGL 1.x 固定管线为主 | 着色器基础设施已准备就绪但还未广泛使用，需后续逐步迁移 |

### P2 - 建议改进

| # | 问题 | 建议 |
|---|------|------|
| 5 | 功能标志命名完全独立 | 建立共享的功能标志文档，记录两端功能标志对应关系 |
| 6 | 无自动化测试 | 任何协议变更都需要两端同步测试 |

---

## 五、后续修复建议

### 近期（下次修复）
1. **Custom 功能客户端实现**：按优先级选择部分 Custom 功能（如 CustomWing、CustomRanking）添加客户端 UI
2. **着色器迁移**：逐步将地形渲染（ZzzLodTerrain.cpp）迁移至 VBO + Shader
3. **建立版本号同步脚本**：确保 `GameServerInfo - 连接信息.dat` 与客户端 `Main.info` 版本号一致

### 中长期
4. 客户端渲染完全迁移至 OpenGL 3.3 Core Profile
5. 服务端单元测试（至少覆盖协议解析）
6. 协议变更流程文档化
7. 评估迁移至 ASIO 新协议系统

---

## 六、编译验证

### 服务端编译
```cmd
msbuild "Source Server\GameServer\GameServer.sln" /p:Configuration=Release_EX502;Platform=Win32
```
预期：编译成功，使用 GAMESERVER_UPDATE=502, LANGUAGE=1 生成 GameServer_EX502.exe

### 客户端编译
```cmd
msbuild "Source Client\Main.sln" /p:Configuration="Global Release";Platform=Win32
```
预期：编译成功，像素格式 32-bit，MSAA 可用时自动启用

---

*本日志由 Claude Code 自动生成*
*生成时间: 2026-05-26*
