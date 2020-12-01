
import Point.*;

public class Line {

    public static void getSlope(Point a, Point b) {
        double slope;
        slope = (a.getY() - b.getY()) / (a.getX() - b.getX());
        System.out.println("The slope of this line is " + slope);
    }

    public static void getLength(Point a, Point b) {
        double length;
        length = (a.getX() - b.getX()) * (a.getX() - b.getX()) + (a.getY() - b.getY()) * (a.getY() - b.getY());
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
