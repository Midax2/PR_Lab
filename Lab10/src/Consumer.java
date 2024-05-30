import java.util.Random;

public class Consumer<T> implements Runnable {
    private Buffer<T> buffer;
    private T item;
    private int consumptionTime;
    private Random random;

    public Consumer(Buffer<T> buffer, T item, int consumptionTime) {
        this.buffer = buffer;
        this.item = item;
        this.consumptionTime = consumptionTime;
        this.random = new Random();
    }

    @Override
    public void run() {
        try {
            while (true) {
                Thread.sleep(consumptionTime + random.nextInt(100)); // Randomize consumption time slightly
                int quantity = random.nextInt(10) + 1; // Consume a random quantity between 1 and 10
                int consumedQuantity = buffer.take(item, quantity);
                System.out.println("Consumed: " + consumedQuantity + " of " + item);
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}
