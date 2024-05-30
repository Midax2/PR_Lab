import java.util.HashMap;
import java.util.Map;

public class Buffer<T> {
    private Map<T, Integer> items;
    private int maxSize;

    public Buffer(int maxSize) {
        this.items = new HashMap<>();
        this.maxSize = maxSize;
    }

    public synchronized void put(T item, int quantity) throws InterruptedException {
        while (getTotalSize() + quantity > maxSize) {
            wait();
        }
        items.put(item, items.getOrDefault(item, 0) + quantity);
        notifyAll();
    }

    public synchronized int take(T item, int quantity) throws InterruptedException {
        while (!items.containsKey(item) || items.get(item) == 0) {
            wait();
        }
        int available = items.get(item);
        int taken = Math.min(quantity, available);
        items.put(item, available - taken);
        if (items.get(item) == 0) {
            items.remove(item);
        }
        notifyAll();
        return taken;
    }

    public synchronized int getSize(T item) {
        return items.getOrDefault(item, 0);
    }

    public synchronized int getTotalSize() {
        return items.values().stream().mapToInt(Integer::intValue).sum();
    }

    public synchronized Map<T, Integer> getStatus() {
        return new HashMap<>(items);
    }
}
