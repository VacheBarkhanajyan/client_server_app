# client_server_app
Simple example of client-server application with raw sockets

##Quick Start

In machine dedicated to be server compile and run server.c 
```
gcc -o server server.c
sudo ./server
```
In machine dedicated to be client you need to modify client.c source file by replacing src and destination IP addresses with your actual ones then compile and run
```
gcc -o client client.c
sudo ./client
```
