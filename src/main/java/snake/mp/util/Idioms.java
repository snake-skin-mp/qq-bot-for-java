package snake.mp.util;

import org.apache.commons.io.FileUtils;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Random;

/**
 * 成语接龙
 */
public class Idioms {
    //2014,"大动干戈","dà  dòng  gān  gē","大规模地进行战争。比喻大张声势地行事。",,"同学们应友好相处，不能为一些小事情而～。","ddgg"
    static HashMap<String, List<String>> idiomsMap =new HashMap<>();
    static HashMap<Character,String> spellMap =new HashMap<>();
    static  {
        try {
            for (String e : FileUtils.readLines(new File("chinese-idioms-12976.txt"), "utf-8")) {
                String name = e.split(",")[1].replace("\"","");
                String sound = e.split(",")[2].replace("\"","");
                if(name.length()==4) {
                    sound=re(sound);
    //                String s = name +" , "+ sound;
    //                System.out.println(s);
                    String[] words = sound.trim().split("\\s+");
                    for (int i = 0; i < name.length(); i++) {
                        spellMap.put(name.charAt(i),words[i]);
                    }
                    idiomsMap.computeIfAbsent(words[0],es->new ArrayList<>()).add(name);
    //                System.out.println(words[0]+","+name);
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static Random random = new Random();
    public static String get(String Idiom){
        char c = Idiom.charAt(Idiom.length()-1);
        List<String> list = idiomsMap.get(spellMap.get(c));
        return list.get(random.nextInt(list.size()));
    }

    private static String re(String s){
        String[] front="ā á ǎ à|ō ó ǒ ò|ê ē é ě è|ī í ǐ ì|ū ú ǔ ù|ǖ ǘ ǚ ǜ ü".split("\\|");
        String[] after="a,o,e,i,u,v".split(",");
        for (int i = 0; i < front.length; i++) {
            String ss=front[i];
            for (String sss : ss.split(" ")) {
                s=s.replaceAll(sss,after[i]);
            }
        }
        return s;
    }
}
