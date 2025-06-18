# NTP_Client
NTP自动对时器
- **跨平台**支持，支持Linux/Windows
- 自动对时
- Make一键编译

# 下载
`git clone https://gea.yzynetwork.xyz:28445/YZYNetwork/NTP_Client.git`
或从`Release`下载预编译版

# 编译
- Windows
推荐使用Visual Studio生成
- Linux
使用`make`一键编译（编译到./bin目录内）

# 配置文件
- 需自己创建
文件名`config.ini`，应位于可执行文件同目录下
- 内容示例
```ini
; NTP client configuration
[config]
; NTP Server
server = yzynetwork.xyz
; Sync interval (s)
interval = 240
```