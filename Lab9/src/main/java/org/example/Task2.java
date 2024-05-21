package org.example;

import java.util.Map;
import java.util.HashMap;
import java.util.Scanner;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

public class Task2 {

    private static final int THREAD_POOL_SIZE = 4;
    private static ExecutorService executor = Executors.newFixedThreadPool(THREAD_POOL_SIZE);
    private static final Map<Long, Boolean> cache = new HashMap<>();

    private static class CachingPrimeChecker {

        private synchronized static boolean isPrime(long x) {
            if (cache.containsKey(x)) {
                return cache.get(x);
            }

            boolean result = computeIfIsPrime(x);
            cache.put(x, result);
            return result;
        }

        private static boolean computeIfIsPrime(long x) {
            final String currentThreadName = Thread.currentThread().getName();
            System.out.printf("\t[%s] Running computation for: %d%n", currentThreadName, x);
            try {
                TimeUnit.SECONDS.sleep(10);
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }
            if (x < 2) {
                return false;
            }
            for (long i = 2; i * i <= x; i++) {
                if (x % i == 0) {
                    return false;
                }
            }
            return true;
        }
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        while (true) {
            System.out.println("Enter up to 4 numbers to check if they are prime (or 'quit' to exit): ");
            String input = scanner.nextLine();
            if (input.equalsIgnoreCase("quit")) {
                break;
            }

            String[] numbers = input.split(" ");
            for (String number : numbers) {
                try {
                    long x = Long.parseLong(number);
                    executor.submit(() -> {
                        boolean isPrime = CachingPrimeChecker.isPrime(x);
                        System.out.printf("%d is %s prime.%n", x, isPrime ? "a" : "not a");
                    });
                } catch (NumberFormatException e) {
                    System.out.println("Invalid input: " + number);
                }
            }

            executor.shutdown();
            try {
                executor.awaitTermination(Long.MAX_VALUE, TimeUnit.NANOSECONDS);
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }

            executor = Executors.newFixedThreadPool(THREAD_POOL_SIZE);

        }
        scanner.close();
    }
}
