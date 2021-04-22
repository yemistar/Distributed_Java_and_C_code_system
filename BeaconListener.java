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
public class BeaconListener implements Runnable{

    private BlockingQueue<Beacon> beaconQueue;

    BeaconListener(BlockingQueue<Beacon> beaconQueue){

        this.beaconQueue=beaconQueue;
    }


    // setup the UDP
    // parse the info
    public void run(){

         // Step 1 : Create a socket to listen at port 1234
         DatagramSocket ds=null;
         byte[] buffer = new byte[1024];
         DatagramPacket incoming=null;
         try {

             ds = new DatagramSocket(1234);
         } catch (Exception e) {
             //TODO: handle exception
             System.out.println("Error connecting:");
         }

         while (true)
         {
             try {
                 incoming = new DatagramPacket(buffer, buffer.length);
                 ds.receive(incoming);
                 
                 byte[] data = new byte[incoming.getLength()];
                 System.arraycopy(incoming.getData(), 0, data, 0, data.length);
                 Beacon beacon=parseData(data);
                 beaconQueue.put(beacon);
                 
             } catch (Exception e) {
                 //TODO: handle exception
             }
         }


    }


    public static int byteArrayToLeInt(byte[] b) {
        final ByteBuffer bb = ByteBuffer.wrap(b);
        bb.order(ByteOrder.LITTLE_ENDIAN);
        return bb.getInt();
    }

    private Beacon parseData( byte[] data){

         //ID
         byte[] Id = new byte[4];
         System.arraycopy(data, 0, Id, 0, Id.length);
         int ID = byteArrayToLeInt(Id);
      
         //StartUpTime
         byte[] startUpTime = new byte[4];
         System.arraycopy(data, 4, startUpTime, 0, startUpTime.length);
         int STARTUPTIME = byteArrayToLeInt(startUpTime);


         //timeInterval
         byte[] timeInterval = new byte[4];
         System.arraycopy(data, 8, timeInterval, 0, timeInterval.length);
         int TIMEINTERVAL = byteArrayToLeInt(timeInterval);


         //cmdPort
         byte[] cmdPort = new byte[4];
         System.arraycopy(data, 64, cmdPort, 0, cmdPort.length);
         int CMDPORT = byteArrayToLeInt(cmdPort);


         //IP
         byte[] Ip = new byte[50];
         System.arraycopy(data, 12, Ip, 0, Ip.length);
         ByteBuffer bb = ByteBuffer.wrap(Ip);
         bb.order(ByteOrder.LITTLE_ENDIAN);
         StringBuilder IP = new StringBuilder();

         for(int i=0; i<Ip.length; i++){
            IP.append((char) Ip[i]);
        }

      
        //create the beacon

        return new Beacon(ID,STARTUPTIME,TIMEINTERVAL,IP.toString(),CMDPORT);
                
    }

   


}