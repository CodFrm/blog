---
title: Windows10%20WSL2 安装 尝鲜
---

> 2019 年 6 月 13 日,windows 发布了 Build 18917 预览版,值得一提的是,Windows Subsystem for Linux 2(WSL2)也可以使用了,在将现在的系统升级到 18917 后(需要参与预览版计划),就迫不及待的想尝试了

![](img/Windows10%20WSL2.assets/a16ed8dbd137815b2577650ac5e8889a.png)

## WSL2 安装开启

安装 wsl2 需要开启`Virtual Machine Platform`

![](img/Windows10%20WSL2.assets/2fd49865293c12aaa0b550b0a35ec143.png)

或者,管理员 PowerShell 运行:
`Enable-WindowsOptionalFeature -Online -FeatureName VirtualMachinePlatform`
然后 PowerShell 管理员运行:
`wsl --set-default-version 2` 设置默认为 wsl2
`wsl --set-version Debian 2`设置某个发行版为 wsl2(竟然可以切换,不错)
(我用的是 CMD 管理员也是可以的,官方 blog 说的是 PowerShell)

![](img/Windows10%20WSL2.assets/89cb491e39c43528434c324ad6f1f372.png)

![](img/Windows10%20WSL2.assets/ecb0ec1c0e6b8ea8ba3b34a23d81f6f4.png)

之后可以使用`wsl --list --verbose`查看使用的 wsl 版本

然后去 Microsoft Store 找 linux 就行了

![](img/Windows10%20WSL2.assets/f8209cd993dd3254af22c3cfa415859c.png)

![](img/Windows10%20WSL2.assets/b54c5fa49baa561e0942c66730c1b577.png)

成了,等下去试试装 docker,然后我就可以抛弃现在的 desktop for docker 了 233,看外表还看不出啥

![](img/Windows10%20WSL2.assets/f1c6ec7ad6baee042655bb366aface0e.png)

### 网络

![](img/Windows10%20WSL2.assets/806f10f788a514951054f68f01af0254.png)

我是这样做的,ifconfig 获取 ip
