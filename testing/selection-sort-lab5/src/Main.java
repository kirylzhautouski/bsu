import java.util.Arrays;

public class Main {

    public static void main(String[] arg) {
        int arr[] = new int[] { 3, 8,  1, 8, 7, 5, 6 };

        SelectionSort.selectionSort(arr);
        System.out.println(Arrays.toString(arr));
    }
}
