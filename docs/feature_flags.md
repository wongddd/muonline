# MU 私服功能标志映射文档

**版本**: v1.0
**日期**: 2026-05-26
**用途**: 记录服务端功能标志与客户端支持状态的对应关系

---

## 编译期宏定义

### 服务端 stdafx.h

| 宏 | 值 | 含义 | 客户端需求 |
|----|-----|------|-----------|
| `GAMESERVER_UPDATE` | 502 | Season 5 Episode 2 | 客户端兼容 |
| `GAMESERVER_LANGUAGE` | 1 | 协议代码 LANGUAGE=1 | 必须匹配 |
| `NEW_PROTOCOL_SYSTEM` | 1 | ASIO 新协议系统 | 客户端已支持 |
| `PROTECT_STATE` | 1 | Premium 模式 | 无直接影响 |
| `ENCRYPT_STATE` | 0 | 加密状态（0=禁用额外加密） | 必须匹配 |
| `PG_CUSTOM` | 1 | 通用自定义系统 | 无直接影响 |
| `POINT_CUSTOM` | 1 | ShopPointEx 积分商店 | 无客户端 UI |
| `BOT_BUFFER` | 1 | 挂机辅助 | 无客户端 UI |
| `PC_POINT` | 2 | PC Point 功能（关闭） | 不适用 |
| `HAPPY_HOURS` | 2 | 欢乐时光（关闭） | 不适用 |
| `MASTER_HOURS` | 2 | 大师时光（关闭） | 不适用 |
| `ACHERON_GUARDIAN` | 2 | 阿卡伦守卫（关闭） | 不适用 |
| `ARCA_WAR` | 2 | 阿尔卡战争（关闭） | 不适用 |
| `GOLDEN_ARCHER` | 2 | 黄金弓箭手（关闭） | 不适用 |

---

## 运行时 Custom 功能

| # | 功能 | 服务端开关 | 配置文件 | 通信协议 | 客户端 UI | 客户端状态 |
|---|------|-----------|---------|---------|----------|-----------|
| 1 | CustomArena | `m_CustomArenaSwitch` | Custom.dat | 无专用协议 | 无 | ❌ 缺失 |
| 2 | CustomAttack | 自读配置 | Custom.dat | DS 协议（持久化） | 无 | ❌ 缺失 |
| 3 | CustomBuyVip | 始终开启 | Custom.dat | BUYPREMIUM_REQ | 无 | ❌ 缺失 |
| 4 | CustomCombo | 始终开启 | 数据文件 | 无 | 无（服务器逻辑） | ✅ 不需要 |
| 5 | CustomCommandDescription | 始终开启 | 数据文件 | 无（聊天命令） | 无（聊天命令） | ✅ 不需要 |
| 6 | CustomDeathMessage | `m_CustomDeathMessageSwitch` | ServerInfo | 无（聊天消息） | 无（聊天消息） | ✅ 不需要 |
| 7 | CustomEventDrop | `m_CustomEventDropSwitch` | Custom.dat | 无 | 无 | ❌ 缺失公告显示 |
| 8 | CustomEventTime | `m_CustomEventTimeSwitch` | ServerInfo | PMSG_CUSTOM_EVENTTIME | 无 | ❌ 缺失 |
| 9 | CustomExchangeCoin | 始终开启 | 数据文件 | 无（NPC 对话） | 通过 NPC 交互 | ✅ 不需要 |
| 10 | CustomJewel | 始终开启 | 数据文件 | 无（物品系统） | 物品渲染需要支持 | ⚠️ 视物品而定 |
| 11 | CustomMix | 始终开启 | 数据文件 | 无（Chaos Machine） | 使用现有混沌合成 UI | ✅ 不需要 |
| 12 | CustomMonster | 始终开启 | 数据文件 | 无 | 使用现有怪物渲染 | ✅ 不需要 |
| 13 | CustomMove | 始终开启 | 数据文件 | 无（聊天命令/NPC） | 无（聊天命令） | ✅ 不需要 |
| 14 | CustomNpcCollector | `m_CustomNpcCollectorSwitch` | ServerInfo | 无（NPC 对话） | 使用 NPC 对话系统 | ✅ 不需要 |
| 15 | CustomNpcCommand | 始终开启 | 数据文件 | 无（NPC 对话） | 使用 NPC 对话系统 | ✅ 不需要 |
| 16 | CustomNpcMove | 始终开启 | 数据文件 | 无（NPC 对话） | 使用 NPC 对话系统 | ✅ 不需要 |
| 17 | CustomNpcQuest | `m_CustomNpcQuestSwitch` | ServerInfo | DS 协议（持久化） | NPC 对话系统 | ⚠️ 部分需要 |
| 18 | CustomOnlineLottery | `m_CustomOnlineLotterySwitch` | Custom.dat | 无 | 无 | ❌ 缺失公告显示 |
| 19 | CustomPick | 已禁用（注释掉） | — | 无 | 无 | 不适用 |
| 20 | CustomQuest | `m_CustomQuestSwitch` | ServerInfo | DS 协议（持久化） | 聊天命令 | ⚠️ 部分需要 |
| 21 | CustomQuiz | `m_CustomQuizSwitch` | ServerInfo | 无（聊天命令） | 聊天命令 | ✅ 不需要 |
| 22 | CustomRanking | `m_CustomRankingSwitch` | ServerInfo | PMSG_CUSTOM_RANKING | 无 | ❌ 缺失 UI |
| 23 | CustomRankUser | `m_CustomRankUserSwitch` | ServerInfo | PMSG_CUSTOM_RANKUSER | 鼠标悬停显示 | ⚠️ 部分需要 |
| 24 | CustomStartItem | 始终开启 | 数据文件 | DS 协议（保存） | 无（创建角色时触发） | ✅ 不需要 |
| 25 | CustomStore | 已禁用（注释掉） | Custom.dat | CGOffTradeRecv | 无离线商店 UI | 不适用 |
| 26 | CustomTop | 已禁用（注释掉） | — | 无 | 无 | 不适用 |
| 27 | CustomWing | 始终开启 | 数据文件 | 无（物品系统） | 需要 3D 模型渲染 | ❌ 缺失模型 |
| 28 | CustomWingMix | 始终开启 | 数据文件 | 无（Chaos Machine） | 使用现有混沌合成 UI | ✅ 不需要 |

### 图例

| 状态 | 含义 |
|------|------|
| ✅ 不需要 | 功能通过聊天命令/NPC 对话/现有 UI 工作，无需额外客户端 UI |
| ❌ 缺失 | 需要客户端 UI 或协议处理但不存在 |
| ⚠️ 部分需要 | 部分功能可用，部分缺失 |
| 不适用 | 功能已在服务端禁用 |

---

## 需要优先实现的客户端功能

按优先级排序：

| 优先级 | 功能 | 原因 |
|--------|------|------|
| P1 | CustomRanking | 服务端发送排行数据，客户端无显示 |
| P1 | CustomEventTime | 服务端发送事件倒计时，客户端无显示 |
| P2 | CustomBuyVip | 玩家无法看到 VIP 购买界面 |
| P2 | CustomArena | 玩家无法看到竞技场界面 |
| P2 | CustomWing | 自定义翅膀物品无客户端模型渲染 |
| P3 | CustomEventDrop | 活动掉落公告在客户端无显示 |
| P3 | CustomOnlineLottery | 在线抽奖公告在客户端无显示 |

---

## 协议操作码对照

### PROTOCOL_CODE（加密头）

| Code | 服务端 (LANGUAGE=1) | 客户端 | 用途 |
|------|--------------------|--------|------|
| CODE1 | 0xD4 | 0xD4 (PACKET_MOVE) | 移动 |
| CODE2 | 0x11 | 0x11 (PACKET_ATTACK) | 攻击 |
| CODE3 | 0x15 | 0x15 (PACKET_POSITION) | 位置 |
| CODE4 | 0xDB | — | 技能/其他 |

### 关键普通操作码

| 方向 | 操作码 | 服务端 | 客户端 | 用途 |
|------|--------|--------|--------|------|
| C→S | 0xF1 | Protocol.cpp | WSclient.cpp | 登录 |
| S→C | 0xF3 | Protocol.cpp | WSclient.cpp | 角色列表 |
| C→S | 0x10 | Protocol.cpp | WSclient.cpp | 角色选择/创建 |
| C→S | 0x11 | Protocol.cpp | WSclient.cpp | 攻击 |
| C→S | 0xD4 | Protocol.cpp | WSclient.cpp | 移动 |
| C→S | 0x15 | Protocol.cpp | WSclient.cpp | 位置同步 |

---

*本文档由 Claude Code 自动生成*
