import static java.lang.Math.abs;

public class ProcessNumbers {

    private ProcessNumbers() {

    }

    public static PairOfNumbers processNumbers(double x, double y) {
        if (x < 0 && y < 0) {
            return new PairOfNumbers(abs(x), abs(y));
        }

        if (x < 0 || y < 0) {
            return new PairOfNumbers(x + 1, y + 1);
        }

        if ((x < 1.5 || x > 2) && (y < 1.5 || y > 2)) {
            return new PairOfNumbers(x - 5, y - 5);
        }

        return new PairOfNumbers(x * 4, y * 4);
    }
}
