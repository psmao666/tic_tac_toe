#include "board.h"
#include <vector>
#include <iostream>
#include <mutex>
#include <cstring>

Board::Board() {
    // Generate a plain 3 X 3 board, filled with EMPTY CELL
    board = std::vector<std::vector<Token>>(GRID_SIZE, 
                        std::vector<Token>(GRID_SIZE, NOTHING));    
}

Board::Board(const std::string& compressedBoard) {
    // board constructor, with a given string (compressed board)
    board = std::vector<std::vector<Token>>();
    auto tmp = std::vector<Token>();
    int iter = 0;

    for (int i = 0; i < GRID_SIZE; ++ i) {
        tmp.clear();
        for (int j = 0; j < GRID_SIZE; ++ j) {
            if (compressedBoard[iter] == 'X') tmp.push_back(Token::X);
            else if (compressedBoard[iter] == 'O') tmp.push_back(Token::O);
            else tmp.push_back(Token::NOTHING);
            ++ iter;
        }
        board.push_back(tmp);
    }
}

bool Board::placeToken(Token token, int x, int y) {
    // Place a token at (x, y) on the board
    // return false if 1. x or y out of range
    //                 2. board[x][y] already has token
    // return true if token placed successfully. 
    if (x < 0 || y < 0 || x >= GRID_SIZE || y >= GRID_SIZE) {
        std::cout << "Illegal Play: Out of Boundary!\n";
        return false;
    }
    if (board[x][y] != Token::NOTHING) {
        std::cout << "Illegal Play: This spot has been taken!\n";
        return false;
    }
    board[x][y] = token;
    return true;
}

void Board::printBoard() {
    // print the board
    // "." means empty cell
    for (int i = 0; i < GRID_SIZE; ++ i) {
        for (int j = 0; j < GRID_SIZE; ++ j) {
            std::cout << (board[i][j] == Token::X ? 'X' :
                            (board[i][j] == Token::O ? 'O' : '.'));
        }
        std::cout << std::endl;
    }
}

std::string Board::BoardToString() {
    // convert the board to string
    std::string stringfiedBoard{};
    for (int i = 0; i < GRID_SIZE; ++ i) {
        for (int j = 0; j < GRID_SIZE; ++ j) {
            stringfiedBoard += (board[i][j] == Token::X ? 'X' :
                                (board[i][j] == Token::O ? 'O' : '.'));
        }
    }
    return stringfiedBoard;
}

Result Board::gameTerminated() {
    std::mutex mtx;
    auto checkSameToken = [&](int x, int y, Token tkn) {
        if (x < 0 || y < 0 || x >= 3 || y >= 3) return false;
        return (board[x][y] == tkn);
    };
    auto validator = [&](int x, int y) {
        return ((checkSameToken(x - 1, y - 1, board[x][y]) && checkSameToken(x + 1, y + 1, board[x][y])) ||
                (checkSameToken(x, y - 1, board[x][y]) && checkSameToken(x, y + 1, board[x][y])) ||
                (checkSameToken(x + 1, y - 1, board[x][y]) && checkSameToken(x - 1, y + 1, board[x][y])) ||
                (checkSameToken(x - 1, y, board[x][y]) && checkSameToken(x + 1, y, board[x][y])));
    };
    std::lock_guard<std::mutex> lck{mtx};
    for (int i = 0; i < 3; ++ i) {
        for (int j = 0; j < 3; ++ j) {
            if (board[i][j] != Token::NOTHING && validator(i, j)) {
                if (board[i][j] == Token::O) return Result::PlayerB;
                else return Result::PlayerA;
            }
        }
    }
    return Result::ONGOING;
}

bool Board::hasToken(int x, int y) {
    std::mutex mtx;
    std::lock_guard<std::mutex> lck{mtx};
    return board[x][y] != Token::NOTHING;
}