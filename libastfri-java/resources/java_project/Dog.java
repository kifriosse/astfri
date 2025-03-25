public class Dog extends Mammal {
    public Dog(String name) {
        super(name);
    }

    @Override
    public String makeSound() {
        return "Woof!";
    }
}