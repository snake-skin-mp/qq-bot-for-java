import snake.mp.util.JavaCallCTools;
public class Main {

    public static void main(String[] args) {
		JavaCallCTools tool = new JavaCallCTools();
		tool.init();
		String ret = tool.get_reply("send msg");
		System.out.println(ret);
    }

}
