package Point;

/**
 * 
 * @author hyq
 * @version 1.0,20191104
 */

public class Point {

    private double x;
    private double y;
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
public void getX(){return x}
    /////////////////////////////////////////////////
    public static void main(String[] args) {
        Point myPoint = new Point(10, 0);
        myPoint.showPointInCartesion();
        myPoint.showPointInPolar();
    }

}
