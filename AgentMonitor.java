

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
public class AgentMonitor implements Runnable{

    HashMap<Integer, Beacon> hashmap = new HashMap<>(); 
    HashMap<Integer, Integer> time = new HashMap<>(); 

    private BlockingQueue<Beacon> beaconQueue;
    AgentMonitor(BlockingQueue<Beacon> beaconQueue){
        this.beaconQueue=beaconQueue;
    }

    //get the becon
    //check if it is new
    //send command to client
    //add becacon th hashmap

    public void run(){

        while(true){
            try {
                while (true) {
                    Beacon beacon =beaconQueue.take();
                    System.out.println("BEACON:{");
                    System.out.println("ID:"+beacon.getId());
                         System.out.println("startUpTime:"+beacon.getStartUpTime());
                         System.out.println("timeInterval:"+beacon.getTimeInterval());
                         System.out.println("Ip:"+beacon.getIp());
                         System.out.println("cmdPort:"+beacon.getCmdPort());
                         System.out.println("}");
                         System.out.println();
                         System.out.println();
                         checkIfBeaconNew(beacon);
                }
              } catch (InterruptedException ex) {
                  System.out.println("Error in AgebtMonitor");
                }
            


        }

    }

    private void checkIfBeaconNew(Beacon beacon){
        if(!hashmap.containsKey(beacon.getId())){
            System.out.println("New Client BEACON");
            hashmap.put(beacon.getId(),beacon);
            time.put(beacon.getId(),beacon.getTimeInterval());
            //lunch new thread
            ClientAgent agent = new ClientAgent(beacon);
            new Thread(agent).start();
        }else{
            //@TODO 
            //1) check if it is an old beacon starting up again
            //2) compute the time diffrence 
            System.out.println("beacon time:"+beacon.getStartUpTime()+
            " System time: "+System.currentTimeMillis()/1000);
            long systime=System.currentTimeMillis()/1000;
            int pass=beacon.getStartUpTime();
            System.out.println("TIME DIFFRENCE:"+(systime-pass));

           
           

        }


    }


}