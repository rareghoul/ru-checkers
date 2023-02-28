#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <stdexcept>


// Определения функций
void drawBoard();
std::vector<int> getPossibleMoves(int player);
bool isMoveValid(int player, int move);
void makeMove(int player, int move);
bool isJump(int move);
std::vector<int> getJumps(int player);
int getUserMove(int player);
int getDifficultyLevel();
int getRandomMove(int player);
int evaluatePosition(int player);
int minimax(int player, int depth, int alpha, int beta, bool maximizingPlayer);
int getMediumDifficultyMove(int player);
int alphabeta(int player, int depth, int alpha, int beta, bool maximizingPlayer);
int getHardDifficultyMove(int player);
bool isGameOver();
int getWinner();

int main() {
    // Основная логика игры
    while (!isGameOver()) {
        int currentPlayer = 1; // Задаем текущего игрока
        drawBoard(); // Отображаем текущее состояние игрового поля
        std::vector<int> possibleMoves = getPossibleMoves(currentPlayer); // Получаем возможные ходы для текущего игрока
        int move; // Объявляем переменную для хода
        if (currentPlayer == 1) { // Если текущий игрок - пользователь
            move = getUserMove(currentPlayer); // Получаем ход от пользователя
        } else { // Иначе, если текущий игрок - компьютер
            int difficultyLevel = getDifficultyLevel(); // Получаем уровень сложности
            if (difficultyLevel == 1) { // Если уровень сложности легкий
                move = getRandomMove(currentPlayer); // Выбираем случайный ход
            } else if (difficultyLevel == 2) { // Иначе, если уровень сложности средний
                move = getMediumDifficultyMove(currentPlayer); // Выбираем ход с помощью алгоритма минимакс
            } else { // Иначе, если уровень сложности трудный
                move = getHardDifficultyMove(currentPlayer); // Выбираем ход с помощью алгоритма альфа-бета отсечения
            }
        }
        if (isMoveValid(currentPlayer, move)) { // Проверяем, является ли ход допустимым для текущего игрока
            makeMove(currentPlayer, move); // Совершаем ход на доске
            if (!isJump(move)) { // Если это не прыжок, передаем ход следующему игроку
                currentPlayer = (currentPlayer == 1) ? 2 : 1;
            }
        }
    }
    // Определяем победителя и выводим результат
    int winner = getWinner();
    if (winner == 1) {
        std::cout << "Выиграл игрок 1!" << std::endl;
    } else if (winner == 2) {
        std::cout << "Выиграл игрок 2!" << std::endl;
    } else {
        std::cout << "Ничья!" << std::endl;
    }
    return 0;
}

//Реализация функций

void drawBoard() {
    std::cout << "  1 2 3 4 5 6 7 8" << std::endl; // Выводим названия столбцов
    for (int i = 0; i < 8; i++) {
        std::cout << i + 1 << " "; // Выводим номер строки
        for (int j = 0; j < 8; j++) {
            if ((i + j) % 2 == 0) { // Если клетка белая
                std::cout << " "; // Выводим пробел
            } else {
                int piece = getPieceAtPosition(i, j); // Получаем шашку на данной позиции
                if (piece == 0) { // Если клетка пустая
                    std::cout << "-"; // Выводим дефис
                } else if (piece == 1) { // Если на клетке стоит шашка первого игрока
                    std::cout << "o"; // Выводим символ шашки первого игрока
                } else if (piece == 2) { // Если на клетке стоит шашка второго игрока
                    std::cout << "x"; // Выводим символ шашки второго игрока
                }
            }
            std::cout << " "; // Выводим пробел между клетками
        }
        std::cout << i + 1 << std::endl; // Выводим номер строки еще раз
    }
    std::cout << "  1 2 3 4 5 6 7 8" << std::endl; // Выводим названия столбцов еще раз
}
std::vector<int> getPossibleMoves(int player) {
    std::vector<int> possibleMoves;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int piece = getPieceAtPosition(i, j); // Получаем тип шашки на данной позиции
            if (piece == player) { // Если на данной позиции стоит шашка текущего игрока
                std::vector<int> jumps = getJumps(i, j, player); // Получаем все возможные прыжки для данной шашки
                if (!jumps.empty()) { // Если есть возможные прыжки
                    possibleMoves.insert(possibleMoves.end(), jumps.begin(), jumps.end()); // Добавляем все прыжки в список возможных ходов
                } else { // Иначе, если нет возможных прыжков
                    std::vector<int> regularMoves = getRegularMoves(i, j, player); // Получаем все возможные обычные ходы для данной шашки
                    possibleMoves.insert(possibleMoves.end(), regularMoves.begin(), regularMoves.end()); // Добавляем все обычные ходы в список возможных ходов
                }
            }
        }
    }
    return possibleMoves;
}

bool isMoveValid(int player, int move) {
    int fromRow = move / 100; // Получаем строку начальной позиции
    int fromCol = (move / 10) % 10; // Получаем столбец начальной позиции
    int toRow = move % 10; // Получаем строку конечной позиции
    int toCol = (move % 100) / 10; // Получаем столбец конечной позиции
    if (!isValidPosition(fromRow, fromCol) || !isValidPosition(toRow, toCol)) { // Проверяем, что начальная и конечная позиции находятся в пределах игрового поля
        return false;
    }
    int piece = getPieceAtPosition(fromRow, fromCol); // Получаем тип шашки на начальной позиции
    if (piece != player) { // Проверяем, что на начальной позиции стоит шашка текущего игрока
        return false;
    }
    if (getPieceAtPosition(toRow, toCol) != 0) { // Проверяем, что конечная позиция свободна
        return false;
    }
    bool isJumpMove = isJump(move); // Проверяем, является ли ход прыжком
    if (isJumpMove) { // Если это прыжок
        std::vector<int> jumps = getJumps(fromRow, fromCol, player); // Получаем все возможные прыжки для данной шашки
        if (std::find(jumps.begin(), jumps.end(), move) == jumps.end()) { // Проверяем, что заданный ход является допустимым прыжком для данной шашки
            return false;
        }
    } else { // Иначе, если это обычный ход
        std::vector<int> regularMoves = getRegularMoves(fromRow, fromCol, player); // Получаем все возможные обычные ходы для данной шашки
        if (std::find(regularMoves.begin(), regularMoves.end(), move) == regularMoves.end()) { // Проверяем, что заданный ход является допустимым обычным ходом для данной шашки
            return false;
        }
    }
    return true; // Если все проверки прошли успешно, ход допустимый
}

bool isJump(int move) {
    int fromRow = move / 100; // Получаем строку начальной позиции
    int fromCol = (move / 10) % 10; // Получаем столбец начальной позиции
    int toRow = move % 10; // Получаем строку конечной позиции
    int toCol = (move % 100) / 10; // Получаем столбец конечной позиции
    int rowDiff = toRow - fromRow; // Вычисляем разницу строк
    int colDiff = toCol - fromCol; // Вычисляем разницу столбцов
    if (abs(rowDiff) == 2 && abs(colDiff) == 2) { // Если разница строк и столбцов равна 2
        int jumpedPieceRow = (fromRow + toRow) / 2; // Вычисляем строку шашки, которую мы перепрыгиваем
        int jumpedPieceCol = (fromCol + toCol) / 2; // Вычисляем столбец шашки, которую мы перепрыгиваем
        int jumpedPiece = getPieceAtPosition(jumpedPieceRow, jumpedPieceCol); // Получаем тип шашки, которую мы перепрыгиваем
        return jumpedPiece != 0; // Если на клетке, которую мы перепрыгиваем, есть шашка, это прыжок
    }
    return false; // Иначе это обычный ход
}

std::vector<int> getJumps(int player) {
    std::vector<int> jumps;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int piece = getPieceAtPosition(i, j); // Получаем тип шашки на данной позиции
            if (piece == player || piece == player + 2) { // Если на данной позиции стоит шашка текущего игрока или дамка текущего игрока
                std::vector<int> pieceJumps = getJumps(i, j, player); // Получаем все возможные прыжки для данной шашки или дамки
                jumps.insert(jumps.end(), pieceJumps.begin(), pieceJumps.end()); // Добавляем все возможные прыжки в список прыжков
            }
        }
    }
    return jumps;
}

int getUserMove(int player) {
    std::string input;
    int move;
    while (true) {
        std::cout << "Player " << player << ", enter your move: ";
        std::getline(std::cin, input);
        std::stringstream ss(input);
        if (ss >> move) {
            if (isMoveValid(player, move)) { // Проверяем, что ход допустимый
                return move; // Если ход допустимый, возвращаем его
            }
        }
        std::cout << "Invalid move. Try again." << std::endl;
    }
}

int getDifficultyLevel() {
    std::string input;
    int level;
    while (true) {
        std::cout << "Choose difficulty level (1 - easy, 2 - medium, 3 - hard): ";
        std::getline(std::cin, input);
        std::stringstream ss(input);
        if (ss >> level && level >= 1 && level <= 3) { // Проверяем, что введенный уровень сложности является допустимым
            return level; // Если уровень сложности допустимый, возвращаем его
        }
        std::cout << "Invalid difficulty level. Try again." << std::endl;
    }
}

int getRandomMove(int player) {
    std::vector<int> moves = getPossibleMoves(player); // Получаем все возможные ходы для заданного игрока
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, moves.size() - 1);
    return moves[dis(gen)]; // Возвращаем случайный ход из списка возможных ходов
}

int evaluatePosition(int player) {
    int score = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int piece = getPieceAtPosition(i, j); // Получаем тип шашки на данной позиции
            if (piece == player) { // Если на данной позиции стоит шашка текущего игрока
                score += 1; // Добавляем 1 к счету текущего игрока за каждую шашку
            } else if (piece == player + 2) { // Если на данной позиции стоит дамка текущего игрока
                score += 3; // Добавляем 3 к счету текущего игрока за каждую дамку
            } else if (piece == opponent(player)) { // Если на данной позиции стоит шашка противника
                score -= 1; // Вычитаем 1 из счета текущего игрока за каждую шашку противника
            } else if (piece == opponent(player) + 2) { // Если на данной позиции стоит дамка противника
                score -= 3; // Вычитаем 3 из счета текущего игрока за каждую дамку противника
            }
        }
    }
    return score;
}

int minimax(int player, int depth, int alpha, int beta, bool maximizingPlayer) {
    if (depth == 0 || isGameOver()) {
        return evaluatePosition(player); // Если достигнута максимальная глубина поиска или игра закончена, возвращаем оценку текущей позиции
    }

    if (maximizingPlayer) {
        int maxScore = INT_MIN;
        std::vector<int> moves = getPossibleMoves(player); // Получаем все возможные ходы для заданного игрока
        for (int move : moves) {
            if (isJump(move)) { // Если ход является прыжком
                makeMove(player, move); // Совершаем прыжок
                int score = minimax(player, depth - 1, alpha, beta, false); // Рекурсивно вызываем функцию minimax для следующего уровня дерева
                undoMove(player, move); // Отменяем ход
                maxScore = std::max(maxScore, score);
                alpha = std::max(alpha, score);
                if (beta <= alpha) {
                    break; // Применяем альфа-бета отсечение
                }
            } else {
                std::vector<int> jumps = getJumps(player); // Получаем все возможные прыжки для заданного игрока
                if (jumps.size() == 0) { // Если нет возможных прыжков
                    makeMove(player, move); // Совершаем обычный ход
                    int score = minimax(player, depth - 1, alpha, beta, false); // Рекурсивно вызываем функцию minimax для следующего уровня дерева
                    undoMove(player, move); // Отменяем ход
                    maxScore = std::max(maxScore, score);
                    alpha = std::max(alpha, score);
                    if (beta <= alpha) {
                        break; // Применяем альфа-бета отсечение
                    }
                }
            }
        }
        return maxScore;
    } else {
        int minScore = INT_MAX;
        std::vector<int> moves = getPossibleMoves(opponent(player)); // Получаем все возможные ходы для противника
        for (int move : moves) {
            if (isJump(move)) { // Если ход является прыжком
                makeMove(opponent(player), move); // Совершаем прыжок противника
                int score = minimax(player, depth - 1, alpha, beta, true); // Рекурсивно вызываем функцию minimax для следующего уровня дерева
                undoMove(opponent(player), move); // Отменяем ход противника
                minScore = std::min(minScore, score);
                beta = std::min(beta, score);
                if (beta <= alpha) {
                    break; // Применяем альфа-бет

                } else {
                  std::vector<int> jumps = getJumps(opponent(player)); // Получаем все возможные прыжки для противника
                  if (jumps.size() == 0) { // Если нет возможных прыжков
                      makeMove(opponent(player), move); // Совершаем обычный ход противника
                      int score = minimax(player, depth - 1, alpha, beta, true); // Рекурсивно вызываем функцию minimax для следующего уровня дерева
                      undoMove(opponent(player), move); // Отменяем ход противника
                      minScore = std::min(minScore, score);
                      beta = std::min(beta, score);
                      if (beta <= alpha) {
                          break; // Применяем альфа-бета отсечение
                        }
              }
          }
      }
      return minScore;
  }
}

int getMediumDifficultyMove(int player) {
    int maxScore = INT_MIN;
    int bestMove = -1;
    std::vector<int> moves = getPossibleMoves(player); // Получаем все возможные ходы для заданного игрока
    for (int move : moves) {
        if (isJump(move)) { // Если ход является прыжком
            makeMove(player, move); // Совершаем прыжок
            int score = minimax(player, MEDIUM_DIFFICULTY_LEVEL, INT_MIN, INT_MAX, false); // Вызываем функцию minimax для поиска лучшего хода
            undoMove(player, move); // Отменяем ход
            if (score > maxScore) {
                maxScore = score;
                bestMove = move;
            }
        } else {
            std::vector<int> jumps = getJumps(player); // Получаем все возможные прыжки для заданного игрока
            if (jumps.size() == 0) { // Если нет возможных прыжков
                makeMove(player, move); // Совершаем обычный ход
                int score = minimax(player, MEDIUM_DIFFICULTY_LEVEL, INT_MIN, INT_MAX, false); // Вызываем функцию minimax для поиска лучшего хода
                undoMove(player, move); // Отменяем ход
                if (score > maxScore) {
                    maxScore = score;
                    bestMove = move;
                }
            }
        }
    }
    return bestMove;
}

int alphabeta(int player, int depth, int alpha, int beta, bool maximizingPlayer) {
    if (depth == 0 || isGameOver()) {
        return evaluatePosition(player); // Если достигнута максимальная глубина поиска или игра закончена, возвращаем оценку текущей позиции
    }

    if (maximizingPlayer) {
        int maxScore = INT_MIN;
        std::vector<int> moves = getPossibleMoves(player); // Получаем все возможные ходы для заданного игрока
        for (int move : moves) {
            if (isJump(move)) { // Если ход является прыжком
                makeMove(player, move); // Совершаем прыжок
                int score = alphabeta(player, depth - 1, alpha, beta, false); // Рекурсивно вызываем функцию alphabeta для следующего уровня дерева
                undoMove(player, move); // Отменяем ход
                maxScore = std::max(maxScore, score);
                alpha = std::max(alpha, score);
                if (beta <= alpha) {
                    break; // Применяем альфа-бета отсечение
                }
            } else {
                std::vector<int> jumps = getJumps(player); // Получаем все возможные прыжки для заданного игрока
                if (jumps.size() == 0) { // Если нет возможных прыжков
                    makeMove(player, move); // Совершаем обычный ход
                    int score = alphabeta(player, depth - 1, alpha, beta, false); // Рекурсивно вызываем функцию alphabeta для следующего уровня дерева
                    undoMove(player, move); // Отменяем ход
                    maxScore = std::max(maxScore, score);
                    alpha = std::max(alpha, score);
                    if (beta <= alpha) {
                        break; // Применяем альфа-бета отсечение
                    }
                }
            }
        }
        return maxScore;
    } else {
        int minScore = INT_MAX;
        std::min(beta, score);
                if (beta <= alpha) {
                    break; // Применяем альфа-бета отсечение
                }
            } else {
                std::vector<int> jumps = getJumps(opponent(player)); // Получаем все возможные прыжки для противника
                if (jumps.size() == 0) { // Если нет возможных прыжков
                    makeMove(opponent(player), move); // Совершаем обычный ход противника
                    int score = alphabeta(player, depth - 1, alpha, beta, true); // Рекурсивно вызываем функцию alphabeta для следующего уровня дерева
                    undoMove(opponent(player), move); // Отменяем ход противника
                    minScore = std::min(minScore, score);
                    beta = std::min(beta, score);
                    if (beta <= alpha) {
                        break; // Применяем альфа-бета отсечение
                    }
                }
            }
        }
        return minScore;
    }
}

int getHardDifficultyMove(int player) {
    int maxScore = INT_MIN;
    int bestMove = -1;
    std::vector<int> moves = getPossibleMoves(player); // Получаем все возможные ходы для заданного игрока
    for (int move : moves) {
        if (isJump(move)) { // Если ход является прыжком
            makeMove(player, move); // Совершаем прыжок
            int score = alphabeta(player, HARD_DIFFICULTY_LEVEL, INT_MIN, INT_MAX, false); // Вызываем функцию alphabeta для поиска лучшего хода
            undoMove(player, move); // Отменяем ход
            if (score > maxScore) {
                maxScore = score;
                bestMove = move;
            }
        } else {
            std::vector<int> jumps = getJumps(player); // Получаем все возможные прыжки для заданного игрока
            if (jumps.size() == 0) { // Если нет возможных прыжков
                makeMove(player, move); // Совершаем обычный ход
                int score = alphabeta(player, HARD_DIFFICULTY_LEVEL, INT_MIN, INT_MAX, false); // Вызываем функцию alphabeta для поиска лучшего хода
                undoMove(player, move); // Отменяем ход
                if (score > maxScore) {
                    maxScore = score;
                    bestMove = move;
                }
            }
        }
    }
    return bestMove;
}


bool isGameOver() {
    return (getPossibleMoves(PLAYER_ONE).size() == 0 && getJumps(PLAYER_ONE).size() == 0) ||
           (getPossibleMoves(PLAYER_TWO).size() == 0 && getJumps(PLAYER_TWO).size() == 0);
}


int getWinner() {
    int playerOneScore = 0;
    int playerTwoScore = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i] == PLAYER_ONE || board[i] == PLAYER_ONE_KING) {
            playerOneScore++;
        } else if (board[i] == PLAYER_TWO || board[i] == PLAYER_TWO_KING) {
            playerTwoScore++;
        }
    }
    if (playerOneScore > playerTwoScore) {
        return PLAYER_ONE;
    } else if (playerTwoScore > playerOneScore) {
        return PLAYER_TWO;
    } else {
        return NO_PLAYER;
    }
}
