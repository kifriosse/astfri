public class Main {
    public static void main(String[] args) {
        Animal dog = new Dog("Buddy");
        Animal cat = new Cat("Whiskers");

        System.out.println(dog.getName() + " says: " + dog.makeSound());
        System.out.println(cat.getName() + " says: " + cat.makeSound());

        Mammal myMammal;

        myMammal = new Dog("Spot");
        System.out.println(myMammal.getName() + " says: " + myMammal.makeSound());

        myMammal = new Cat("Mittens");
        System.out.println(myMammal.getName() + " says: " + myMammal.makeSound());
    }
}