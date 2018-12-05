import java.lang.IllegalArgumentException;

public class SelectionSort {

    private SelectionSort() {

    }

    public static void selectionSort(int arr[]) {
        if (arr == null)
            throw new IllegalArgumentException("argument is null");

        for (int i = 0; i < arr.length - 1; i++) {
            int indexOfMin = findIndexOfMin(arr, i);

            if (indexOfMin != i) {
                swap(arr, indexOfMin, i);
            }
        }
    }

    public static int findIndexOfMin(int arr[], int start) {
        int indexOfMin = start;

        for (int j = start; j < arr.length; j++)
            if (arr[j] < arr[indexOfMin])
                indexOfMin = j;

        return indexOfMin;
    }

    public static void swap(int arr[], int i1, int i2) {
        int tmp = arr[i1];
        arr[i1] = arr[i2];
        arr[i2] = tmp;
    }
}
