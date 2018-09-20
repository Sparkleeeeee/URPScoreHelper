# URPScoreHelper（扬州大学掌上教务查分系统）
Mobile URP system implemented by C++ procedural code

是由扬州高校在校学生自行开发，由 N加青年在扬州范围内发行的掌上 URP 代理程序。

代码由过程化C++实现。因为是无聊的时候随便瞎写的，作者自身开发能力欠缺薄弱，许多地
方经不起推敲，代码重用率低，没有面向对象可能是程序的短板之一。

作者不经意间已经是大三菜鸡老腊肉(2018.09)，这种 suck code 也已无力维护。所以将其开源，望
有条件的同学可以加以利用和改进。

## 支持功能

* 一剑🗡评教
* 本学期成绩
* 其他成绩(月考，期中考，补考，缓考，清考，各种疑难杂考)
* 自动计算GPA / 加权平均分
* 学籍基本信息
* 自动电子注册
* 课程表
* 密码修改
* 微信 OAuth 登录与绑定
* 快速查询(逃)
* 验证码识别(Win)
* 密码找回(deprecated)
* 稳定性远端监控
* 后台管理系统

## 代码槽点
* C++ 鬼畜写 动态 Web
* 原始社会的 MVC 思想
* 跨平台，Windows + Linux (Debian 教派)
* 有了它就知道为什么要学 J2EE 和 PHP 了
* 带后台管理平台
* 包含一整套工具平台

依赖库：libcurl, libiconv, libfcgi, libmariadbclient(libmysqld) Linux 可使用 install-dep.sh 直
接获取，Windows 系统需要手动下载依赖库的源码并编译 DLLs。

系统已测试过成功在 Windows(x64), Linux Debian(x64), Linux Debian(arm64), Linux Raspbian
树莓派(armhf) 系统及架构通过编译并运行。

## 源码文件夹布局：
\Assets\：

\Assets\URPOCRServer : 由 Python + Flask 框架实现的验证码识别

\Assets\URPSH_portal : 静态 HTML + Javascript 实现的服务器列表前端

\Assets\URPSH_selfsvc：PHP 乱写的自主密码找回，曾活过一段时间

\Assets\URPSH_selfsvc\：PHP 乱写的自主密码找回，曾活过一段时间

\Assets\URPSH_selfsvc\backend: Python 写的 百度 TTS 语言合成接口的调用

\Assets\ URPSH_speedtest：Python 写的查分系统稳定性远程监控

\Obj\: 编译后生成的目标文件

\URPScoreHelper\: 后端程序 C++ 源码文件夹

\Web\: 前端模板文件夹(将本文件夹部署到 WEB 服务器)

\Install-dep.sh：Debian 派系的 Linux 系统在部署前使用的依赖包安装脚本

\Install.txt: Linux 系统下，Web 服务器的配置样例( Nginx 反向代理 + Apache 维护本系
统 FastCGI 进程池)

\Makefile: Linux 系统使用 GNU Make tools 来编译本系统

\URPScoreHelper.sln: Windows 系统能使用打开的 Visual Studio 项目文件。

__要自行编译部署这个系统的话，请一定注意修改 \URPScoreHelper\Encrypt.cpp 中的加密
密钥和初始化向量表。否则不安全。__