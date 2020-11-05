
/**
 * This program displays a greeting for the reader.
 * 
 * @version 1.0 2018-09
 * @author He Yongqiang
 */
public class Welcome {
   public static void main(String[] args) {
      String greeting = "Welcome to Core Java!";
      System.out.println(greeting);
      for (int i = 0; i < greeting.length(); i++)
         System.out.print("=");
      System.out.print("\u8930\u88f3\u000d\u000a\u3010\u4f5c\u8005\u3011\u4f5a\u540d\u0020\u3010\u671d\u4ee3\u3011\u5148\u79e6\u000d\u000a\u5b50\u60e0\u601d\u6211\uff0c\u8930\u88f3\u6d89\u6eb1\u3002\u5b50\u4e0d\u6211\u601d\uff0c\u5c82\u65e0\u4ed6\u4eba\uff1f\u72c2\u7ae5\u4e4b\u72c2\u4e5f\u4e14\uff01\u000d\u000a\u5b50\u60e0\u601d\u6211\uff0c\u8930\u88f3\u6d89\u6d27\u3002\u5b50\u4e0d\u6211\u601d\uff0c\u5c82\u65e0\u4ed6\u58eb\uff1f\u72c2\u7ae5\u4e4b\u72c2\u4e5f\u4e14\uff01");
   }
}
