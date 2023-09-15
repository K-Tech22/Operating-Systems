import java.io.*;
import java.net.*;
import java.util.*;

public class worker implements Runnable {

    private Socket sock;

    public worker(Socket sock) {
        this.sock = sock;
    }

    @Override
    public void run() {
        PrintWriter out = null;
        try {
            out = new PrintWriter(sock.getOutputStream(), true);

            Date date = new Date();

            out.println(date.toString());

            try {
                sock.close();
            } catch (IOException err) {}

        } catch (IOException e) {

            if (sock != null) {
                try {
                    sock.close();
                } catch (IOException er) {}
            }
        }
    }
}