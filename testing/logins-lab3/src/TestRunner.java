import java.io.*;
import java.util.Scanner;
import java.util.StringTokenizer;

public class TestRunner {

    public TestRunner(String inputFileName, String outputFileName) {
        if (!(new File(inputFileName)).isFile())
            throw new IllegalArgumentException("file '" + inputFileName +  "' does not exist");

        this.inputFile = inputFileName;
        this.outputFile = outputFileName;
    }

    private String inputFile;
    private String outputFile;
    private int testCount = 1;

    public void runTests() {
        try (FileReader fileReader = new FileReader(inputFile);
             Scanner scanner = new Scanner(fileReader);
             FileWriter fileWriter = new FileWriter(outputFile);
             BufferedWriter bufferedWriter = new BufferedWriter(fileWriter)) {
            while (scanner.hasNextLine()) {
                // filename
                // login1|password1 ... loginn|passwordn
                // message1/ ... /messagem
                // [empty line]
                String expectedOutput = new String();
                StringBuilder input = new StringBuilder();
                StringBuilder output = new StringBuilder();
                try {
                    String filename = scanner.nextLine();
                    String logins = scanner.nextLine();

                    input.append("\n").append(filename).append("\n");
                    input.append(logins);
                    String[] loginsArray = logins.split(" ");
                    expectedOutput = scanner.nextLine();

                    Verification verification = new Verification(filename);

                    output = new StringBuilder();
                    for (int i = 0; i < loginsArray.length; i++) {
                        String[] data = loginsArray[i].split("-");

                        if (data.length == 1)
                            output.append(verification.verificate(data[0], "").message);
                        else
                            output.append(verification.verificate(data[0], data[1]).message);

                        if (i != loginsArray.length - 1)
                            output.append("/");
                    }
                } catch (IllegalArgumentException ex) {
                    output.append(ex.getMessage());
                }

                printResult(bufferedWriter, input.toString(), expectedOutput, output.toString());

                if (scanner.hasNextLine())
                    scanner.nextLine();
            }
        }
        catch (IOException ex) {
            System.out.println(ex.getMessage());
        }
    }

    private <K, V> void printResult(BufferedWriter bufferedWriter, String input, K expectedOutput, V output)
        throws IOException {
        StringBuilder message = new StringBuilder("TEST #");
        message.append(this.testCount++);
        if (expectedOutput.equals(output))
            message.append(" OK:\n");
        else
            message.append(" FAILED:\n");

        message.append("input: ").append(input);
        message.append("\nexpected output: ").append(expectedOutput.toString());
        message.append("\noutput: ").append(output.toString()).append("\n\n");

        System.out.print(message);
        bufferedWriter.write(message.toString());
    }
}