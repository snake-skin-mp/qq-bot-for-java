package snake.mp.util;


public class JavaCallCTools {
	
	static { System.loadLibrary("ctools"); }
	
	public native void init();
	public native String get_reply(String str);
}