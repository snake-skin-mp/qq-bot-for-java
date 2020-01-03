java 调用 C 方法：
	1. 生成头文件 javac -h -jni JavaCallCTools.java
	2. 实现头文件，编译为 dll，放入 PATH
	3. javac -d . JavaCallCTools.java Main.java，其中 Main.java 为测试文件
	4. java Main 测试