public class ProcessTriangleResult {
    public ProcessTriangleResult(double square, int perimeter, Triangle.TriangleType triangleType) {
        this.square = square;
        this.perimeter = perimeter;
        this.triangleType = triangleType;
    }

    private double square;

    private int perimeter;

    private Triangle.TriangleType triangleType;

    public double getSquare() {
        return square;
    }

    public int getPerimeter() {
        return perimeter;
    }

    public Triangle.TriangleType getTriangleType() {
        return triangleType;
    }

    @Override
    public String toString() {
        return String.format("%.3f", square) + "|" + perimeter + "|" + triangleType;
    }
}
