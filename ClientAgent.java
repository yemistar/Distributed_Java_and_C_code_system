
import java.io.IOException; 
import java.net.DatagramPacket; 
import java.net.DatagramSocket; 
import java.net.InetAddress; 
import java.net.SocketException;
import java.net.*;
import java.io.*;
import java.io.*; 
import java.util.*;
import java.nio.charset.StandardCharsets;
import java.util.Base64;
import java.nio.ByteBuffer;
import java.nio.*;
import java.util.concurrent.*;


public class ClientAgent implements Runnable{

    Beacon beacon;
    ClientAgent(Beacon beacon){
        this.beacon=beacon;
    }


    public void run(){
        //getLocalOS
        //getLocalTime

        String cmd1="getLocalOS()";
        String cmd2="getLocalTime()";
        int test=10;

        try {
            System.out.println("IP: "+beacon.getIp()+" port:"+beacon.getCmdPort());
            Socket socket = new Socket(beacon.getIp(), beacon.getCmdPort());
            DataOutputStream outStream = new DataOutputStream(socket.getOutputStream());
            DataInputStream inStream  = new DataInputStream(socket.getInputStream());

            System.out.println("Sending getLocalOS command");
            sendData(socket,outStream,inStream,cmd1,0);
            System.out.println("Sending getLocalTime command");
            sendData(socket,outStream,inStream,cmd2,1);
             outStream.close();
             inStream.close();
             socket.close();
             System.out.println("message sent");
        } catch (Exception e) {
            System.err.println("Error: "+ e);
        }
    }
    private int byteArrayToLeInt(byte[] b) {
      final ByteBuffer bb = ByteBuffer.wrap(b);
      bb.order(ByteOrder.LITTLE_ENDIAN);
      return bb.getInt();
    }

    private void sendData(Socket socket,DataOutputStream outStream, DataInputStream inStream,String message, int type){

      try{
      
            // prepare a binary buffer       
            byte[] buf = message.getBytes();
       

            // send the length, and then buffer      
            byte[] bufLengthInBinary = toBytes(buf.length);
          
            // send 4 bytes      
            outStream.write(bufLengthInBinary, 0, bufLengthInBinary.length);        
            // send the string
            outStream.write(buf, 0, buf.length);        
            outStream.flush();
            System.out.println("command sent");

          // read the data back 
          inStream.readFully(bufLengthInBinary);  // ignore the first 4 bytes    
          inStream.readFully(buf); 

          if(type ==0){

          // convert the binary bytes to string  
          String ret = new String(buf);
          System.out.println("ClientOS: "+ret);
          System.out.println("Client Startup time: "+beacon.getStartUpTime());
          }else{
            int v= byteArrayToLeInt(buf);
            System.out.println("Client Local time:"+v);
          }
         
          

      }catch (Exception e) {
            //TODO: handle exception
            System.err.println("Error: "+ e);
        }
      

    }

  private void  printBinaryArray(byte[] b, String comment){
    System.out.println(comment);
    for (int i=0; i<b.length; i++){
        System.out.print(b[i] + " ");

    }
    System.out.println();
  }
    private byte[] toBytes(int i){
        byte[] result = new byte[4];
        result[0] = (byte) (i >> 24);
        result[1] = (byte) (i >> 16);        
        result[2] = (byte) (i >> 8);        
        result[3] = (byte) (i /*>> 0*/);
        return result;
    }


}