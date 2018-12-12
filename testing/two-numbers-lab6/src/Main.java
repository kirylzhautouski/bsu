import java.util.Scanner;

public class Main {

    public static void main(String[] args) {
        double x, y;

        try {
            Scanner scanner = new Scanner(System.in);

            System.out.print("Enter x: ");
            x = Double.parseDouble(scanner.nextLine());

            System.out.print("Enter y: ");
            y = Double.parseDouble(scanner.nextLine());

            PairOfNumbers pair = ProcessNumbers.processNumbers(x, y);
            System.out.println("Input: x=" + x + ", y=" + y);
            System.out.println("Output: x=" + pair.firstNumber + ", y=" + pair.secondNumber);
        } catch (NumberFormatException ex) {
            System.out.println("Not a number");
        }
    }
}
