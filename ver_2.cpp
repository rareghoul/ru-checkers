void drawBoard(char board[8][8]) {
    std::cout << "  ";
    for (int i = 0; i < 8; i++) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < 8; i++) {
        std::cout << i << " ";
        for (int j = 0; j < 8; j++) {
            std::cout << board[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
std::vector<Move> getPossibleMoves(char board[8][8], char player) {
    std::vector<Move> possibleMoves;
    std::vector<Move> possibleJumps;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j] == player || board[i][j] == toupper(player)) {
                Move move(i, j);
                std::vector<Move> jumps = getJumps(board, move);
                if (jumps.size() > 0) {
                    possibleJumps.insert(possibleJumps.end(), jumps.begin(), jumps.end());
                } else {
                    std::vector<Move> moves = getMoves(board, move);
                    possibleMoves.insert(possibleMoves.end(), moves.begin(), moves.end());
                }
            }
        }
    }
    if (possibleJumps.size() > 0) {
        return possibleJumps;
    } else {
        return possibleMoves;
    }
}
bool isMoveValid(char board[8][8], Move move, char player) {
    if (move.row < 0 || move.row > 7 || move.col < 0 || move.col > 7) {
        return false;
    }
    if (board[move.row][move.col] != '-') {
        return false;
    }
    if (player == 'x') {
        if (move.row > 0 && move.row < 7 && move.col > 0 && move.col < 7) {
            if (board[move.row-1][move.col-1] == 'o' && board[move.row+1][move.col+1] == 'o') {
                return false;
            }
            if (board[move.row-1][move.col+1] == 'o' && board[move.row+1][move.col-1] == 'o') {
                return false;
            }
        }
        if (move.row == 0 || board[move.row-1][move.col-1] != 'o' && board[move.row-1][move.col+1] != 'o') {
            return false;
        }
    } else if (player == 'o') {
        if (move.row > 0 && move.row < 7 && move.col > 0 && move.col < 7) {
            if (board[move.row-1][move.col-1] == 'x' && board[move.row+1][move.col+1] == 'x') {
                return false;
            }
            if (board[move.row-1][move.col+1] == 'x' && board[move.row+1][move.col-1] == 'x') {
                return false;
            }
        }
        if (move.row == 7 || board[move.row+1][move.col-1] != 'x' && board[move.row+1][move.col+1] != 'x') {
            return false;
        }
    }
    return true;
}
void makeMove(char board[8][8], Move move) {
    int row = move.row;
    int col = move.col;
    board[row][col] = board[selectedPiece.row][selectedPiece.col];
    board[selectedPiece.row][selectedPiece.col] = '-';
    if (isJump(move)) {
        int jumpRow = (row + selectedPiece.row) / 2;
        int jumpCol = (col + selectedPiece.col) / 2;
        board[jumpRow][jumpCol] = '-';
        selectedPiece = move;
        std::vector<Move> jumps = getJumps(board, selectedPiece);
        if (jumps.size() > 0) {
            std::cout << "You must continue jumping." << std::endl;
        }
    } else {
        selectedPiece = move;
    }
    if (player == 'x' && row == 0) {
        board[row][col] = 'X';
    } else if (player == 'o' && row == 7) {
        board[row][col] = 'O';
    }
}
bool isJump(Move move) {
    return (abs(move.row - selectedPiece.row) == 2 && abs(move.col - selectedPiece.col) == 2);
}
std::vector<Move> getMoves(char board[8][8], Move move) {
    std::vector<Move> moves;
    int row = move.row;
    int col = move.col;
    char player = board[row][col];
    if (player == 'x' || player == 'X') {
        if (row > 0 && col > 0 && board[row-1][col-1] == '-') {
            Move newMove(row-1, col-1);
            moves.push_back(newMove);
        }
        if (row > 0 && col < 7 && board[row-1][col+1] == '-') {
            Move newMove(row-1, col+1);
            moves.push_back(newMove);
        }
        if (player == 'X') {
            if (row < 7 && col > 0 && board[row+1][col-1] == '-') {
                Move newMove(row+1, col-1);
                moves.push_back(newMove);
            }
            if (row < 7 && col < 7 && board[row+1][col+1] == '-') {
                Move newMove(row+1, col+1);
                moves.push_back(newMove);
            }
        }
    } else if (player == 'o' || player == 'O') {
        if (row < 7 && col > 0 && board[row+1][col-1] == '-') {
            Move newMove(row+1, col-1);
            moves.push_back(newMove);
        }
        if (row < 7 && col < 7 && board[row+1][col+1] == '-') {
            Move newMove(row+1, col+1);
            moves.push_back(newMove);
        }
        if (player == 'O') {
            if (row > 0 && col > 0 && board[row-1][col-1] == '-') {
                Move newMove(row-1, col-1);
                moves.push_back(newMove);
            }
            if (row > 0 && col < 7 && board[row-1][col+1] == '-') {
                Move newMove(row-1, col+1);
                moves.push_back(newMove);
            }
        }
    }
    return moves;
}
int evaluatePosition(char board[8][8], char player) {
    int value = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j] == player || board[i][j] == toupper(player)) {
                value += 5;
                if (board[i][j] == toupper(player)) {
                    value += 5;
                }
                if (i == 0 || i == 7) {
                    value += 3;
                }
                if (j == 0 || j == 7) {
                    value += 3;
                }
            } else if (board[i][j] != '-') {
                value -= 5;
                if (toupper(board[i][j]) == player) {
                    value -= 5;
                }
                if (i == 0 || i == 7) {
                    value -= 3;
                }
                if (j == 0 || j == 7) {
                    value -= 3;
                }
            }
        }
    }
    return value;
}
int minimax(char board[8][8], char player, int depth, int alpha, int beta) {
    if (depth == 0) {
        return evaluatePosition(board, player);
    }
    vector<Move> possibleMoves = getAllMoves(board, player);
    if (possibleMoves.size() == 0) {
        return evaluatePosition(board, player);
    }
    if (player == 'x') {
        int bestValue = -100000;
        for (int i = 0; i < possibleMoves.size(); i++) {
            Move move = possibleMoves[i];
            if (isJump(move)) {
                char newBoard[8][8];
                copyBoard(board, newBoard);
                makeJump(newBoard, move);
                int value = minimax(newBoard, player, depth-1, alpha, beta);
                if (value > bestValue) {
                    bestValue = value;
                }
                if (bestValue > alpha) {
                    alpha = bestValue;
                }
                if (beta <= alpha) {
                    break;
                }
            } else {
                vector<Move> jumps = getJumps(board, move);
                if (jumps.size() == 0) {
                    char newBoard[8][8];
                    copyBoard(board, newBoard);
                    makeMove(newBoard, move);
                    int value = minimax(newBoard, 'o', depth-1, alpha, beta);
                    if (value > bestValue) {
                        bestValue = value;
                    }
                    if (bestValue > alpha) {
                        alpha = bestValue;
                    }
                    if (beta <= alpha) {
                        break;
                    }
                }
            }
        }
        return bestValue;
    } else if (player == 'o') {
        int bestValue = 100000;
        for (int i = 0; i < possibleMoves.size(); i++) {
            Move move = possibleMoves[i];
            if (isJump(move)) {
                char newBoard[8][8];
                copyBoard(board, newBoard);
                makeJump(newBoard, move);
                int value = minimax(newBoard, player, depth-1, alpha, beta);
                if (value < bestValue) {
                    bestValue = value;
                }
                if (bestValue < beta) {
                    beta = bestValue;
                }
                if (beta <= alpha) {
                    break;
                }
            } else {
                vector<Move> jumps = getJumps(board, move);
                if (jumps.size() == 0) {
                    char newBoard[8][8];
                    copyBoard(board, newBoard);
                    makeMove(newBoard, move);
                    int value = minimax(newBoard, 'x', depth-1, alpha, beta);
                    if (value < bestValue) {
                        bestValue = value;
                    }
                    if (bestValue < beta) {
                        beta = bestValue;
                    }
                    if (beta <= alpha) {
                        break;
                    }
                }
            }
        }
        return bestValue;
    }
}
Move getBestMove(char board[8][8], char player, int depth) {
    vector<Move> possibleMoves = getAllMoves(board, player);
    Move bestMove = possibleMoves[0];
    int bestValue = -100000;
    for (int i = 0; i < possibleMoves.size(); i++) {
        Move move = possibleMoves[i];
        if (isJump(move)) {
            char newBoard[8][8];
            copyBoard(board, newBoard);
            makeJump(newBoard, move);
            int value = minimax(newBoard, player, depth-1, -100000, 100000);
            if (value > bestValue) {
                bestValue = value;
                bestMove = move;
            }
        } else {
            vector<Move> jumps = getJumps(board, move);
            if (jumps.size() == 0) {
                char newBoard[8][8];
                copyBoard(board, newBoard);
                makeMove(newBoard, move);
                int value = minimax(newBoard, 'o', depth-1, -100000, 100000);
                if (value > bestValue) {
                    bestValue = value;
                    bestMove = move;
                }
            }
        }
    }
    return bestMove;
}
void playGame(char board[8][8], int difficulty) {
    char player = 'x';
    while (true) {
        drawBoard(board);
        if (isGameOver(board)) {
            cout << "Game Over!" << endl;
            return;
        }
        if (player == 'x') {
            Move move = getPlayerMove(board, player);
            if (isJump(move)) {
                makeJump(board, move);
            } else {
                makeMove(board, move);
            }
        } else {
            Move move = getBestMove(board, player, difficulty);
            if (isJump(move)) {
                makeJump(board, move);
            } else {
                makeMove(board, move);
            }
        }
        player = (player == 'x') ? 'o' : 'x';
    }
}
#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

struct Move {
    int fromRow;
    int fromCol;
    int toRow;
    int toCol;
};

void drawBoard(char board[8][8]);
void initializeBoard(char board[8][8]);
vector<Move> getJumps(char board[8][8], Move move);
vector<Move> getPossibleMoves(char board[8][8], int row, int col);
vector<Move> getAllMoves(char board[8][8], char player);
bool isJump(Move move);
bool isGameOver(char board[8][8]);
Move getPlayerMove(char board[8][8], char player);
void makeMove(char board[8][8], Move move);
void makeJump(char board[8][8], Move move);
void copyBoard(char board[8][8], char newBoard[8][8]);
int evaluatePosition(char board[8][8], char player);
int minimax(char board[8][8], char player, int depth, int alpha, int beta);
Move getBestMove(char board[8][8], char player, int depth);
void playGame(char board[8][8], int difficulty);

int main() {
    char board[8][8];
    initializeBoard(board);
    int difficulty;
    cout << "Select difficulty level (1, 2, or 3): ";
    cin >> difficulty;
    playGame(board, difficulty);
    return 0;
}

