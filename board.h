#include <vector>
#include <iostream>
constexpr int GRID_SIZE = 3;

enum Token{X, O, NOTHING};
enum Result{PlayerA, PlayerB, ONGOING};

class Board{
public:
    Board();
    Board(const std::string&);
    Result gameTerminated();
    bool placeToken(Token, int, int);
    bool hasToken(int, int);
    void printBoard();
    std::string BoardToString();
private:
    std::vector<std::vector<Token>> board;
};
