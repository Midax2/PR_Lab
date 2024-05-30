import java.io.FileWriter;
import java.io.IOException;
import java.util.Map;
import java.util.Timer;
import java.util.TimerTask;

public class Reporter<T> {
    private Buffer<T> bufferA;
    private Buffer<T> bufferB;
    private int interval;
    private String outputFile;
    private Timer timer;

    public Reporter(Buffer<T> bufferA, Buffer<T> bufferB, int interval, String outputFile) throws IOException {
        this.bufferA = bufferA;
        this.bufferB = bufferB;
        this.interval = interval;
        this.outputFile = outputFile;
        this.timer = new Timer(true);

        // Clear the file at the start
        try (FileWriter writer = new FileWriter(outputFile)) {
            writer.write(""); // This clears the content of the file
        }
    }

    public void start() {
        timer.scheduleAtFixedRate(new TimerTask() {
            @Override
            public void run() {
                report();
            }
        }, 0, interval);
    }

    public void close() {
        timer.cancel();
        report(); // Final report on close
    }

    private void report() {
        Map<T, Integer> statusA = bufferA.getStatus();
        Map<T, Integer> statusB = bufferB.getStatus();

        System.out.println("Buffer A: " + statusA);
        System.out.println("Buffer B: " + statusB);

        try (FileWriter writer = new FileWriter(outputFile, true)) {
            writer.append("Buffer A: ").append(statusA.toString()).append("\n");
            writer.append("Buffer B: ").append(statusB.toString()).append("\n");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
