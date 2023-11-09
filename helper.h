#include <iostream>
#include <string>
#include <thread.h>
#include <mutex>
#include <cstdlib>
#include <cstring>
#include <atomic>
#include <unistd.h>
#include <winsock.h>
#include <unordered_map>

#define SERVER_PORT 5208
#define SERVER_IP "127.0.0.1"
#define MAX_CLIENT 2
constexpr int BUF_SIZE = 1000;

inline void handle_eror(const std::string& err_msg) noexcept {
    // this function outputs the error message and
    // terminate the function immediately.

    std::cout << "Error! " << err_msg << std::endl;
    exit(1);
}

inline void print(const std::string& msg) noexcept {
    // output the message
    std::cout << msg << std::endl;
}