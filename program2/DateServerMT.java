import java.io.*;
import java.net.*;
import java.util.*;
import java.lang.Thread;

public class DateServerMT {

    public static void main(String[] args) throws IOException {
        
        int port = Integer.parseInt(args[0]);

        try {
            ServerSocket ss = new ServerSocket(port);

            while (true) {
                Socket clientServer = ss.accept();
                Thread thrd = new Thread(new Runnable() {
                    @Override
                    public void run() {
                        try {
                            PrintWriter pout = new PrintWriter(clientServer.getOutputStream(), true);
                            Thread.sleep(5000);
                            pout.println(new java.util.Date().toString());
                            clientServer.close();
                        } catch (InterruptedException e) {} 
                        catch (IOException e) {}
                    }
                });
                thrd.start();
            }
        } catch (IOException iee) {}
    }
}