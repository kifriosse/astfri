public class Cat extends Mammal {
    public Cat(String name) {
        super(name);
    }

    @Override
    public String makeSound() {
        return "Meow!";
    }
}