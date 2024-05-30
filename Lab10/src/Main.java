import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) throws IOException {
        // Configuration
        int numProducersX = 3;
        int numConsumersY = 3;
        int numProducersY = 3;
        int numConsumersZ = 3;
        int bufferSizeA = 10;
        int bufferSizeB = 10;
        int productionTime = 1000;
        int consumptionTime = 1000;
        int reportInterval = 5000;
        String outputFile = "buffer_status.csv";

        Buffer<String> bufferA = new Buffer<>(bufferSizeA);
        Buffer<String> bufferB = new Buffer<>(bufferSizeB);

        List<Thread> threads = new ArrayList<>();

        // Producers X -> Buffer A
        for (int i = 0; i < numProducersX; i++) {
            Thread t = new Thread(new Producer<>(bufferA, "A" + i, productionTime));
            threads.add(t);
            t.start();
        }

        // Consumers Y -> Buffer A, Producers Y -> Buffer B
        for (int i = 0; i < numConsumersY; i++) {
            Thread t1 = new Thread(new Consumer<>(bufferA, "A" + i, consumptionTime));
            threads.add(t1);
            t1.start();

            Thread t2 = new Thread(new Producer<>(bufferB, "B" + i, productionTime));
            threads.add(t2);
            t2.start();
        }

        // Consumers Z -> Buffer B
        for (int i = 0; i < numConsumersZ; i++) {
            Thread t = new Thread(new Consumer<>(bufferB, "B" + i, consumptionTime));
            threads.add(t);
            t.start();
        }

        // Start reporter
        Reporter<String> reporter = new Reporter<>(bufferA, bufferB, reportInterval, outputFile);
        reporter.start();

        // Shutdown hook for graceful shutdown
        Runtime.getRuntime().addShutdownHook(new Thread(() -> {
            reporter.close();
            threads.forEach(Thread::interrupt);
            System.out.println("Application terminated.");
        }));

        // Wait for Enter key to stop the application
        System.out.println("Press Enter to stop the application...");
        try (Scanner scanner = new Scanner(System.in)) {
            scanner.nextLine();
        }

        // Interrupt all threads to stop them
        threads.forEach(Thread::interrupt);
        reporter.close();
        System.out.println("Application terminated.");
    }
}
