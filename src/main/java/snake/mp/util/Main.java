import snake.mp.util.JavaCallCTools;
public class Main {

    /**
     * 文档
     * https://github.com/HyDevelop/PicqBotX
     */
    public static void main(String[] args) {
		JavaCallCTools tool = new JavaCallCTools();
		tool.init();
		String ret = tool.get_reply("send msg");
		System.out.println(ret);
    }

}
