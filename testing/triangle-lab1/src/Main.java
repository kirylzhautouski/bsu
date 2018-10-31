import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        if (args.length == 1 && args[0].equals("-t"))
        {
            System.out.println("<---- TESTS STARTED ---->");

            TestRunner testRunner = new TestRunner("tests_input.txt", "tests_output.txt");
            testRunner.runTests();

            System.out.println("/////////////////////////");
        }
        else
        {
            System.out.println("Enter triangle sides: ");

            Scanner scanner = new Scanner(System.in);
            String triangle = scanner.nextLine();

            ProcessTriangleResult result = ProcessTriangle.processTriangle(triangle);

            if (result == null)
                System.out.println("-1");
            else
                System.out.println(result);

            scanner.close();
        }
    }
}
