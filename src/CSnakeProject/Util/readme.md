## Util 项目

### 编译流程

1. 项目属性中选择：`Release x64`
2. 配置属性——常规——输出目录 修改为 `$(SolutionDir)lib`
4. 配置属性——常规——配置类型 修改为 `静态库(.lib)`
5. 生成事件 —— 命令行 添加 `xcopy /i /r /y $(ProjectDir)*.h  $(SolutionDir)include\`

### 说明

该项目包含了一些常用工具，目前包含字符串编码格式的转换。

编译后将在项目文件夹同级生成 `lib\Util.lib` 和接口文件 `include\String_convert.h`。

如果其他项目想使用，需要包含这个头文件并引用此项目即可。

该项目应该在解决方案中最先生成

