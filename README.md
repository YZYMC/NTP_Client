# NTP_Client
NTP自动对时器
- **跨平台**支持，支持Linux/Windows
- 自动对时
- Make一键编译

项目仓库地址：  
- [GitHub](https://github.com/YZYMC/NTP_Client/)
- [国内镜像](https://gea.yzynetwork.xyz:28445/YZYNetwork/NTP_Client)

# 下载
- 源代码  
`git clone https://github.com/YZYMC/NTP_Client.git`  
或国内镜像：  
`git clone https://gea.yzynetwork.xyz:28445/YZYNetwork/NTP_Client.git`  
- 预编译版  
从`Release`下载预编译版

# 编译
- Windows  
推荐使用Visual Studio生成
- Linux  
使用CMake编译
```bash
cd NTP_Client/
mkdir build
cd build
cmake ..
make
# Run
cd bin
./ntp_client
```
# 配置文件
- Linux在使用CMake编译时自动创建
- Windows使用VS需手动创建，参考下文
文件名`config.ini`，应位于当前目录下  
内容示例  
```ini
; NTP client configuration
[config]
; NTP Server
server = yzynetwork.xyz
; Sync interval (second)
interval = 240
```