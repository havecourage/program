
/**
 * 
 * @author hyq
 * @version 1.0,20191104
 */

class Point {

    double x;
    double y;
    private double rho;
    private double theta;

    public Point() {
        x = 0.0;
        y = 0.0;
        rho = 0.0;
        theta = 0.0;
    }

    public Point(double x, double y) {
        this.x = x;
        this.y = y;
        cartesianToPolar();
    }

    private void cartesianToPolar() {
        rho = Math.sqrt(x * x + y * y);
        theta = Math.atan2(y, x);
    }

    public void showPointInCartesion() {
        System.out.println("x=" + x + ", y=" + y);
    }

    public void showPointInPolar() {
        System.out.println("rho=" + rho + ", theta=" + theta);
    }

    /////////////////////////////////////////////////
    public static void main(String[] args) {
        Point myPoint = new Point(10, 0);
        myPoint.showPointInCartesion();
        myPoint.showPointInPolar();
    }

}

public class Line {

    public static double getSlope(Point a, Point b) {
        double slope;
        slope = (a.y - b.y) / (a.x - b.x);
        System.out.println("The slope of this line is " + slope);
        return slope;
    }

    public static double getLength(Point a, Point b) {
        double length;
        length = (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
        System.out.println("The Length of this line is " + length);
        return length;
    }
//构造函数，可以是两个端点，也可以是一个端点（左下角）、
    public static void main(String[] args) {

        Point a = new Point(10, 0);
        Point b = new Point(1, 9);
        getSlope(a, b);
        getLength(a, b);
    }
}
