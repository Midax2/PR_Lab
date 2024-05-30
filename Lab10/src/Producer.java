import java.util.Random;

public class Producer<T> implements Runnable {
    private Buffer<T> buffer;
    private T item;
    private int productionTime;
    private Random random;

    public Producer(Buffer<T> buffer, T item, int productionTime) {
        this.buffer = buffer;
        this.item = item;
        this.productionTime = productionTime;
        this.random = new Random();
    }

    @Override
    public void run() {
        try {
            while (true) {
                Thread.sleep(productionTime + random.nextInt(100)); // Randomize production time slightly
                int quantity = random.nextInt(10) + 1; // Produce a random quantity between 1 and 10
                buffer.put(item, quantity);
                System.out.println("Produced: " + quantity + " of " + item);
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}
