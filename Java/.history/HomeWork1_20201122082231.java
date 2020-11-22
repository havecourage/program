import java.lang.Math;
import java.io.*;

public class HomeWork1 {
    public static void main(String[] args) throws IOException {
        int NUM = 2000;
        double distance = 0, x = 0, y = 0, angle = 0;
        double rou, theta;
        for (int i = 0; i < NUM; i++) {
            rou = Math.random();
            theta = 2 * Math.random() * Math.PI;
            x += rou * Math.cos(theta);
            y += rou * Math.sin(theta);
            distance = Math.sqrt(Math.pow(x, 2) + Math.pow(y, 2));
            angle += theta;
            if (i % 500 == 0) {
                System.out.printf("He is at %4.6f    %4.6f\n", x, y);
                System.out.printf("He has finished %4.6f,theta is %4.6f\n", distance, angle);
                System.out.printf("THETA is %4.6f\n", Math.asin(Math.sin(angle)));
            }
        }
    }
}
