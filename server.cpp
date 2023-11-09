#include "helper.h"
#include <thread.h>

std::atomic<int> n_client;
std::unordered_map<std::string, int> NameToSocket;

void client_handler(int clientSocket);
void game_handler();

int main() {
    struct sockaddr_in srv_addr;
    struct sockaddr_in cln_addr;
    // Initialize WSA
    WSADATA ws;
    if (WSAStartup(MAKEWORD(2, 2), &ws) < 0) {
        handle_eror("Failed to initialize WSA");
    }
    int nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (nSocket < 0) {
        handle_eror("Failed to initialize the socket!");
    }
    print("Successfully created the server socket....");
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(SERVER_PORT);
    srv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    memset(&(srv_addr.sin_zero), 0, 8);
    
    int nRet = bind(nSocket, (sockaddr*)&srv_addr, sizeof(sockaddr));
    if (nRet < 0) {
        handle_eror("Failed to bind to the socket!");
    }
    print("Successfully binded to the socket...");

    nRet = listen(nSocket, MAX_CLIENT);
    if (nRet < 0) {
        handle_eror("Failed to listen to the socket!");
    }
    print("Now the server is running......");

    while (1) {
        int client_addr_size = sizeof(cln_addr);
        // accept will block until a msg is sent
        std::cout << "blocked\n";
        int clientSocket = accept(nSocket, (sockaddr*)&cln_addr, &client_addr_size);
        if (clientSocket == -1) {
            handle_eror("Failed to accept a request from the client side!");
        }
        n_client ++;
        std::thread client_thread(client_handler, clientSocket);
        client_thread.detach();

        if (n_client == 2) {
            print("Now the game starts\n");
            std::thread game_thread(game_handler);
            game_thread.detach();
        }
    }

    return 0;
}

void client_handler(int clientSocket) {
    char msg[BUF_SIZE];
    std::string clientName{};
    int steps = 0;

    while(1) {
        int ret = recv(clientSocket, msg, sizeof(msg), 0);
        if (ret <= 0) {
            std::cout << "User " << clientName << " has left.\n";
            break;
        }
        if (steps == 0) {
            int len = strlen(msg);
            for (int i = 13; i < len; ++ i) {
                clientName += msg[i];
            }
            std::cout << "User " << clientName << " has joined the game.\n";
        }
        ++ steps;
    }
    close(clientSocket);
}

void game_handler() {

}
