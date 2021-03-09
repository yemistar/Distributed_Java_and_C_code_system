# Distributed_Java_and_C_code_system

This was a fun school project that deals with java and c code communicating 
If you are intrested in trying it yourself, the project description is in 
[ **ProjectDes.pdf** ](https://github.com/yemistar/Distributed_Java_and_C_code_system/blob/main/ProjectDes.pdf).

## Getting Started
Download the project
```bash
git clone https://github.com/yemistar/Distributed_Java_and_C_code_system.git
```

## Runing the code

compiler and run the server first
```bash
javac AgentMonitor.java
javac ClientAgent.java
javac Manger.java
java Manager
```
then compile and run the client
```bash
gcc -o agent Agent.c -lpthread

./agent
```
To see it fully working run mulitlpe **./agent** in new windows

## Chalange yourself
You can try and make your own solution yourself.
Like i said the project description is in the [ **ProjectDes.pdf** ](https://github.com/yemistar/Distributed_Java_and_C_code_system/blob/main/ProjectDes.pdf).

This should be fun, it gives you a bacis understanding on how to commucate with two diffrent language **Java** and **C** 
And if you get stuck, you can checkout my solution. You have my permission :thumbsup: 

This is also a good refrence project on how data between to diffrent language.

## NOTE
The [ **Agent.c** ](https://github.com/yemistar/Distributed_Java_and_C_code_system/blob/main/Agent.c) will not run on macOS
To run it on macOS, you need to change the UDP and TCP connecton setup.
Other than that, everything should work.
If not, you can create an issuse :octocat: to get my attention.

## Licence
 [MIT](https://github.com/yemistar/Distributed_Java_and_C_code_system/blob/main/LICENSE.txt)
