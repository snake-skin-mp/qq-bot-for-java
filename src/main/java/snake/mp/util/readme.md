## 文件夹说明

`JavaCallCTools.java` 是 Java 调用 C 动态链接库的接口类

`Main.java` 是动态链接库测试文件，没有其他作用

## java 调用 C 方法

### 编写接口

首先编写 `JavaCallCTools.java`

```java
public class JavaCallCTools {
	
	static { System.loadLibrary("ctools"); }
	
	public native void init();
	public native String get_reply(String str);
}
```

这里声明了两个方法 `void init()` 和 `String get_reply(String)` 但不实现，这两个方法会在动态链接库实现。类初始化时加载动态链接库 `ctools.dll`

### 生成 C 头文件

生成头文件 `javac -h -jni JavaCallCTools.java`

之后会在 `-jni/` 下找到头文件 `JavaCallCTools.h`

### C/C++ 实现接口

实现头文件 `JavaCallCTools.h`，编写 `JavaCallCTools.cpp`，注意头文件路径需要包含 `jni.h`，编译为动态链接库，放入 PATH

### 进行测试

编写测试文件 `Main.java` 如下：

```java
public class Main {

    public static void main(String[] args) {
		JavaCallCTools tool = new JavaCallCTools();
		tool.init();
		String ret = tool.get_reply("send msg");
		System.out.println(ret);
    }
}
```

编译执行 `javac -d . JavaCallCTools.java Main.java`，其中 Main.java 为测试文件

最后执行 `java Main` 测试