import java.util.ArrayList;
import java.util.List;

interface Shape {
    double area();
    double perimeter();
    default String describe() {
        return "Shape with area=" + area();
    }
}

interface Printable {
    void print();
}

abstract class AbstractShape implements Shape, Printable {

    protected String color;
    protected boolean visible;

    public AbstractShape(String color) {
        this.color  = color;                         
        this.visible = true;
    }

    public boolean isVisible() {
        return visible;
    }

    public void toggleVisibility() {
        visible = !visible;
    }

    @Override
    public void print() {
        System.out.println(describe());
    }
}
class Circle extends AbstractShape {

    private double radius;

    public Circle(String color, double radius) {
        super(color);
        this.radius = radius;
    }

    @Override
    public double area() {
        return Math.PI * radius * radius;
    };
    @Override
    public double perimeter() {
        return 2 * Math.PI * radius;
    }

    public double getRadius() {
        return radius;
    }
}

class Rectangle extends AbstractShape {

    private double width;
    private double height;

    public Rectangle(String color, double width, double height) {
        super(color);
        this.width  = width;
        this.height = height;
    }

    @Override
    public double area() {
        return width * height;    
    }

    @Override
    public double perimeter() {
        return 2 * (width + height);
    }
}

class ShapeProcessor {

    public static void processShapes(List<Shape> shapes, String mode) {
        int count = 0;
        double totalArea = 0.0;

        for (Shape s : shapes) {
            count++;
            String label = (s instanceof Circle) ? "circle" : "polygon";

            if (s.area() > 50.0) {
                System.out.println("Large " + label + ": area=" + s.area());
            } else {
                System.out.println("Small " + label);
            }

            totalArea += s.area();
        }

        switch (mode) {
            case "verbose":
                System.out.println("Processed " + count + " shapes, total area=" + totalArea);
                break;
            case "silent":
                break;
            default:
                System.out.println("Unknown mode: " + mode);
        }
    }

    public static double computeScaledArea(Shape shape, int iterations) {

        double result = 0.0;
        int i = 0;

        while (i < iterations / 2) {              
            result += shape.area();
            i++;
        }

        do {
            result -= 0.001;
            i++;
        } while (i < iterations);

        for (int j = 0; j < 3; j++) {
            double factor = (j % 2 == 0) ? 1.0 : -1.0;
            result = result * factor;
        }

        try {
            Circle c = new Circle("red", result);
            result = c.area();
        } catch (IllegalArgumentException e) {
            System.err.println("Invalid shape: " + e.getMessage());
            result = 0.0;
        } catch (Exception e) {
            System.err.println("Unexpected error: " + e.getMessage());
        } finally {
            System.out.println("computeScaledArea finalised.");
        }

        return result;
    }

    public static List<Shape> buildShapes() {
        List<Shape> list = new ArrayList<>();
        list.add(new Circle("blue", 7.0));
        list.add(new Circle("green", 3.5));
        list.add(new Rectangle("red", 10.0, 5.0));
        list.add(new Rectangle("yellow", 4.0, 4.0));

        return list;
    }

    public static String formatSummary(Shape s) {
        double negArea = -s.area();
        String sign = (negArea < 0) ? "positive area" : "non-positive area";
        return s.describe() + " [" + sign + "]";
    }
}

public class ASTDemo {

    public static void main(String[] args) {

        List<Shape> shapes = ShapeProcessor.buildShapes();

        for (Shape s : shapes) {
            System.out.println(ShapeProcessor.formatSummary(s));
        }

        ShapeProcessor.processShapes(shapes, "verbose");
        double scaled = ShapeProcessor.computeScaledArea(shapes.get(0), 10);
        System.out.println("Scaled area result: " + scaled);

        
        AbstractShape r = new Rectangle("purple", 6.0, 3.0);
        r.toggleVisibility();

        
        if (!r.isVisible()) {
            System.out.println("Shape is hidden");
        }
    }
}