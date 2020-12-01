public class Animal {
    private static String name;
    private static String color;
    private static double weight;
    public static int nextid = 1;

    public static void getInfo() {
        System.out.println("Its name is " + name + ", color " + color + ", " + weight + " kg");
    }

    public static void changeColor(String dcolor) {
        color = dcolor;
    }

    public Animal() {

        nextid += 1;
    }

    public Animal(String name) {
        netxid += 1;
    }
}
