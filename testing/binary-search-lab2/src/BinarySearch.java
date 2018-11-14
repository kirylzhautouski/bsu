import java.lang.*;
import javafx.util.Pair;

public class BinarySearch {

    private BinarySearch() {

    }

    public static Pair<Boolean, Integer> binarySearch(int arr[], int key) {
        if (arr == null || arr.length < 1)
        {
            throw new IllegalArgumentException("Array is empty");
        }

        if (!isSorted(arr))
        {
            throw new IllegalArgumentException("Array is not sorted");
        }

        int leftBorder = 0, rightBorder = arr.length - 1, i = 0;
        boolean isFound = false;
        while (leftBorder <= rightBorder && !isFound)
        {
            i = (leftBorder + rightBorder) / 2;

            if (arr[i] == key)
                isFound = true;
            else if (arr[i] < key)
                leftBorder = i + 1;
            else if (arr[i] > key)
                rightBorder = i - 1;
        }

        if (!isFound)
            i = -1;

        return new Pair<>(isFound, i + 1);
    }

    private static boolean isSorted(int arr[]) {
        for (int i = 1; i < arr.length; i++) {
            if (arr[i] < arr[i - 1])
                return false;
        }

        return true;
    }
}
