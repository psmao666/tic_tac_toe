# tic_tac_toe

## Introduction
This a simple tic_tac_toe game that supports two players (A, B).
The connection will be established via TCP/IP.

## How to set up

My mingw64 compiler doesn't have an appropriate #include<thread> implementation, hence
I installed the relevant head filers and that's why I have #include<thread.h>

Under Windows11 system, compile server code with 
```
g++ -std=c++2a -o server server.cpp -lwsock32 -pthread
```

Similarly, compile client code with
```
g++ -std=c++2a -o client client.cpp -lwsock32 -pthread
```
