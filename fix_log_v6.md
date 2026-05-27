# v6 修复日志 — 角色选取与协议适配

## 修复项

| # | 问题 | 状态 | 涉及文件 |
|---|------|------|----------|
| 1 | 角色画面点击模型无法选取角色 | ✅ 已修复 | 3 文件 |
| 2 | `SendRequestFinishLoading` 未走现代协议 | ✅ 已修复 | 1 文件 |
| 3 | 函数声明与实现签名不匹配 | ✅ 已修复 | 1 文件 |
| 4 | 断行字符串字面量编译错误 | ✅ 已修复 | 1 文件 |
| 5 | `MoveMainScene` 每帧日志刷屏 + 初始加入未发送 `SendRequestFinishLoading` | ✅ 已修复 | 1 文件 |

---

## 详细修复说明

### 1. 角色选取修复

**症状**: 客户端登录到角色选择画面后，点击角色模型无反应，无法选中角色进入游戏。

**根因分析**:

三个独立问题共同导致：

1. **`SelectCharacter` 声明不匹配** (ZzzInterface.h:105)
   - 头文件声明 `int SelectCharacter();`（无参），但实现为 `int SelectCharacter(BYTE Kind)`
   - 导致调用处 `SelectObjects()` 内部 `SelectCharacter()` 传入的参数被丢弃

2. **`m_bIsSelected` 过滤** (ZzzInterface.cpp:7745)
   - `SelectCharacter()` 中 `if (c->m_bIsSelected == false) continue;` 无条件跳过所有未选中角色
   - 角色场景中没有角色会标记 `m_bIsSelected = true`，导致永远选不中

3. **缺少 3D 点击检测入口** (ZzzScene.cpp:1118-1134)
   - `NewRenderCharacterScene()` 中 `SelectObjects()` 的 3D 射线检测需要 ALT 键，不适用于角色场景
   - 没有直接调用 `SelectCharacter(KIND_PLAYER)` 的代码路径

**修复方案**:

```cpp
// ZzzInterface.h:105 — 修复声明签名
int  SelectCharacter(BYTE Kind);

// ZzzInterface.cpp:7745 — 角色场景中跳过 m_bIsSelected 检查
if ( c->m_bIsSelected==false && Main )
{
    continue;
}
// Main==false 时处于角色场景，不拦截

// ZzzScene.cpp:1118-1134 — 角色场景添加 3D 点击检测
if(!CUIMng::Instance().IsCursorOnUI())
{
    SelectObjects();
    if (SceneFlag == CHARACTER_SCENE && MouseLButtonPush
        && SEASON3B::CheckMouseIn(0, 0, GetScreenWidth(), 429))
    {
        int idx = SelectCharacter(KIND_PLAYER);
        if (idx >= 0 && idx < 5)
        {
            SelectedCharacter = idx;
            SelectedHero = idx;
            CUIMng::Instance().m_CharSelMainWin.UpdateDisplay();
        }
    }
}
```

**关键逻辑**:
- `SelectObjects()` 仍被调用（用于鼠标悬停高亮等）
- `MouseLButtonPush` 时直接调用 `SelectCharacter(KIND_PLAYER)` 检测 OBB 碰撞
- 选中后同时设置 `SelectedCharacter` 和 `SelectedHero`
- 调用 `m_CharSelMainWin.UpdateDisplay()` 刷新 UI（启用 Connect 按钮）

---

### 2. `SendRequestFinishLoading` 现代协议适配

**症状**: 开始游戏后一直卡在 Loading 界面，无法进入游戏。

**根因**: `wsclientinline.h` 中的 `SendRequestFinishLoading` 宏使用 `CStreamPacketEngine`（旧协议类）发送 `0xC1 0xF3 0x12` 包，但连接已升级到现代协议（`olc::net::message` 封装），旧协议的 `spe.Send()` 写入的 socket 无人处理。

**修复方案**:

```cpp
// wsclientinline.h:313-324 — 新协议路径
#ifdef NEW_PROTOCOL_SYSTEM
#define SendRequestFinishLoading()\
{\
    BYTE packet[4];\
    packet[0] = 0xC1;\
    packet[1] = sizeof(packet);\
    packet[2] = 0xF3;\
    packet[3] = 0x12;\
    gProtocolSend.SendPacketClassic(packet, sizeof(packet));\
    g_dwLatestZoneMoving = GetTickCount();\
    g_bWhileMovingZone = FALSE;\
}
#else
// 旧 CStreamPacketEngine 路径保留
...
#endif
```

现代协议路径下，`SendPacketClassic()` 将 C1 包封装为 `BOTH_MESSAGE` 通过 `olc::net::message` 发送。

---

### 3. 断行字符串字面量修复

**症状**: `ZzzScene.cpp` 中 5 处 `g_ErrorReport.Write()` 调用的字符串字面量断行：

```cpp
// 错误写法（加了换行后 \n 不在同一行）
g_ErrorReport.Write("[MoveMainScene] step1: after SendJoin
");

// 修复后
g_ErrorReport.Write("[MoveMainScene] step1: after SendJoin\n");
```

使用 Python 正则批量修复：`re.sub(r'(g_ErrorReport\.Write\(")([^"]*)\n\s*"\s*\)\s*;', r'\1\2\\n");', content)`

---

### 5. MoveMainScene 日志刷屏与初始加入协议修复

**症状**:
- `[MoveMainScene] InitMainScene=1, Name=Wandering Merchant` 每帧输出数百次，日志被刷屏
- 连接在进入游戏后约 30 秒被服务端关闭

**根因**:
1. `g_ErrorReport.Write` 放在 `MoveMainScene()` 函数顶部，每次调用都输出，而 `CharactersClient[0]` 在进入游戏后被 NPC 数据覆盖
2. `SendRequestFinishLoading()` 只在 `InitGame()` 函数末尾调用，但 `InitGame()` 仅在换地图/重连时执行，初始加入流程从未走到该路径

**修复**:
```cpp
// ZzzScene.cpp — 将日志移到 InitMainScene 初始块内部
void MoveMainScene()
{
    if(!InitMainScene)
    {
        g_ErrorReport.Write("[MoveMainScene] InitMainScene=%d, SelectedHero=%d, Name=%s\n", ...);
        // ... init ...
        InitMainScene = true;
    }

    if(CurrentProtocolState == RECEIVE_JOIN_MAP_SERVER)
    {
        EnableMainRender = true;
        SendRequestFinishLoading();  // 通知服务端加载完成
    }
    // ...
}
```

**效果**:
- 日志仅打印一次（初始时），不再刷屏
- `SendRequestFinishLoading()` 在服务端回包后立即发送，避免服务端超时断开

---

## 修改文件清单

| 文件 | 操作 | 说明 |
|------|------|------|
| `Source Client/source/ZzzInterface.h` | 修改 | `SelectCharacter` 声明添加 `BYTE Kind` 参数 |
| `Source Client/source/ZzzInterface.cpp` | 修改 | `m_bIsSelected` 检查添加 `&& Main` 条件 |
| `Source Client/source/ZzzScene.cpp` | 修改 | 角色场景添加 3D 点击选角色 + 修复断行字符串 + 修复日志刷屏 + 初始加入调用 `SendRequestFinishLoading` |
| `Source Client/source/wsclientinline.h` | 修改 | `SendRequestFinishLoading` 宏新协议路径 |

---

## 编译验证

```
msbuild "Source Client\Main.sln" /p:Configuration="Global Release";Platform=x86 /t:Rebuild
```

**结果**: 0 错误 ✅ | 输出 `Main.exe` (6,096 KB)

---

## 累计修复统计

| 版本 | 修复内容 | 修改文件数 |
|------|----------|-----------|
| v1 | 初始修复 | — |
| v2 | 联调测试 | — |
| v3 | 编码 + Shader + Custom UI | 15 |
| v4 | CustomBuyVip UI | 11 |
| v5 | 自动化测试登录 | 4 |
| **v6** | **角色选取 + 协议适配 + Loading 修复** | **4** |
| **合计** | | **34** |
