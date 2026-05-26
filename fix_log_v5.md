# v5 修复日志 — CustomArena UI 实现

## 概述

实现 CustomArena 的完整客户端 UI 和服务端协议支持。玩家可以打开竞技场列表（Y 键），查看进行中的竞技场信息（状态、倒计时、玩家人数），点击 Join 按钮加入竞技场。进入竞技场后，服务端会推送积分榜。

## 修改的文件

### 服务端 (3 文件)

| 文件 | 操作 | 说明 |
|------|------|------|
| `Source Server/GameServer/GameServer/CustomArena.h` | 修改 | 添加协议结构体和 S→C 协议方法声明 |
| `Source Server/GameServer/GameServer/CustomArena.cpp` | 修改 | 实现 5 个协议方法 + 状态变更时主动推送 |
| `Source Server/GameServer/GameServer/Protocol.cpp` | 修改 | `case 0xF3:` 子开关中添加 `case 0xE9:`（列表请求）和 `case 0xEC:`（加入请求） |

### 客户端 (7 文件)

| 文件 | 操作 | 说明 |
|------|------|------|
| `Source Client/source/NewUICustomArena.h` | **新建** | UI 类声明、常量、协议结构体 |
| `Source Client/source/NewUICustomArena.cpp` | **新建** | UI 完整实现（渲染、交互、协议收发） |
| `Source Client/source/_enum.h` | 修改 | 添加 `INTERFACE_CUSTOM_ARENA` |
| `Source Client/source/NewUISystem.h` | 修改 | 添加 `CNewUICustomArena*` 成员、getter、宏 |
| `Source Client/source/NewUISystem.cpp` | 修改 | 创建/销毁/显示/隐藏 Arena UI |
| `Source Client/source/NewUIHotKey.cpp` | 修改 | Y 键切换 Arena 界面 |
| `Source Client/source/WSclient.cpp` | 修改 | 添加 `case 0xEE:`（接收列表/状态）和 `case 0xEF:`（接收积分榜） |
| `Source Client/source/Main.vcxproj` | 修改 | 添加 NewUICustomArena.h/.cpp |

## 协议设计

### C→S（客户端请求）

| 操作 | 格式 | 说明 |
|------|------|------|
| 请求列表 | `C1+04+F3+E9` | 通过 `spe.Init(0xC1, 0xF3)` + 0xE9 发送 |
| 请求加入 | `C1+08+F3+EC+arenaIndex(4)` | 通过 `spe.Init(0xC1, 0xF3)` + 0xEC + int 发送 |

### S→C（服务端推送）

| 操作 | 格式 | HeadCode | 说明 |
|------|------|----------|------|
| 发送列表/状态更新 | `C1+size+EE+count(4)+[entries...]` | 0xEE | 列表 + 状态更新复用同一格式 |
| 发送积分榜 | `C1+size+EF+arenaIndex(4)+count(4)+[score_entries...]` | 0xEF | 单独头码区分 |

**设计选择**: S→C 使用原始头码（0xEE/0xEF）而非 0xF3 包裹，以匹配客户端 `TranslateProtocol` 顶层 `case` 分发。

## 客户端 UI 规格

- **窗口**: 400×450，可滚动列表显示 30 个竞技场
- **竞技场条目**: 状态（颜色编码）+ 名称 + 倒计时 + 玩家人数 + Join 按钮
  - Waiting=灰色, Standby=黄色, Active=绿色, Ending=红色
- **积分榜**: 居中覆盖窗口 250×380，显示 Rank/Name/Kill/Death/Score
  - 高亮当前玩家行
- **自动刷新**: 每 5 秒重新请求列表
- **滚动**: ↑/↓ 键滚动

## 构建验证

- 客户端: ✅ `Main.exe` 编译成功（0 错误 0 警告）
- 服务端: ✅ `GameServer.exe` 编译成功（0 错误，4 个预存警告）
