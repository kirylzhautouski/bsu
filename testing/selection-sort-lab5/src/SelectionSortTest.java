import junit.framework.TestCase;
import junit.framework.TestSuite;
import java.util.Arrays;

public class SelectionSortTest extends TestCase {

    public SelectionSortTest(String testName) {
        super(testName);
    }

    public static void findMin_oneElementArray() {
        int arr[] = { 1 };
        int expected = 0;

        int actual = SelectionSort.findIndexOfMin(arr, 0);

        assertEquals(expected, actual);
    }

    public static void findMin_minIsFirst() {
        int arr[] = { 13, 255, 113, 18, 98 };
        int expected = 0;

        int actual = SelectionSort.findIndexOfMin(arr, 0);

        assertEquals(expected, actual);
    }

    public static void findMin_minIsLast() {
        int arr[] = { 98, 255, 113, 18, 13 };
        int expected = 4;

        int actual = SelectionSort.findIndexOfMin(arr, 0);

        assertEquals(expected, actual);
    }

    public static void findMin_minIsNotLastOrFirst() {
        int arr[] = { 98, 255, 3, 18, 13 };
        int expected = 2;

        int actual = SelectionSort.findIndexOfMin(arr, 0);

        assertEquals(expected, actual);
    }

    public static void findMin_startNot0() {
        int arr[] = { 125, 987, 871, 33, 99 };
        int expected = 3;

        int actual = SelectionSort.findIndexOfMin(arr, 2);

        assertEquals(expected, actual);
    }

    public static void findMin_startIsLastElement() {
        int arr[] = { 98, 255, 3, 18, 13 };
        int expected = 4;

        int actual = SelectionSort.findIndexOfMin(arr, 4);

        assertEquals(expected, actual);
    }

    public static void swap_edgeElements() {
        int arr[] = { 56, 16, 89, 13 };
        int expected[] = {13, 16, 89, 56};

        SelectionSort.swap(arr, 0, 3);

        assertTrue(Arrays.equals(arr, expected));
    }

    public static void swap_neighbourElements() {
        int arr[] = { 13, 16, 89, 56 };
        int expected[] = { 13, 89, 16, 56 };

        SelectionSort.swap(arr, 1, 2);

        assertTrue(Arrays.equals(arr, expected));
    }

    public static void swap_equalElements() {
        int arr[] = { 13, 16, 89, 58, 13, 56 };
        int expected[] = {13, 16, 89, 58, 13, 56};

        SelectionSort.swap(arr, 0, 4);

        assertTrue(Arrays.equals(arr, expected));
    }

    public static void swap_itself() {
        int arr[] = { 13, 16, 89, 58, 13, 56 };
        int expected[] = {13, 16, 89, 58, 13, 56};

        SelectionSort.swap(arr, 2, 2);

        assertTrue(Arrays.equals(arr, expected));
    }

    public static void sort_nullArray_throwsIllegalArgumentException() {
        boolean throwsIllegalArgumentException = false;

        try {
            SelectionSort.selectionSort(null);
        } catch (IllegalArgumentException ex) {
            throwsIllegalArgumentException = true;
        } catch (Exception ex) {

        }

        assertTrue(throwsIllegalArgumentException);
    }

    public static void sort_sortedArray_getSameArray() {
        int arr[] = { 1, 3, 5, 6, 7, 8, 8 };
        final int expected[] = Arrays.copyOf(arr, arr.length);

        SelectionSort.selectionSort(arr);

        assertTrue(Arrays.equals(arr, expected));
    }

    public static void sort_arrayWithEqualElements_getSameArray() {
        int arr[] = { 255, 255, 255, 255, 255 };
        final int expected[] = Arrays.copyOf(arr, arr.length);

        SelectionSort.selectionSort(arr);

        assertTrue(Arrays.equals(arr, expected));
    }

    public static void sort_emptyArray_getSameEmptyArray() {
        int arr[] = {};
        final int expected[] = {};

        SelectionSort.selectionSort(arr);

        assertTrue(Arrays.equals(arr, expected));
    }

    public static void sort_arrayWithOddNumberOfElements() {
        int arr[] = { 3, 1, 9, 7, 4 };
        final int expected[] = { 1, 3, 4, 7, 9 };

        SelectionSort.selectionSort(arr);

        assertTrue(Arrays.equals(arr, expected));
    }

    public static void sort_arrayWithEvenNumberOfElements() {
        int arr[] = { 255, 147, 34, 567, 987, 889 };
        final int expected[] = { 34, 147, 255, 567, 889, 987 };

        SelectionSort.selectionSort(arr);

        assertTrue(Arrays.equals(arr, expected));
    }

    public static void sort_oneElementArray_getSameArray() {
        int arr[] = { 47 };
        final int expected[] = { 47 };

        SelectionSort.selectionSort(arr);

        assertTrue(Arrays.equals(arr, expected));
    }

    public static void sort_arrayWithDuplicates() {
        int arr[] = { 1, 2, 2, 1, 0, 0, 15, 15 };
        final int expected[] = { 0, 0, 1, 1, 2, 2, 15, 15 };

        SelectionSort.selectionSort(arr);

        assertTrue(Arrays.equals(arr, expected));
    }

    public static void sort_arrayWithPositiveAndNegative() {
        int arr[] = { 25, -3, 17, 18, 0, -55, -1, 3 };
        final int expected[] = { -55, -3, -1, 0, 3, 17, 18, 25 };

        SelectionSort.selectionSort(arr);

        assertTrue(Arrays.equals(arr, expected));
    }

    public static void sort_arrayWithNegativeNumbers() {
        int arr[] = { -25, -3, -550, -18, -515, -113, -123 };
        final int expected[] = { -550, -515, -123, -113, -25, -18, -3 };

        SelectionSort.selectionSort(arr);

        assertTrue(Arrays.equals(arr, expected));
    }

    public static void sort_reverseSortedArray() {
        int arr[] = { 556, 480, 301, 25, 1 };
        final int expected[] = { 1, 25, 301, 480, 556 };

        SelectionSort.selectionSort(arr);

        assertTrue(Arrays.equals(arr, expected));
    }

    public static TestSuite suite() {
        TestSuite suite = new TestSuite();

        suite.addTest(new SelectionSortTest("findMin_oneElementArray"));
        suite.addTest(new SelectionSortTest("findMin_minIsFirst"));
        suite.addTest(new SelectionSortTest("findMin_minIsLast"));
        suite.addTest(new SelectionSortTest("findMin_minIsNotLastOrFirst"));
        suite.addTest(new SelectionSortTest("findMin_startNot0"));
        suite.addTest(new SelectionSortTest("findMin_startIsLastElement"));

        suite.addTest(new SelectionSortTest("swap_edgeElements"));
        suite.addTest(new SelectionSortTest("swap_neighbourElements"));
        suite.addTest(new SelectionSortTest("swap_equalElements"));
        suite.addTest(new SelectionSortTest("swap_itself"));

        suite.addTest(new SelectionSortTest("sort_nullArray_throwsIllegalArgumentException"));
        suite.addTest(new SelectionSortTest("sort_sortedArray_getSameArray"));
        suite.addTest(new SelectionSortTest("sort_arrayWithEqualElements_getSameArray"));
        suite.addTest(new SelectionSortTest("sort_emptyArray_getSameEmptyArray"));
        suite.addTest(new SelectionSortTest("sort_arrayWithOddNumberOfElements"));
        suite.addTest(new SelectionSortTest("sort_arrayWithEvenNumberOfElements"));
        suite.addTest(new SelectionSortTest("sort_oneElementArray_getSameArray"));
        suite.addTest(new SelectionSortTest("sort_arrayWithDuplicates"));
        suite.addTest(new SelectionSortTest("sort_arrayWithPositiveAndNegative"));
        suite.addTest(new SelectionSortTest("sort_arrayWithNegativeNumbers"));
        suite.addTest(new SelectionSortTest("sort_reverseSortedArray"));

        return suite;
    }
}
