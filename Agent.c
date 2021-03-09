#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 
#include <errno.h> 
#include <netdb.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <string.h>
#include <pthread.h>
#include <sys/utsname.h>


#define PORT     1234
#define MAXLINE 1024
#define MAX 80 

void checkHostName(int hostname);
void checkHostEntry(struct hostent * hostentry);
void checkIPbuffer(char *IPbuffer);
struct BEACON *  setupBeacon();
void *udpThreadFun(void *vargp);
int receiveFully(int client_socket, char *buffer, int length);
void convertUpperCase(char *buffer, int length); 
int receive_one_byte(int client_socket, char *cur_char);
void printBinaryArray(char *buffer, int length);
int toInteger32(char *bytes);
void GetLocalOS(char OS[16], int *valid);
void GetLocalTime(int *time2, int *valid);
void *tcpThreadFun(void *vargp);




struct BEACON{
    int ID;                     // randomly generated during startup
    int startUpTime;        // the time when the client starts
    int  timeInterval;   // the time period that this beacon will be repeated 
    char  IP[50];               // the IP address of this client
    int  cmdPort;       // the client listens to this port for manager commands
};
int CmdPort;



int main(){

    //getting the beacon
    struct BEACON *beacon =setupBeacon();
    pthread_t thread_id;
    pthread_t thread_id2;
   
        pthread_create(&thread_id2, NULL, tcpThreadFun, beacon);
        pthread_create(&thread_id, NULL, udpThreadFun, beacon);
        
        
       
        pthread_join(thread_id2, NULL);
        pthread_join(thread_id, NULL);
   
   
    pthread_exit(NULL);

   return 0;
}

/**
 * makes sure the hostname is not empty 
*/
void checkHostName(int hostname) 
{ 
    if (hostname == -1) 
    { 
        perror("gethostname"); 
        exit(1); 
    } 
} 
  
/**
 * Returns host information corresponding to host name 
*/
void checkHostEntry(struct hostent * hostentry) 
{ 
    if (hostentry == NULL) 
    { 
        perror("gethostbyname"); 
        exit(1); 
    } 
} 
   
/**
 * Converts space-delimited IPv4 addresses 
 * to dotted-decimal format 
*/
void checkIPbuffer(char *IPbuffer) 
{ 
    if (NULL == IPbuffer) 
    { 
        perror("inet_ntoa"); 
        exit(1); 
    } 
}



/**
 *  Populate the  BEACON Struct
*/
struct BEACON *  setupBeacon(){
    //gen a random id
    srand(time(NULL));   
    int r = rand();  
    
    //get sys time
    time_t seconds;
    seconds = time (NULL);
    int s=seconds;
    
    // set the time interval
    int interval=60;

    // get the IP address
     char hostbuffer[256];

  
    // To retrieve hostname
    int hostname;
    hostname = gethostname(hostbuffer, sizeof(hostbuffer));
    checkHostName(hostname);
  
    // To retrieve host information
    struct hostent *host_entry;
    host_entry = gethostbyname(hostbuffer);
    checkHostEntry(host_entry);
  
    // To convert an Internet network
    // address into ASCII string
    char IPbuffer[sizeof(inet_ntoa(*((struct in_addr*)
                           host_entry->h_addr_list[0])))];

    strcpy(IPbuffer,inet_ntoa(*((struct in_addr*)
                           host_entry->h_addr_list[0])));                       
    
    
    
    struct BEACON *beacon= malloc(sizeof(*beacon));
    
    beacon->ID=r;
    beacon->startUpTime=s;
    beacon->timeInterval=interval;
    strcpy(beacon->IP,IPbuffer);

    CmdPort=1024+(rand()%200);
    beacon->cmdPort=CmdPort;
    return beacon;
}


/**
 *  This is the Thread that sends BEACON
 * with UDP every 60 second

*/
void *udpThreadFun(void *vargp)
{
  struct BEACON *beacon =(struct BEACON *)vargp;
    int sockfd;
    struct sockaddr_in     servaddr;
      
        // Creating socket file descriptor
        if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
            perror("socket creation failed");
            exit(EXIT_FAILURE);
        }
      
        memset(&servaddr, 0, sizeof(servaddr));
          
        // Filling server information
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(PORT);
        servaddr.sin_addr.s_addr = INADDR_ANY;

        //converting the BEACON to a char array to send
        char *fool;
        char b[sizeof(*beacon)];
        memcpy(b, &(*beacon), sizeof(*beacon));
       
          
        int n, len;
        int first=0;

        while(1){
            if(first==0){
                first=1;
            }else{
                 sleep(60);
            }

        sendto(sockfd, b, (sizeof(b)),
            MSG_CONFIRM, (const struct sockaddr *) &servaddr,
                sizeof(servaddr));

       
        printf("beacon message sent.\n");
        }

         free(beacon);
        
              
        close(sockfd);
}

/**
 * Receive the data sent from the client
 * bit by bit
*/
int receiveFully(int client_socket, char *buffer, int length)
{
    char *cur_char = buffer;
    ssize_t bytes_received = 0;
 
    while (bytes_received != length)
    {
        receive_one_byte(client_socket, cur_char);
        cur_char++;
        bytes_received++;
    }


    
    return 1;
}

void convertUpperCase(char *buffer, int length)
{
    int i = 0;
    while (i < length)
    {
        if (buffer[i] >= 'a' && buffer[i] <= 'z')
        {
            buffer[i] = buffer[i] - 'a' + 'A';
        }
        i++;
    }
}


/**
 *  Helper fiunction for 
 *  the reciveFully function
*/
int receive_one_byte(int client_socket, char *cur_char)
{
    ssize_t bytes_received = 0;
    while (bytes_received != 1)
    {
        bytes_received = recv(client_socket, cur_char, 1, 0);
    }
    
    return 1;
}


/**
 * Prints the binary array
*/
void printBinaryArray(char *buffer, int length)
{
    int i=0;
    while (i<length)
    {
        printf("%d ", buffer[i]);
        i++;
    }  
    printf("\n");
}

/**
 * converts the char to int 
*/
int toInteger32(char *bytes)
{
    int tmp = (bytes[0] << 24) +
              (bytes[1] << 16) +
              (bytes[2] << 8) +
              bytes[3];
   
    return tmp;
}

/**
 * gets the localOs of the system runing the code
*/
void GetLocalOS(char OS[16], int *valid){
    #ifdef _WIN32
        strcpy(OS,"Windows");
	    printf("Windows\n");
	#elif __linux__
        strcpy(OS,"Linux");
	    printf("Linux\n");
	#elif __unix__
        strcpy(OS,"unix");
	    printf("Other unix OS\n");
	#else
        strcpy(OS,"Unidentified");
	    printf("Unidentified OS\n");
	#endif
}

/**
 * gets the system time of the system runing the code
*/
void GetLocalTime(int *time2, int *valid){

    //get sys time
    time_t seconds;
    seconds = time (NULL);
    (*time2)=seconds;
}


/**
 *  The TCP thread server that 
 *  gets the command from the client
 *  and call the function, then sends 
 *  the result back to the java client 
*/
void *tcpThreadFun(void *vargp){

    struct BEACON *beacon =(struct BEACON *)vargp;
    ///TCP
    // get a socket descriptor
    int server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    
    printf("CMDPORT %d \n ",CmdPort);
    sin.sin_family = AF_INET; // or AF_INET6 (address family)
    sin.sin_port = htons(CmdPort);
    sin.sin_addr.s_addr= INADDR_ANY;
    
    if (bind(server_socket, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        // Handle the error.
        printf("bind error\n");
        printf("Error code: %d\n", errno);
    }
     char buff[MAX];
     
    listen(server_socket, 5); /* maximum 5 connections will be queued */
    int counter = 0;
     struct sockaddr client_addr;
        unsigned int client_len;
        
        printf("accepting ....\n");
          int client_socket = accept(server_socket, &client_addr, &client_len);
          printf("request %d comes ...\n", counter++);
      
        
        // get the command length
        char packet_length_bytes[4];
  
    
    // How this works is the following

    // 1) the client first send the data size;
    // 2) the server recives it and makes a char array with the recived data size
    // 3) The client sends the data and the server recives it in the buffer
    // 4) The client convert the bytes to a char array
    // 5) Base on the data command the server calls either the GetLocalTime or GetLocalOS
    // 6) Gets the result and send it to the client
    while (1)
    {
        // processing this request
        receiveFully(client_socket, packet_length_bytes, 4);
        
        //printing the bytes
        printBinaryArray(packet_length_bytes, 4);

        //1)
        int packet_length = toInteger32(packet_length_bytes);
        printf("packet_length_bytes = %d", packet_length);

        //2)
        // allocate buffer to receive the data
        char *buffer = (char *)malloc(packet_length);
        receiveFully(client_socket, buffer, packet_length);
        printf("buffer: %s \n",buffer);

        if(strcmp(buffer,"getLocalOS()")==0){
            printf("getLocalOS() is the call \n");

            char os[16];
            int * v;
            GetLocalOS(os,v);
            buffer=0;
            buffer = (char *)malloc(packet_length);
            strcpy(buffer,os);
        }else if(strcmp(buffer,"getLocalTime()")==0){
            printf("getLocalTime() is the call \n");
            int time;
            int * v;
            GetLocalTime(&time,v);
            buffer=0;
            buffer = (char *)malloc(packet_length);
             memcpy(buffer, &time, sizeof(time));
        }


        // 6) 4 bytes first
         send(client_socket, packet_length_bytes, 4, 0); 

         // 6) send back  
         send(client_socket, buffer, packet_length, 0);


        // release buffer    
        free(buffer);
            
    }
    free(beacon);
    
}
