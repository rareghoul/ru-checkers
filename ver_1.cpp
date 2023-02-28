#include <iostream>
#include <cstdlib>
#include <vector>
#include <climits>
#include <ctime>
int main() {
    char board[8][8] = {{'o', '-', 'o', '-', 'o', '-', 'o', '-'},
                        {'-', 'o', '-', 'o', '-', 'o', '-', 'o'},
                        {'o', '-', 'o', '-', 'o', '-', 'o', '-'},
                        {'-', '-', '-', '-', '-', '-', '-', '-'},
                        {'-', '-', '-', '-', '-', '-', '-', '-'},
                        {'-', 'x', '-', 'x', '-', 'x', '-', 'x'},
                        {'x', '-', 'x', '-', 'x', '-', 'x', '-'},
                        {'-', 'x', '-', 'x', '-', 'x', '-', 'x'}};
    char player = 'x';
    srand(time(NULL));
    while (true) {
        drawBoard(board);
        if (player == 'x') {
            Move move = getUserMove(board, player);
            makeMove(board, move);
        } else {
            Move move;
            if (getDifficultyLevel() == 1) {
                move = getRandomMove(board, player);
            } else if (getDifficultyLevel() == 2) {
                move = getMediumDifficultyMove(board, player);
            } else {
                move = getHardDifficultyMove(board, player);
            }
            std::cout << "Computer chooses move: " << move.row << ", " << move.col << std::endl;
            makeMove(board, move);
        }
        if (isGameOver(board)) {
            drawBoard(board);
            char winner = getWinner(board);
            if (winner == 'x') {
                std::cout << "You win!" << std::endl;
            } else {
                std::cout << "Computer wins!" << std::endl;
            }
            break;
        }
        player = (player == 'x') ? 'o' : 'x';
    }
    return 0;
}

void drawBoard(char board[8][8]) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

int selectDifficulty() {
    int difficulty;
    cout << "Select difficulty level (1-3): ";
    cin >> difficulty;
    return difficulty;
}

struct Move {
    int fromRow, fromCol, toRow, toCol;
};

vector<Move> getPossibleMoves(char board[8][8], char player) {
    vector<Move> possibleMoves;
    int direction = (player == 'o') ? 1 : -1;
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (board[row][col] == player || board[row][col] == toupper(player)) {
                if (board[row + direction][col + 1] == ' ') {
                    possibleMoves.push_back({row, col, row + direction, col + 1});
                }
                if (board[row + direction][col - 1] == ' ') {
                    possibleMoves.push_back({row, col, row + direction, col - 1});
                }
                if (board[row + direction][col + 1] == toupper(player)) {
                    possibleMoves.push_back({row, col, row + direction, col + 1});
                }
                if (board[row + direction][col - 1] == toupper(player)) {
                    possibleMoves.push_back({row, col, row + direction, col - 1});
                }
                if (board[row + 2*direction][col + 2] == ' ' && (board[row + direction][col + 1] == toupper(player) || board[row + direction][col + 1] == tolower(player))) {
                    possibleMoves.push_back({row, col, row + 2*direction, col + 2});
                }
                if (board[row + 2*direction][col - 2] == ' ' && (board[row + direction][col - 1] == toupper(player) || board[row + direction][col - 1] == tolower(player))) {
                    possibleMoves.push_back({row, col, row + 2*direction, col - 2});
                }
            }
        }
    }
    return possibleMoves;
}
Move getRandomMove(vector<Move> possibleMoves) {
    int randomIndex = rand() % possibleMoves.size();
    return possibleMoves[randomIndex];
}

int evaluatePosition(char board[8][8], char player) {
    int playerScore = 0;
    int opponentScore = 0;
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (board[row][col] == player) {
                playerScore += 1;
            } else if (board[row][col] == toupper(player)) {
                playerScore += 3;
            } else if (board[row][col] == tolower(player)) {
                playerScore += 2;
            } else if (board[row][col] == toupper(tolower(player))) {
                opponentScore += 3;
            } else if (board[row][col] == tolower(tolower(player))) {
                opponentScore += 2;
            } else if (board[row][col] == toupper('o') || board[row][col] == 'o') {
                opponentScore += 1;
            } else if (board[row][col] == toupper('x') || board[row][col] == 'x') {
                playerScore += 1;
            }
        }
    }
    return playerScore - opponentScore;
}

int minimax(char board[8][8], char player, int depth, int alpha, int beta) {
    if (depth == 0) {
        return evaluatePosition(board, player);
    }
    vector<Move> possibleMoves = getPossibleMoves(board, player);
    if (possibleMoves.size() == 0) {
        return evaluatePosition(board, player);
    }
    if (player == 'x') {
        int bestValue = INT_MIN;
        for (int i = 0; i < possibleMoves.size(); i++) {
            char newBoard[8][8];
            copy(&board[0][0], &board[0][0]+8*8,&newBoard[0][0]);
Move getMediumDifficultyMove(char board[8][8], char player) {
    vector<Move> possibleMoves = getPossibleMoves(board, player);
    int bestValue = INT_MIN;
    int depth = 4;
    vector<Move> bestMoves;
    for (int i = 0; i < possibleMoves.size(); i++) {
        char newBoard[8][8];
        copy(&board[0][0], &board[0][0]+8*8,&newBoard[0][0]);
        makeMove(newBoard, possibleMoves[i]);
        int value = minimax(newBoard, 'o', depth, INT_MIN, INT_MAX);
        if (value > bestValue) {
            bestValue = value;
            bestMoves.clear();
            bestMoves.push_back(possibleMoves[i]);
        } else if (value == bestValue) {
            bestMoves.push_back(possibleMoves[i]);
        }
    }
    int randomIndex = rand() % bestMoves.size();
    return bestMoves[randomIndex];
}

int alphabeta(char board[8][8], char player, int depth, int alpha, int beta) {
    if (depth == 0) {
        return evaluatePosition(board, player);
    }
    vector<Move> possibleMoves = getPossibleMoves(board, player);
    if (possibleMoves.size() == 0) {
        return evaluatePosition(board, player);
    }
    if (player == 'x') {
        int bestValue = INT_MIN;
        for (int i = 0; i < possibleMoves.size(); i++) {
            char newBoard[8][8];
            copy(&board[0][0], &board[0][0]+8*8,&newBoard[0][0]);
            makeMove(newBoard, possibleMoves[i]);
            int value = alphabeta(newBoard, 'o', depth-1, alpha, beta);
            bestValue = max(bestValue, value);
            alpha = max(alpha, bestValue);
            if (beta <= alpha) {
                break;
            }
        }
        return bestValue;
    } else {
        int bestValue = INT_MAX;
        for (int i = 0; i < possibleMoves.size(); i++) {
            char newBoard[8][8];
            copy(&board[0][0], &board[0][0]+8*8,&newBoard[0][0]);
            makeMove(newBoard, possibleMoves[i]);
            int value = alphabeta(newBoard, 'x', depth-1, alpha, beta);
            bestValue = min(bestValue, value);
            beta = min(beta, bestValue);
            if (beta <= alpha) {
                break;
            }
        }
        return bestValue;
    }
}

Move getHardDifficultyMove(char board[8][8], char player) {
    vector<Move> possibleMoves = getPossibleMoves(board, player);
    int bestValue = INT_MIN;
    int depth = 6;
    vector<Move> bestMoves;
    for (int i = 0; i < possibleMoves.size(); i++) {
        char newBoard[8][8];
        copy(&board[0][0], &board[0][0]+8*8,&newBoard[0][0]);
        makeMove(newBoard, possibleMoves[i]);
        int value = alphabeta(newBoard, 'o', depth, INT_MIN, INT_MAX);
        if (value > bestValue) {
            bestValue = value;
            bestMoves.clear();
            bestMoves.push_back(possibleMoves[i]);
        } else if (value == bestValue) {
            bestMoves.push_back(possibleMoves[i]);
        }
    }
    int randomIndex = rand() % bestMoves.size();
    return bestMoves[randomIndex];
}


#dddd
