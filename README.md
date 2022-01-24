# myOpenBrowser
myOpenBrowser是基于QT的简易浏览器项目，使用谷歌浏览器内核，具有多标签模式，谷歌、火狐等浏览器的兼容性最好，适用于某些对浏览器兼容性有要求的场合，也可以作为web项目的专用客户端。

# QT版本
5.15.2

# 开发工具
 Qt Creator
 
# 发布
## Windows环境打包
1. 用Release模式运行一遍;
2. 在Build directory目录下，新建dist目录，将release目录下的myOpenBrowser.exe可执行文件复制到dist目录；
3. 打开QT命令行(开始菜单里面/QT下)，进入dist目录；
4. 执行：windeployqt myOpenBrowser.exe
5. 将系统盘windows/system32/下的msvcp140_1.dll和vcruntime140_1.dll复制到dist目录。

# 参考资料
参考了QT的相关示例项目
