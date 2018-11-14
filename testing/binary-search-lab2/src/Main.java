import java.util.InputMismatchException;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        if (args.length == 1 && args[0].equals("-t")) {
            System.out.println("<---- TESTS STARTED ---->");

            TestRunner testRunner = new TestRunner("tests_input.txt", "tests_output.txt");
            testRunner.runTests();

            System.out.println("/////////////////////////");
        }
        else {
            try (Scanner scanner = new Scanner(System.in)) {
                System.out.print("Enter number of elements: ");
                int elementsNumber = scanner.nextInt();

                if (elementsNumber < 0)
                    throw new NegativeArraySizeException("Number of elements is less than zero");

                int arr[] = new int[elementsNumber];

                for (int i = 0; i < elementsNumber; i++)
                {
                    System.out.print("Enter " + (i + 1) + " element: ");
                    arr[i] = scanner.nextInt();
                }

                scanner.nextLine();
                System.out.print("Enter key: ");
                String keyString = scanner.nextLine();
                int key = Integer.parseInt(keyString);

                System.out.println(BinarySearch.binarySearch(arr, key));
            } catch (InputMismatchException ex) {
                System.out.println("Not a number");
            } catch (Exception ex) {
                System.out.println(ex.getMessage());
            }
        }
    }
}
