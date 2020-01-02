package snake.mp.Listener;

import cc.moecraft.icq.event.EventHandler;
import cc.moecraft.icq.event.IcqListener;
import cc.moecraft.icq.event.events.message.EventGroupMessage;
import snake.mp.util.MessageUtil;

public class mp extends IcqListener
{
    private static long time=0;
    @EventHandler
    public void EventGroupMessage(EventGroupMessage event)
    {
        System.out.println(event.getSenderId());
        System.out.println(event.getMessage());
        if (event.getSenderId()==2667165349L&&time!=0) {
//            if (event.getSenderId()==372091066L&&time!=0) {
            int det= (int) (System.currentTimeMillis()-time)/1000;
            if(det>10)
                event.respond(MessageUtil.at(event.getSenderId()) +" 你距离上次蛇皮已经有"+det+"秒了");
        }
        if (event.getSenderId()==2667165349L)time=System.currentTimeMillis();
        if (event.getSenderId()==2667165349L&&event.getMessage().contains("蛇"))
//            event.respond(MessageUtil.at(event.getSenderId())+"蛇皮");
            event.respond("MP，请你不要蛇皮了");

//        if (event.getSenderId()==2854196306L&&event.getMessage().contains("【"))
//            入室操戈】
//            event.respond(MessageUtil.at(event.getSenderId())+"蛇皮");
//            event.respond("MP，请你不要蛇皮了");
    }
}
