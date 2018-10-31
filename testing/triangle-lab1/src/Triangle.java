import java.lang.Math;


public class Triangle {

    public enum TriangleType {
        Versatile,
        Isosceles,
        Equilateral
    }

    public Triangle(short firstSide, short secondSide, short thirdSide) {
        if (firstSide <= 0 || secondSide <= 0 || thirdSide <= 0)
            throw new IllegalArgumentException("all sides must be longer than zero");

        if (firstSide >= secondSide + thirdSide || secondSide >= firstSide + thirdSide
            || thirdSide >= firstSide + secondSide)
            throw new IllegalArgumentException("sides must form a triangle");

        this.firstSide = firstSide;
        this.secondSide = secondSide;
        this.thirdSide = thirdSide;
    }

    private short firstSide;

    private short secondSide;

    private short thirdSide;

    public short getFirstSide() {
        return firstSide;
    }

    public short getSecondSide() {
        return secondSide;
    }

    public short getThirdSide() {
        return thirdSide;
    }

    public double Square() {
        double p = Perimeter() / 2.0;

        return Math.sqrt(p * (p - firstSide) * (p - secondSide) * (p - thirdSide));
    }

    public int Perimeter() {
        return firstSide + secondSide + thirdSide;
    }

    public TriangleType getTriangleType() {
        boolean firstToSecond = firstSide == secondSide;
        boolean firstToThird = firstSide == thirdSide;
        boolean secondToThird = secondSide == thirdSide;

        if (firstToSecond && firstToThird)
            return TriangleType.Equilateral;

        if (firstToSecond || firstToThird || secondToThird)
            return TriangleType.Isosceles;

        return TriangleType.Versatile;
    }

    @Override
    public String toString() {
        return "Triangle{" +
            "firstSide=" + firstSide +
            ", secondSide=" + secondSide +
            ", thirdSide=" + thirdSide +
            '}';
    }
}
