# MU Online Private Server (Season 5 / EX502)

基于 MU Online Season 5 (EX502) 的私服项目，包含完整的服务端和客户端源码。

## 项目结构

```
E:\23\
├── Source Server/     # 服务端 C++ 源码 (GameServer, ConnectServer, DataServer, JoinServer)
├── Source Client/     # 客户端 C++ 源码 (DirectX 9 / OpenGL 3 渲染)
├── MuServer/          # 部署的服务端 (二进制 + 配置 + SQL)
├── Client/            # 预构建客户端
└── docs/              # 文档
```

## 修复日志

| 版本 | 说明 | 日期 |
|------|------|------|
| v1 | 初始编译环境搭建 + 基础通信验证 | - |
| v2 | 联调测试 + 协议分析 | - |
| **v3** | **角色名编码 + Shader 绑定 + Custom UI 集成** | **2026-05-26** |
| **v4** | **纹理加载回退 + 自动化测试模式** | **2026-05-26** |
| **v5** | **自动化测试自动登录** | **2026-05-26** |

### v3 修复内容

- **控制台编码**: UTF-8 编码修复角色名乱码
- **Shader 绑定**: 将已有 Terrain/SimpleTexture shader 绑定到渲染管线
- **Custom UI**: 新增排行榜 (CustomRanking) 和事件时间 (CustomEventTime) 界面
- **菜单/快捷键**: 菜单按钮 + R/E 快捷键
- **协议测试**: 独立协议测试工具 `protocol_test.c` 验证完整通信链路

详情见 [fix_log_v3.md](fix_log_v3.md)

### v4 修复内容

- **纹理加载回退**: 图片加载失败时创建品红/黑棋盘格占位纹理，防止客户端崩溃
- **自动化测试模式**: 资源缺失时跳过阻塞弹窗，改为日志记录

### v5 修复内容

- **自动化登录**: AutoTest 模式下连接成功自动填入账号 111/111 登录
- **自动请求角色列表**: 登录成功后自动请求角色列表，实现全流程自动化

## 构建

需要 Visual Studio 2019 (v142 toolset) 和 Windows 10 SDK。

```bash
# GameServer
msbuild "Source Server\GameServer\GameServer.sln" /p:Configuration=Release_EX502;Platform=Win32

# Client
msbuild "Source Client\Main.sln" /p:Configuration="Global Release";Platform=Win32
```
