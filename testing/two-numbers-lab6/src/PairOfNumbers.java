import java.util.Objects;

public class PairOfNumbers {

    public double firstNumber;
    public double secondNumber;

    public PairOfNumbers(double a, double b) {
        this.firstNumber = a;
        this.secondNumber = b;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        PairOfNumbers that = (PairOfNumbers) o;

        return Double.compare(that.firstNumber, firstNumber) == 0 &&
            Double.compare(that.secondNumber, secondNumber) == 0;
    }

    @Override
    public int hashCode() {
        return Objects.hash(firstNumber, secondNumber);
    }

    @Override
    public String toString() {
        return "PairOfNumbers{" +
            "firstNumber=" + firstNumber +
            ", secondNumber=" + secondNumber +
            '}';
    }
}
