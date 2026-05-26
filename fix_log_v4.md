# v4 修复日志 — CustomBuyVip UI

## 修复项

| # | 问题 | 状态 | 涉及文件 |
|---|------|------|----------|
| 1 | CustomBuyVip 无客户端 UI | ✅ 已实现 | 9 文件 |
| 2 | 服务端缺 VIP 列表发送协议 | ✅ 已添加 | 2 文件 |

---

## CustomBuyVip UI

### 功能描述

VIP 套餐购买界面，展示从服务端获取的 VIP 定价信息（名称、天数、价格），玩家可点击"Buy"按钮直接购买。

### 协议设计

使用子码 **0xEA**（0xF3 下）：

| 方向 | 格式 | 说明 |
|------|------|------|
| 客户端→服务端 | `C1 + size + F3 + EA` | 请求 VIP 列表 |
| 服务端→客户端 | `C1 + size + F3 + EA + count(4) + [CUSTOM_BUYVIP_INFO...]` | 返回 VIP 信息 |
| 客户端→服务端 | `C1 + size + F3 + F0 + VipType(4)` | 购买 VIP（已有协议） |

`CUSTOM_BUYVIP_INFO` 结构（每项 60 字节）：
- `Index`(4) + `Exp`(4) + `Drop`(4) + `Days`(4) + `Coin1`(4) + `Coin2`(4) + `Coin3`(4) + `VipName`(32)

### 客户端 UI 设计

- 窗口尺寸 300×350，标题"VIP Package Purchase"
- 最多显示 3 个 VIP 套餐卡片，每个 80px 高
- 每项显示：VIP 名称（金色）、天数、Coin1/Coin2/Coin3 价格
- 每项右侧有"Buy"按钮，点击触发购买
- 打开窗口时自动请求 VIP 列表
- 'V' 键打开/关闭，Esc 键关闭

### 修改文件清单

#### 服务端（2 文件）

| 文件 | 修改 |
|------|------|
| `Source Server/GameServer/GameServer/CustomBuyVip.h` | 添加 `PMSG_CUSTOM_BUYVIP_INFO_SEND` 结构体、`GCReqBuyVipInfo()` 方法 |
| `Source Server/GameServer/GameServer/CustomBuyVip.cpp` | 实现 `GCReqBuyVipInfo()` — 遍历 `m_CustomBuyVipInfo` 发送 VIP 列表 |
| `Source Server/GameServer/GameServer/Protocol.cpp` | `case 0xF3:` 下添加 `case 0xEA:` → `gCustomBuyVip.GCReqBuyVipInfo()` |

#### 客户端（9 文件）

| 文件 | 操作 |
|------|------|
| `Source Client/source/NewUICustomBuyVip.h` | **新建** — 类声明、协议结构体 |
| `Source Client/source/NewUICustomBuyVip.cpp` | **新建** — 完整 UI 实现 |
| `Source Client/source/_enum.h` | 添加 `INTERFACE_CUSTOM_BUYVIP` |
| `Source Client/source/NewUISystem.h` | 添加 `m_pNewCustomBuyVip`、`GetUI_NewCustomBuyVip()`、`g_pNewUICustomBuyVip` 宏 |
| `Source Client/source/NewUISystem.cpp` | 由生成脚本添加 BuyVip 生命周期管理 |
| `Source Client/source/WSclient.cpp` | 添加 `case 0xEA:` 接收处理器 → `ReceiveVipInfo()` |
| `Source Client/source/NewUIHotKey.cpp` | 添加 `'V'` 键切换 BuyVip 窗口 |
| `Source Client/source/Main.vcxproj` | 添加 NewUICustomBuyVip.h/.cpp 编译项 |

### 图片资源

| 纹理索引 | 文件名 | 用途 |
|----------|--------|------|
| `IMAGE_CUSTOM_BUYVIP_BACK = 31994` | `Interface\newui_msgbox_back.jpg` | 窗口背景 |
| `IMAGE_CUSTOM_BUYVIP_CLOSE = 31995` | `Interface\newui_btn_close.tga` | 关闭按钮 |
| `IMAGE_CUSTOM_BUYVIP_BTN = 31996` | `Interface\newui_btn_empty_very_small.tga` | Buy 按钮 |

---

## 编译验证

### 客户端
```
msbuild "Source Client\Main.sln" /p:Configuration="Global Release";Platform=x86 /t:Rebuild
```
**结果**：0 错误，0 警告 ✅

### 服务端
```
msbuild "Source Server\GameServer\GameServer.sln" /p:Configuration=Release_EX502;Platform=Win32
```
**结果**：0 错误，24 警告（均为已有） ✅

---

## 累计修复统计（v1~v4）

| 版本 | 修复内容 | 修改文件数 |
|------|----------|-----------|
| v1 | 初始修复 | — |
| v2 | 联调测试 | — |
| v3 | 编码 + Shader + CustomRanking + CustomEventTime | 15 |
| v4 | CustomBuyVip UI | 11 |
| **合计** | | **26** |
