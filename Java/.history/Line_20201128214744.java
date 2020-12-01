
import Point.*;

public class Line {

    public static void getSlope(doublle) {
        double slope;
        slope = (a.y - b.y) / (a.x - b.x);
        System.out.println("The slope of this line is " + slope);
    }

    public static void getLength(Point a, Point b) {
        double length;
        length = (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
        System.out.println("The Length of this line is " + length);
    }

    public Line() {

    }

    public static void main(String[] args) {

        Point a = new Point(10, 0);
        Point b = new Point(1, 9);
        getSlope(a, b);
        getLength(a, b);
    }
}
