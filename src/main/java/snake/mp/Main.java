package snake.mp;

import cc.moecraft.icq.PicqBotX;
import cc.moecraft.icq.PicqConfig;
import snake.mp.Listener.mp;
import snake.mp.Listener.xiaoBing;

public class Main {

    /**
     * 文档
     * https://github.com/HyDevelop/PicqBotX
     */
    public static void main(String[] args) {
        // 创建机器人配置 ( 传入Picq端口 )
        PicqConfig config = new PicqConfig(31092);//31092
        // 创建机器人对象 ( 传入机器人配置对象 )
        PicqBotX bot = new PicqBotX(config);
        // 添加一个机器人账户 ( 传入名字, 酷Q URL, 酷Q端口 )
        bot.addAccount("MyBot", "127.0.0.1", 31091);//31091
//        bot.getEventManager().registerListeners(new mp());
        bot.getEventManager().registerListeners(new xiaoBing());


        // 启动机器人, 不会占用主线程
        bot.startBot();
    }

}
