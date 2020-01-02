package snake.mp.Listener;

import cc.moecraft.icq.event.EventHandler;
import cc.moecraft.icq.event.IcqListener;
import cc.moecraft.icq.event.events.message.EventDiscussMessage;
import cc.moecraft.icq.event.events.message.EventGroupMessage;
import cc.moecraft.icq.event.events.message.EventMessage;
import snake.mp.util.Idioms;
import snake.mp.util.MessageUtil;

public class xiaoBing extends IcqListener {
    public void EventMessage(EventMessage event)
    {
        if (event.getSenderId()==2854196306L)
            if(event.getMessage().contains("【")){
                String message = event.getMessage();
                String s=message.substring(message.indexOf("【")+1,message.indexOf("】"));
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                event.respond(MessageUtil.at(event.getSenderId())+" "+Idioms.get(s));
            }
    }
    @EventHandler
    public void EventGroupMessage(EventGroupMessage event)
    {
        EventMessage(event);
    }
    @EventHandler
    public void EventDiscussMessage(EventDiscussMessage event)
    {
        EventMessage(event);
    }
}
