import javafx.util.Pair;

import java.io.*;
import java.util.Scanner;

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
            // n a1 a2 ... an key 1 isFound index
            // n a1 a2 ... an key 0 message

            while (scanner.hasNextLine()) {
                // read line - run test - compare and show result
                int n = scanner.nextInt();

                int arr[] = new int[n];
                for (int i = 0; i < n; i++)
                    arr[i] = scanner.nextInt();

                int key = scanner.nextInt();
                int flag = scanner.nextInt();

                boolean isFound = false;
                int i = 0;
                String exceptionMessage = "";
                if (flag == 1) {
                    isFound = scanner.nextBoolean();
                    i = scanner.nextInt();
                } else {
                    scanner.skip(" ");
                    exceptionMessage = scanner.nextLine();
                }

                Pair<Boolean, Integer> expectedOutput = new Pair<>(isFound, i);
                Pair<Boolean, Integer> result = new Pair<>(false, 0);
                try {
                    result = BinarySearch.binarySearch(arr, key);

                    if (flag == 1) {
                        if (result.getKey() == expectedOutput.getKey()
                            && result.getValue().equals(expectedOutput.getValue()))
                            printSuccess(bufferedWriter);
                        else
                            printFail(bufferedWriter, expectedOutput, result);
                    } else {
                        printFail(bufferedWriter, exceptionMessage, result);
                    }
                }
                catch (IllegalArgumentException ex) {
                    if (flag == 0) {
                        if (ex.getMessage().equals(exceptionMessage))
                            printSuccess(bufferedWriter);
                        else
                            printFail(bufferedWriter, exceptionMessage, ex.getMessage());
                    }
                    else {
                        printFail(bufferedWriter, result, ex.getMessage());
                    }
                }
            }
        }
        catch (IOException ex) {
            System.out.println(ex.getMessage());
        }
    }

    private void printSuccess(BufferedWriter bufferedWriter) throws IOException {
        String message = "TEST #" + this.testCount++ + " OK\n\n";

        System.out.print(message);
        bufferedWriter.write(message);
    }

    private <K, V> void printFail(BufferedWriter bufferedWriter, K expectedOutput, V output)
        throws IOException {
        StringBuilder message = new StringBuilder("TEST #");
        message.append(this.testCount++).append(" FAILED:\nexpected output: ").append(expectedOutput.toString());
        message.append("\noutput: ").append(output.toString()).append("\n\n");

        System.out.print(message);
        bufferedWriter.write(message.toString());
    }
}
