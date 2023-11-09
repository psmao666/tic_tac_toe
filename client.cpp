#include "helper.h"

int main() {
    struct sockaddr_in srv_addr;
    // Initialize WSA
    WSADATA ws;
    if (WSAStartup(MAKEWORD(2, 2), &ws) < 0) {
        handle_eror("Failed to initialize WSA");
    }
    int nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (nSocket < 0) {
        handle_eror("Failed to initialize the socket!");
    }
    print("Successfully created the client socket....");
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(SERVER_PORT);
    srv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    memset(&(srv_addr.sin_zero), 0, 8);
    
    int nRet = connect(nSocket, (sockaddr*)&srv_addr, sizeof(srv_addr));
    if (nRet < 0) {
        handle_eror("Failed to connect to the server socket!");
    }
    print("Successfully connected to the server!");
    
    print("Now please enter your nickname in order to join the game :)");
    std::string name;
    std::cin >> name;
    std::string my_name = "#new client: " + name;
    send(nSocket, my_name.c_str(), my_name.length() + 1, 0);
    std::string op;
    while (1) {
        print("Please enter your next move: ");
        std::cin >> op;
        if (op == "exit") break;
    }
    close(nSocket);
    return 0;
}
