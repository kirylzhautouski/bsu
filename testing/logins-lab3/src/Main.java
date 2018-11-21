import java.util.Scanner;

public class Main {

    public static void main(String[] args) {
        if (args.length == 1 && args[0].equals("-t")) {
            System.out.println("<---- TESTS STARTED ---->");

            TestRunner testRunner = new TestRunner("tests_input.txt", "tests_output.txt");
            testRunner.runTests();

            System.out.println("/////////////////////////");
        } else {
            try (Scanner scanner = new Scanner(System.in)) {
                System.out.print("Enter name of file: ");

                Verification verification = new Verification(scanner.nextLine());

                do {
                    System.out.print("Enter your login: ");
                    String login = scanner.nextLine();

                    System.out.print("Enter your password: ");
                    String password = scanner.nextLine();

                    VerificationInfo verificationInfo = verification.verificate(login, password);
                    System.out.println(verificationInfo.message);

                    if (verificationInfo.result)
                        break;
                } while (true);
            } catch (IllegalArgumentException ex) {
                System.out.println(ex.getMessage());
            }
        }
    }

}
