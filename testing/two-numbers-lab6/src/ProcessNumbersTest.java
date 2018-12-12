import junit.framework.TestCase;
import junit.framework.TestSuite;

public class ProcessNumbersTest extends TestCase {

    public ProcessNumbersTest(String testName) {
        super(testName);
    }

    public static void processNumbers_bothNegative() {
        double x = -5.3, y = -345.1;

        PairOfNumbers actual = ProcessNumbers.processNumbers(x, y);
        PairOfNumbers expected = new PairOfNumbers(5.3, 345.1);

        assertEquals(expected, actual);
    }

    public static void processNumbers_firstIsNegative() {
        double x = -289.32, y = 13.7;

        PairOfNumbers actual = ProcessNumbers.processNumbers(x, y);
        PairOfNumbers expected = new PairOfNumbers(-288.32, 14.7);

        assertEquals(expected, actual);
    }

    public static void processNumbers_secondIsNegative() {
        double x = 33.245, y = -7.1;

        PairOfNumbers actual = ProcessNumbers.processNumbers(x, y);
        PairOfNumbers expected = new PairOfNumbers(34.245, -6.1);

        assertEquals(expected, actual);
    }

    public static void processNumbers_bothPositiveAndDoNotBelongToTheSegment() {
        double x = 48.098, y = 0.5;

        PairOfNumbers actual = ProcessNumbers.processNumbers(x, y);
        PairOfNumbers expected = new PairOfNumbers(43.098, -4.5);

        assertEquals(expected, actual);
    }

    public static void processNumbers_bothPositiveAndBelongToTheSegment() {
        double x = 1.6, y = 1.789;

        PairOfNumbers actual = ProcessNumbers.processNumbers(x, y);
        PairOfNumbers expected = new PairOfNumbers(1.6 * 4, 1.789 * 4);

        assertEquals(expected, actual);
    }

    public static void processNumbers_bothPositiveAndFirstBelongsToTheSegment() {
        double x = 1.5, y = 31.849;

        PairOfNumbers actual = ProcessNumbers.processNumbers(x, y);
        PairOfNumbers expected = new PairOfNumbers(1.5 * 4, 31.849 * 4);

        assertEquals(expected, actual);
    }

    public static void processNumbers_bothPositiveAndSecondBelongsToTheSegment() {
        double x = 12.901, y = 1.6789;

        PairOfNumbers actual = ProcessNumbers.processNumbers(x, y);
        PairOfNumbers expected = new PairOfNumbers(12.901 * 4, 1.6789 * 4);

        assertEquals(expected, actual);
    }

    public static TestSuite suite() {
        TestSuite suite = new TestSuite();

        suite.addTest(new ProcessNumbersTest("processNumbers_bothNegative"));
        suite.addTest(new ProcessNumbersTest("processNumbers_firstIsNegative"));
        suite.addTest(new ProcessNumbersTest("processNumbers_secondIsNegative"));
        suite.addTest(new ProcessNumbersTest("processNumbers_bothPositiveAndDoNotBelongToTheSegment"));
        suite.addTest(new ProcessNumbersTest("processNumbers_bothPositiveAndBelongToTheSegment"));
        suite.addTest(new ProcessNumbersTest("processNumbers_bothPositiveAndFirstBelongsToTheSegment"));
        suite.addTest(new ProcessNumbersTest("processNumbers_bothPositiveAndSecondBelongsToTheSegment"));

        return suite;
    }
}
