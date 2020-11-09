import java.lang.Math;
import java.io.*;
import java.nio.file.Paths;

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
            System.out.printf("%4.6f    %4.6f\n", x, y);
        }
    }
}
