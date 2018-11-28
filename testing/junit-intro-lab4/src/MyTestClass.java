import junit.framework.Test;
import junit.framework.TestCase;
import junit.framework.TestSuite;

public class MyTestClass extends TestCase {

    public MyTestClass(String currentTestMethodName) {
        super(currentTestMethodName);
    }

    @Override
    public void setUp() {
        System.out.println("<---setUp--->");
    }

    @Override
    public void tearDown() {
        System.out.println("<---tearDown--->\n");
    }

    public static void test1() {
        System.out.println("Message from test1");
    }

    public static void test2() {
        System.out.println("test2 is always successful");
        assertTrue("assertTrue with true from test2 (always succeeds)", true);
    }

    public static void test3() {
        System.out.println("test3 is always failing");
        fail();
    }

    public static Test suite() {
        TestSuite suite = new TestSuite();

        suite.addTest(new MyTestClass("test3"));
        suite.addTest(new MyTestClass("test2"));
        suite.addTest(new MyTestClass("test1"));

        return suite;
    }

}
