# MU Online 私服第三版 (v3) 修复日志

**日期**: 2026-05-26

---

## 概述

v3 修复基于联调测试结果（v2）和 protocol_test 验证，对已知问题进行系统性修复。所有修改均通过编译验证（客户端 0 错误 0 警告）。

---

## 修复清单

### 1. 控制台字符编码（角色名乱码）

**文件**: `Source Server/GameServer/GameServer/WindowsConsole.cpp`

**修改**: 在 `CConsoleWindow::Open()` 中的 `SetConsoleMode` 调用后添加 UTF-8 编码设置（第 99-100 行）：

```cpp
::SetConsoleOutputCP(CP_UTF8);
::SetConsoleCP(CP_UTF8);
```

**效果**: 控制台输出使用 UTF-8 编码，正确显示多字节字符（中文角色名等）。

---

### 2. Shader 绑定（OpenGL 渲染管线）

**文件**:
- `Source Client/source/ZzzShader.cpp`
- `Source Client/source/ZzzLodTerrain.cpp`
- `Source Client/source/ZzzOpenglUtil.cpp`

**修改内容**:
- **ZzzShader.cpp**: Terrain 和 SimpleTexture 两个 shader 的 GLSL v120 源码已正确使用 `gl_Color` 和 `v_Color`（而非 `u_Color` uniform），通过 `ftransform()` 实现固定管线兼容。shader 加载并在 `ZzzShader.cpp:449-472` 完成初始化（`Link()`, `Use()`, `SetUniform1i("u_Texture", 0)`）。
- **ZzzLodTerrain.cpp**: 地形渲染函数添加 `g_pTerrainShader->Use()` / `g_pTerrainShader->Unuse()` 绑定。
- **ZzzOpenglUtil.cpp**: `BeginSprite()`/`EndSprite()` 和 `BeginBitmap()`/`EndBitmap()` 中添加 `g_pSimpleTextureShader->Use()` / `g_pSimpleTextureShader->Unuse()` 绑定。

**效果**: 渲染管线从固定管线升级为可编程管线（GLSL v120），但通过 `gl_Color`/`gl_Vertex`/`gl_MultiTexCoord0` 等 built-in attribute 保持与旧式 `glBegin`/`glEnd` 渲染兼容，视觉上无差异。

---

### 3. Custom UI 功能集成

#### 3a. CustomRanking 排行榜 UI

**新建文件**:
- `Source Client/source/NewUICustomRanking.h` — 声明和协议结构
- `Source Client/source/NewUICustomRanking.cpp` — UI 实现

**功能**:
- 窗口带分类选择（等级排名、PK 排名等分类通过服务端配置）
- 列表显示每个分类下的排名条目（名称、数值）
- 打开时自动请求排名分类列表，选择分类后请求数据
- 左右键关闭支持

**协议**:
- 请求分类数: C1 03 F3 E7
- 响应: C1 + size + F3 + E7 + count(int)
- 请求排名数据: C1 04 F3 E6 + type(byte)
- 响应: C2 + size + F3 + E6 + index + name[20] + col1[20] + col2[20] + count + entries...

#### 3b. CustomEventTime 事件时间 UI

**新建文件**:
- `Source Client/source/NewUICustomEventTime.h` — 声明和协议结构
- `Source Client/source/NewUICustomEventTime.cpp` — UI 实现

**功能**:
- 可滚动列表显示最多 42 个事件
- 事件名称 + 剩余时间 (MM:SS 或 Xh XXm)
- 灰色显示未排期事件（time=-1 或 0）
- 支持上下箭头滚动

**协议**:
- 请求: C1 03 F3 E8
- 响应: C2 + size + F3 + E8 + count(int) + 重复条目 { index(int) + time(int) }

#### 3c. 系统集成

**修改文件**:
| 文件 | 修改内容 |
|------|----------|
| `_enum.h` | 添加 `INTERFACE_CUSTOM_RANKING`、`INTERFACE_CUSTOM_EVENTTIME` |
| `NewUISystem.h` | 添加成员指针、getter、全局宏 |
| `NewUISystem.cpp` | 创建/销毁/OpenningProcess/ClosingProcess 管理 |
| `WSclient.cpp` | 添加 0xF3:0xE6、0xF3:0xE7、0xF3:0xE8 协议处理器 |
| `NewUIWindowMenu.h` | `MENU_MAX_INDEX = 8 → 10` |
| `NewUIWindowMenu.cpp` | 添加菜单按钮 "Custom Ranking" (index 8) 和 "Event Schedule" (index 9) |
| `NewUIHotKey.cpp` | R → CustomRanking, E → CustomEventTime |

---

## 协议验证

使用 `protocol_test.c` 工具进行了完整协议测试（连接 GameServer 127.0.0.1:55901）：

| 步骤 | 协议 | 结果 |
|------|------|------|
| ASIO 握手 | 8-byte challenge/response | PASS |
| SERVER_CONNECT | id=2, index=9000 | PASS |
| 登录 (111/111) | BOTH_CONNECT_LOGIN (id=4), result=0x01 | PASS |
| 角色列表 | BOTH_CONNECT_CHARACTER (id=5), 5 角色 | PASS |
| CustomRanking count | C1 04 F3 E7 | PASS |
| CustomRanking data type=0 | C1 05 F3 E6 00 | PASS |
| CustomRanking data type=1 | C1 05 F3 E6 01 | PASS |
| CustomEventTime | C1 04 F3 E8 | PASS |

关键协议细节:
- ASIO header: 8 bytes (id:uint16_t + padding:2 + size:uint32_t)
- BuxConvert XOR key: {0xFC, 0xCF, 0xAB} 循环
- Classic C1/C2 包通过 `DataSend()` 包装在 BOTH_MESSAGE (id=11) 中发送
- 登录包 55 字节，必须使用 `#pragma pack(1)` 避免 MSVC 对齐填充

---

## 编译验证

- **客户端**: `Main.sln` (Global Release|Win32) — 0 错误, 0 警告
- **GameServer**: 通过 protocol_test 工具验证协议正确性

---

## 推迟到 v4 的功能

- CustomBuyVip UI — 需要服务端新增 VIP 定价协议
- CustomArena UI — 复杂状态机 + 协议
- CustomWing — 需要 3D 模型文件 (.bmd)
- 自动化测试
