public abstract class Mammal implements Animal {
    private String name;

    public Mammal(String name) {
        this.name = name;
    }

    @Override
    public String getName() {
        return name;
    }
}