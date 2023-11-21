#include "server.h"
#include "board.h"
#include <vector>

int main() {
    int nSocket = server_init();
    std::thread server_thread(server_handler, nSocket);
    server_thread.join();
    close(nSocket);
    return 0;
}

int server_init() {
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
    print("Successfully created the server socket....");
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(SERVER_PORT);
    srv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    memset(&(srv_addr.sin_zero), 0, 8);
    
    int nRet = bind(nSocket, (sockaddr*)&srv_addr, sizeof(sockaddr));
    if (nRet < 0) {
        handle_error("Failed to bind to the socket!");
    }
    print("Successfully binded to the socket...");

    nRet = listen(nSocket, MAX_CLIENT);
    if (nRet < 0) {
        handle_error("Failed to listen to the socket!");
    }
    print("Now the server is running......");
    return nSocket;
}

void client_init(int clientSocket) {
    char msg[BUF_SIZE];
    std::string clientName{};
    int ret = recv(clientSocket, msg, sizeof(msg), 0);
    if (ret <= 0) handle_error("Failed to listen to the client socket.");
    int len = strlen(msg);
    for (int i = 13; i < len; ++ i) {
        clientName += msg[i];
    }
    print("User " + clientName + " has joined the game.");
    std::mutex mtx;
    std::lock_guard<std::mutex> lck{mtx};
    NameToSocket[clientName] = clientSocket;
    SocketToName[clientSocket] = clientName;
    n_client ++;
    
}

void server_handler(int nSocket) {
    struct sockaddr_in cln_addr;
    std::vector<int> cln_sockets;

    while (1) {
        int client_addr_size = sizeof(cln_addr);
        // accept will block the current thread until a msg is sent
        int clientSocket = accept(nSocket, (sockaddr*)&cln_addr, &client_addr_size);
        if (clientSocket == -1) {
            handle_error("Failed to accept a request from the client side!");
        }
        
        cln_sockets.push_back(clientSocket);
        std::thread client_thread(client_init, clientSocket);
        client_thread.join();
        
        if (n_client == 2) {
            print("Now the game starts\n");
            print("Player #1: " + SocketToName[cln_sockets[0]] + " is using token X");
            print("Player #2: " + SocketToName[cln_sockets[1]] + " is using token O");
            std::thread game_thread([&cln_sockets](){
                game_handler(cln_sockets);
            });
            game_thread.join();
            std::cout << "Game is over!" << std::endl;
            break;
        }
    }
    close(nSocket);
}

void game_handler(const std::vector<int>& clientSockets) {
    // generate a plain board first
    Board gameBoard;
    Turn turn_controller = Turn::PlayerATurn;
    msg_broadcast(clientSockets, "Game Started");
    std::string token_msg{"Your token is X"};
    if (send(clientSockets[0], token_msg.c_str(), token_msg.length() + 1, 0) < 0) {
        handle_error("failed to send token_msg to client");
    }
    std::string token_msg2{"Your token is O"};
    if (send(clientSockets[1], token_msg2.c_str(), token_msg2.length() + 1, 0) < 0) {
        handle_error("failed to send token_msg to client");
    }
    
    std::string msg{"It is your turn now! Please enter x,y to indicate where you want to place your token."};
    int n_round = 0;
    char response[BUF_SIZE];

    while (1) {
        
        Result res = gameBoard.gameTerminated();
        ++ n_round;
        print("Round #" + std::to_string(n_round));
        gameBoard.printBoard();
        print("-----------------------------------");
        // broadcast boardinfo to both players
        msg_broadcast(clientSockets, gameBoard.BoardToString());
        
        if (res == Result::PlayerA) {
            std::string winner_msg = "Player " + SocketToName[clientSockets[0]] + " won!";
            print(winner_msg);
            msg_broadcast(clientSockets, winner_msg);
            break;
        } else if (res == Result::PlayerB) {
            std::string winner_msg = "Player " + SocketToName[clientSockets[1]] + " won!";
            print(winner_msg);
            msg_broadcast(clientSockets, winner_msg);
            break;
        }
        if (n_round == 9) {
            print("Tie game.");
            msg_broadcast(clientSockets, "Server closed.");
        }
        if (send(clientSockets[(turn_controller == Turn::PlayerBTurn)], msg.c_str(), msg.length() + 1, 0) < 0) {
            handle_error("Failed to send msg.");
        }
        if (recv(clientSockets[(turn_controller == Turn::PlayerBTurn)], response, sizeof(response), 0) < 0) {
            handle_error("Failed to listen to the client socket.");
        }
        if (response[0] == 'q') {
            print("The game is terminated due to one player has left.");
            break;
        }
        int input_x = response[0] - '0';
        int input_y = response[2] - '0';
        if (gameBoard.placeToken((turn_controller == Turn::PlayerATurn ? Token::X : Token::O), input_x, input_y) == false) {
            handle_error("invalid response caught.");
        }
        turn_controller = (turn_controller == Turn::PlayerATurn ? Turn::PlayerBTurn : Turn::PlayerATurn);
        
    }
}

void msg_broadcast(const std::vector<int>& clientSockets, const std::string& msg) {
    for (const auto& socket: clientSockets) {
        if (send(socket, msg.c_str(), msg.length() + 1, 0) < 0) {
            handle_error("Failed to broadcast to player " + SocketToName[socket] + "!");
        }
    }
}