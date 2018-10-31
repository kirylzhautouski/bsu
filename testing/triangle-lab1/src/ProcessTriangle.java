import java.util.StringTokenizer;

public class ProcessTriangle {
    public static ProcessTriangleResult processTriangle(String data) {
        // должно быть три числа, влезающие в short (проверить), если треугольник не существует вернуть null, иначе
        // посчитать площадь, периметр и тип треугольника
        StringTokenizer tokenizer = new StringTokenizer(data, ",");

        if (tokenizer.countTokens() != 3)
            return null;

        short firstSide, secondSide, thirdSide;
        try {
            firstSide = Short.parseShort((String)tokenizer.nextElement());
            secondSide = Short.parseShort((String)tokenizer.nextElement());
            thirdSide = Short.parseShort((String)tokenizer.nextElement());
        } catch (NumberFormatException ex) {
            return null;
        }


        Triangle triangle;
        try {
            triangle = new Triangle(firstSide, secondSide, thirdSide);
        } catch (IllegalArgumentException ex) {
            return null;
        }

        return new ProcessTriangleResult(triangle.Square(), triangle.Perimeter(),
            triangle.getTriangleType());
    }
}
