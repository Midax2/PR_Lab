package org.example;

import java.util.Random;
import java.util.Scanner;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

class Warehouse {
    private final int capacity;
    private final int[] stock;

    public Warehouse(int capacity) {
        this.capacity = capacity;
        this.stock = new int[capacity];
    }

    public synchronized boolean produce(int type, int quantity) {
        if (availableSpace() < quantity) {
            return false;
        }
        for (int i = 0; i < capacity; i++) {
            if (stock[i] == 0) {
                stock[i] = type;
                quantity--;
            }
            if (quantity == 0) {
                return true;
            }
        }
        return true;
    }

    public synchronized int consume(int type, int quantity) {
        int count = 0;
        for (int i = 0; i < capacity; i++) {
            if (stock[i] == type) {
                stock[i] = 0;
                count++;
                if (count == quantity) {
                    return count;
                }
            }
        }
        return count;
    }

    private synchronized int availableSpace() {
        int count = 0;
        for (int item : stock) {
            if (item == 0) {
                count++;
            }
        }
        return count;
    }
}

class Producer implements Runnable {
    private final Warehouse warehouse;
    private final Random random = new Random();
    private volatile boolean running;

    public Producer(Warehouse warehouse, boolean running) {
        this.warehouse = warehouse;
        this.running = running;
    }

    @Override
    public void run() {
        while (running) {
            int type = random.nextInt(3) + 1;
            int quantity = random.nextInt(5) + 1;
            if (warehouse.produce(type, quantity)) {
                System.out.println("Produced " + quantity + " of product " + type);
            }
            try {
                Thread.sleep(random.nextInt(1000) + 500);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public void stop() {
        running = false;
    }
}

class Consumer implements Runnable {
    private final Warehouse warehouse;
    private final Random random = new Random();
    private volatile boolean running;

    public Consumer(Warehouse warehouse, boolean running) {
        this.warehouse = warehouse;
        this.running = running;
    }

    @Override
    public void run() {
        while (running) {
            int type = random.nextInt(3) + 1;
            int quantity = random.nextInt(3) + 1;
            int consumed = warehouse.consume(type, quantity);
            if (consumed > 0) {
                System.out.println("Consumed " + consumed + " of product " + type);
            }
            try {
                Thread.sleep(random.nextInt(1000) + 500);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public void stop() {
        running = false;
    }
}

public class Task1 {
    private static volatile boolean running = true;

    public static void main(String[] args) {
        Warehouse warehouse = new Warehouse(10);

        ExecutorService producerPool = Executors.newFixedThreadPool(2);
        ExecutorService consumerPool = Executors.newFixedThreadPool(2);

        Producer[] producers = new Producer[2];
        for (int i = 0; i < producers.length; i++) {
            producers[i] = new Producer(warehouse, running);
            producerPool.submit(producers[i]);
        }

        Consumer[] consumers = new Consumer[2];
        for (int i = 0; i < consumers.length; i++) {
            consumers[i] = new Consumer(warehouse, running);
            consumerPool.submit(consumers[i]);
        }

        System.out.println("Press enter to stop the program...");
        Scanner scanner = new Scanner(System.in);
        scanner.nextLine();

        for (Producer producer : producers) {
            producer.stop();
        }
        for (Consumer consumer : consumers) {
            consumer.stop();
        }

        producerPool.shutdown();
        consumerPool.shutdown();

        while (!producerPool.isTerminated() || !consumerPool.isTerminated()) {
            // Wait for threads
        }

        System.out.println("Program has stopped.");
    }
}
