class Greeter {
    static void main(String[] args) {
        println "Hello from the greet() method!"
        Greeter2 g2 = new Greeter2()
        g2.test()
        hallo();
    }

    static void hallo() {
        println "Hallo"
    }
}

class Greeter2 {
    static void test() {
        println "Hello from the greet2() method!"
        hallo2();
    }

    static void hallo2() {
        println "Hallo"
    }
}
