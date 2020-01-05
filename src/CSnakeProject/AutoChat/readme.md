## AutoChat 项目

### 编译流程

1. 项目属性中选择：`Release x64`
2. 配置属性——常规——输出目录 修改为 `$(SolutionDir)lib`
4. 配置属性——常规——配置类型 修改为 `静态库(.lib)`
6. 生成事件 —— 命令行 添加 `xcopy /i /r /y $(ProjectDir)*.h  $(SolutionDir)include\`

### 说明

该项目包含了自动聊天项目，主要是搜索以前的聊天记录并回复最匹配的 5 条中的随机下一句

编译后将在项目文件夹同级生成 `lib\AutoChat.lib` 和接口文件 `include\AutoChat.h` 

如果其他项目想使用，需要包含头文件并引用此项目即可。

该项目依赖于 `Util` 和 `SearchEngine` 项目，应该在其后生成。



