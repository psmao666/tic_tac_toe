#include "client.h"
#include "board.h"

int main() {
    print("Now please enter your nickname in order to join the game :)");
    std::string name;
    char msg[BUF_SIZE];
    std::string action;
    Board gameBoard;

    std::cin >> name;
    name = "#new client: " + name;
    int nSocket = clientInit();
    send(nSocket, name.c_str(), name.length() + 1, 0);
    print("Please wait for another player to join...");
    
    while (1) {
        if (recv(nSocket, msg, sizeof(msg), 0) < 0) {
            handle_error("Failed to listen to the server.");
        }
        if (msg[0] == '.' || msg[0] == 'X' || msg[0] == 'O') {
            gameBoard = Board (msg);
            gameBoard.printBoard();
            continue;
        }
        if (msg[0] == 'S') {
            print(msg);
            break;
        }
        if (msg[0] == 'Y') {
            print(msg);
            continue;
        }
        if (msg[0] == 'G') {
            print("Game started....");
            continue;
        }
        if (msg[0] == 'P') {
            if (msg[7] == name[0]) {
                print("Congratulations! Winner Winner, Chicken Dinner!");
            } else {
                print("Unlucky, better luck next time!");
            }
            break;
        }
        if (msg[0] == 'I') {
            print(msg);
            print("or type \"quit\" to quit the game.");
            
            int input_x, input_y;
            while (1) {
                std::cin >> action;
                if (action == "quit") {
                    if (send(nSocket, action.c_str(), action.length() + 1, 0) < 0) {
                        handle_error("Failed to send msg to server!");
                    }
                    handle_error("Successfully quit the game!");
                }
                if (action.size() != 3) {
                    print("Invalid input, please try again..");
                    continue;
                }
                input_x = action[0] - '0';
                input_y = action[2] - '0';
                if (input_x > 2 || input_y > 2) {
                    print("Out of range, please try again..");
                    continue;
                }
                if (gameBoard.hasToken(input_x, input_y)) {
                    print("Spot has been taken, please try again...");
                    continue;
                }
                break;
            }
            if (send(nSocket, action.c_str(), action.length() + 1, 0) < 0) {
                handle_error("Failed to send msg to server!");
            }
        }
    }
    close(nSocket);
    return 0;
}

int clientInit() {
    struct sockaddr_in srv_addr;
    // Initialize WSA
    WSADATA ws;
    if (WSAStartup(MAKEWORD(2, 2), &ws) < 0) {
        handle_error("Failed to initialize WSA");
    }
    int nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (nSocket < 0) {
        handle_error("Failed to initialize the socket!");
    }
    print("Successfully created the client socket....");
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(SERVER_PORT);
    srv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    memset(&(srv_addr.sin_zero), 0, 8);
    
    int nRet = connect(nSocket, (sockaddr*)&srv_addr, sizeof(srv_addr));
    if (nRet < 0) {
        handle_error("Failed to connect to the server socket!");
    }
    print("Successfully connected to the server!");
    
    return nSocket;
}