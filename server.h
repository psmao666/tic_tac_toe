#include "helper.h"
#include <thread>
#include <vector>

std::atomic<int> n_client;
std::unordered_map<std::string, int> NameToSocket;
std::unordered_map<int, std::string> SocketToName;
enum Turn{PlayerATurn, PlayerBTurn};

int server_init();
void client_init(int);
void server_handler(int);
void game_handler(const std::vector<int>&);
void msg_broadcast(const std::vector<int>&, const std::string&);