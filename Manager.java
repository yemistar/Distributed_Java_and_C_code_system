
import java.io.IOException; 
import java.net.DatagramPacket; 
import java.net.DatagramSocket; 
import java.net.InetAddress; 
import java.net.SocketException;
import java.net.*;
import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.Base64;
import java.nio.ByteBuffer;
import java.nio.*;
import java.util.concurrent.*;

public class Manager{

     public static void main(String[] args) {

        BlockingQueue<Beacon> blockingQueue = new LinkedBlockingDeque<>();
        BeaconListener beaconLisn = new BeaconListener(blockingQueue);
        AgentMonitor agentMonitor = new AgentMonitor(blockingQueue);

        new Thread(beaconLisn).start();
        new Thread(agentMonitor).start();
         
    }

}
