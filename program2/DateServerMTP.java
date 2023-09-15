import java.io.*;
import java.net.*;
import java.util.*;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class DateServerMTP {
    public static void main(String[] args) throws IOException {
        
        int port = Integer.parseInt(args[0]);

        try {
            ServerSocket sock = new ServerSocket(port);
            int numThreads = 4; // You can adjust the number of threads according to your needs.
            ExecutorService executor = Executors.newFixedThreadPool(numThreads);

            while (true) {
                Socket client = sock.accept();
                executor.submit(new Runnable() {
                    @Override
                    public void run() {
                        try {
                            PrintWriter pout = new PrintWriter(client.getOutputStream(), true);
                            Thread.sleep(5000);
                            pout.println(new java.util.Date().toString());
                            client.close();
                        } catch (IOException | InterruptedException e) {}
                    }
                });
            }
        } catch (IOException ioe) {}
    }
}