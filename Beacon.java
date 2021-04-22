
public class Beacon{

    int Id;
    int startUpTime;
    int timeInterval;
    String Ip;
    int cmdPort;


    Beacon( int Id,int startUpTime,int timeInterval,String Ip,int cmdPort){
        this.Id=Id;
        this.startUpTime=startUpTime;
        this.timeInterval=timeInterval;
        this.Ip=Ip;
        this.cmdPort=cmdPort;
    }

    public int getId(){
        return Id;
    }

    public int getStartUpTime(){
        return startUpTime;
    }

    public int getTimeInterval(){
        return timeInterval;
    }

    public String getIp(){
        return Ip;
    }

    public int getCmdPort(){
        return cmdPort;
    }

}