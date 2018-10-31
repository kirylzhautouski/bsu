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

    public void runTests() {
        int testCount = 1;

        try (FileReader fileReader = new FileReader(inputFile);
             Scanner scanner = new Scanner(fileReader);
             FileWriter fileWriter = new FileWriter(outputFile);
             BufferedWriter bufferedWriter = new BufferedWriter(fileWriter)) {

            scanner.useDelimiter(";");
            while (scanner.hasNextLine()) {
                String input = scanner.next();
                String expectedOutput = scanner.next();
                scanner.skip(";\r\n");

                ProcessTriangleResult processTriangleResult = ProcessTriangle.processTriangle(input);
                checkResult(expectedOutput, processTriangleResult, testCount, bufferedWriter);
                testCount++;
            }
        }
        catch (IOException ex) {
            ex.printStackTrace();
        }
    }

    private static void checkResult(String expectedOutput,
                                    ProcessTriangleResult processTriangleResult, int testNumber,
                                    BufferedWriter bufferedWriter) throws IOException {
        if (expectedOutput.equals("-1")) {
            if (processTriangleResult == null) {
                printSuccess(testNumber, bufferedWriter);
                return;
            }
        } else {
            StringTokenizer tokenizer = new StringTokenizer(expectedOutput, ",");

            double expectedSquare = Double.parseDouble((String)tokenizer.nextElement());
            int expectedPerimeter = Integer.parseInt((String)tokenizer.nextElement());
            String expectedType = (String)tokenizer.nextElement();

            if (processTriangleResult != null && Math.abs(expectedSquare - processTriangleResult.getSquare()) < 0.001 &&
                expectedPerimeter == processTriangleResult.getPerimeter() &&
                expectedType.equals(processTriangleResult.getTriangleType().toString())
            ) {
                printSuccess(testNumber, bufferedWriter);
                return;
            }
        }

        printFail(testNumber, bufferedWriter, expectedOutput, processTriangleResult);
    }

    private static void printSuccess(int number, BufferedWriter bufferedWriter) throws IOException {
        String message = "Test #" + number + " OK\n";

        bufferedWriter.write(message);
        System.out.print(message);
    }

    private static void printFail(int number, BufferedWriter bufferedWriter, String expectedOutput,
                                  ProcessTriangleResult result) throws IOException {
        StringBuilder message = new StringBuilder("Test #");
        message.append(number).append(" FAILED (hint: [").append(expectedOutput).append("] != [");

        if (result == null) {
            message.append("null");
        } else {
            message.append(result.toString());
        }

        message.append("])\n");
        bufferedWriter.write(message.toString());
        System.out.print(message.toString());
    }
}
