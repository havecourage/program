public class Animal {
	private static String name;
	private static String color;
	private static double weight;
publ
	public static void getInfo() {
		System.out.println("Its name is " + name + ", color " + color + ", " + weight + " kg");
	}

	public static void changeColor(String dcolor) {
		color=dcolor;
	}
}
