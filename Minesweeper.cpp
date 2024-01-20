/**
*
* Solution to course project # <номер на вариант>
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2023/2024
*
* @author Petya Petrova
* @idnumber 1MI0600311
* @compiler VC
*
* Minesweeper
*
*/
#include <cstdlib> 
#include <iostream>
using namespace std;
constexpr int MIN_SIZE = 3;
constexpr int MAX_SIZE = 10;
constexpr int MAX_SIZE_OF_COMMAND = 100;
void rules() {
    cout << "The rules of the game are:" << endl;
    cout << "•you enter the size you want to play on (the smallest field you can play on is 3×3 and the biggest is 10×10)" << endl;
    cout << "•you enter the mines you want to put in the field (there has to be at least 1mine and the maximum is 3×size of the field)" << endl;
    cout << "•you have to write commands (open, mark, unmark) and the coordinates of your move" << endl;
    cout << "•the goal of the game is to open every cell without a mine or to mark every cell with mine" << endl;
    cout << "•if you open mine the game is over" << endl;
    cout << "Enjoy the game!" << endl;
}
bool isNumber(char ch) {
    return (ch >= '0' && ch <= '9');
}
void printTheField(const char fieldFrontend[][MAX_SIZE], size_t size) {
    if (!fieldFrontend) {
        return;
    }
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
            cout << "[" << fieldFrontend[i][j] << "]";
        cout << endl;
    }
}
int myStrcmp(const char* first, const char* second) {
    if (!first || !second)
        return 0;
    while ((*first) && (*second) && ((*first) == (*second))) {
        first++;
        second++;
    }
    return (*first - *second);
}
void setTheNumberOfSize(size_t& size) {
    bool hasTryedToWriteASizeYet = false;
    cout << "Write the size of the field you want to play:";
    do {
        if (hasTryedToWriteASizeYet) {
            cout << "Invalide input. Try again!";
        }
        cin >> size;
        hasTryedToWriteASizeYet = true;
    } while (size < MIN_SIZE || size > MAX_SIZE);
}
void setTheNumberOfMines(unsigned int& NumberOfMines, size_t size) {
    bool hasTryedToWriteNumOfMinesYet = false;
    cout << "How many mines you want to play with:";
    do {
        if (hasTryedToWriteNumOfMinesYet) {
            cout << "Invalide input. Try again!";
        }
        hasTryedToWriteNumOfMinesYet = true;
        cin >> NumberOfMines;
    } while (NumberOfMines < 1 || NumberOfMines>(3 * size));
}
void fillFieldFrontendWithSpace(char fieldFrontend[][MAX_SIZE], size_t size) {
    if (!fieldFrontend) {
        return;
    }
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
            fieldFrontend[i][j] = ' ';
    }
}
bool isMarkedWithAMine(char fieldBackend[][MAX_SIZE], int x, int y) {
    if (!fieldBackend) {
        return 0;
    }
    return fieldBackend[x][y] == '*';
}
void countTneMinesNextToTheCell(const char fieldBackend[][MAX_SIZE], size_t size, int i, int j, unsigned int& count) {
    if (!fieldBackend) {
        return;
    }
    for (int x = i - 1; x <= i + 1; x++) {
        if (x >= 0 && x < size) {
            for (int y = j - 1; y <= j + 1; y++) {
                if (y >= 0 && y < size)
                {
                    if (x == i && y == j)
                        continue;
                    if (fieldBackend[x][y] == '*')
                        count++;
                }
            }
        }
    }
}
char fromIndexToSymbol(unsigned int index) {
    if (index >= 0 && index <= 9) {
        return index + '0';
    }
    /*if (index >= 10 && index <= 15){
        return index + 'A' - 10;
    }*/
    return '\0';
}
void putNumbersInCellsNextToMine(char fieldBackend[][MAX_SIZE], size_t size) {
    if (!fieldBackend) {
        return;
    }
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (fieldBackend[i][j] == '*') {
                continue;
            }
            unsigned int count = 0;
            countTneMinesNextToTheCell(fieldBackend, size, i, j, count);
            char symbol = fromIndexToSymbol(count);
            fieldBackend[i][j] = symbol;
        }
    }
}
void putTheMinesOnRandomPossitions(char fieldBackend[][MAX_SIZE], size_t size, unsigned int numberOfMines) {
    if (!fieldBackend) {
        return;
    }
    srand(time(0));
    for (unsigned int i = 1; i <= numberOfMines; i++) {
        int x = rand() % size;
        int y = rand() % size;
        if (isMarkedWithAMine(fieldBackend, x, y)) {
            i--;
        }
        else
            fieldBackend[x][y] = '*';
    }
}
void readArrayCommand(char* command) {
    if (!command) {
        return;
    }
    cin >> command;
}
void writeTheCommand(char* command, int& x, int& y, size_t size) {
    if (!command) {
        return;
    }
    cout << "Write command (open, mark, unmark) and coordinates in the range of the field:";
    bool commandIsValid = true;
    do {
        commandIsValid = true;
        readArrayCommand(command);
        cin >> x >> y;
        if (x <= 0 || y <= 0 || x > size || y > size) {
            commandIsValid = false;
            cout << "Incorrect input! Try again" << endl;
        }
        if (!commandIsValid && myStrcmp(command, "open") != 0 && myStrcmp(command, "mark") != 0 && myStrcmp(command, "unmark") != 0) {
            commandIsValid = false;
            cout << "Incorrect input! Try again" << endl;
        }
    } while (!commandIsValid);
    x--;//we decrement the coordinates so they can match the array's index
    y--;
}
void openAllCellsWithZerosAround(char fieldFrontend[][MAX_SIZE], const char fieldBackend[][MAX_SIZE], size_t size, int x, int y, unsigned int& numberOfCellsToBeOpen) {
    /*if (fieldFrontend[x][y] == ' '){
        fieldFrontend[x][y] = fieldBackend[x][y];
        numberOfCellsToBeOpen--;
    }*/
    for (int i = x - 1; i <= x + 1; i++) {
        if (i >= 0 && i < size) {
            for (int j = y - 1; j <= y + 1; j++) {
                if (j >= 0 && j < size) {
                    if (i == x && j == y)
                        continue;
                    if (fieldFrontend[i][j] == ' ' && fieldBackend[i][j] != '*') {
                        fieldFrontend[i][j] = fieldBackend[i][j];
                        numberOfCellsToBeOpen--;
                        if (fieldFrontend[i][j] == '0') {
                            openAllCellsWithZerosAround(fieldFrontend, fieldBackend, size, i, j, numberOfCellsToBeOpen);
                        }
                    }
                }
            }
        }
    }
}
bool openCell(char fieldFrontend[][MAX_SIZE], const char fieldBackend[][MAX_SIZE], size_t size, int x, int y, unsigned int& numberOfCellsToBeOpen) {
    if (!fieldFrontend || !fieldBackend) {
        return 0;
    }
    if (fieldFrontend[x][y] == fieldBackend[x][y]) {
        cout << "You can't open a cell that is already open! Try again" << endl;
        return 1;
    }
    fieldFrontend[x][y] = fieldBackend[x][y];
    numberOfCellsToBeOpen--;
    if (fieldFrontend[x][y] == '*') {
        return 0;
    }
    if (fieldFrontend[x][y] == '0') {
        openAllCellsWithZerosAround(fieldFrontend, fieldBackend, size, x, y, numberOfCellsToBeOpen);
    }
    return 1;
}
void markCell(char fieldFrontend[][MAX_SIZE], const char fieldBackend[][MAX_SIZE], size_t size, int x, int y, unsigned int& minesMarked) {
    if (!fieldFrontend || !fieldBackend) {
        return;
    }
    if (fieldFrontend[x][y] == '^') {
        cout << "You can't mark a cell that is already marked! Try again" << endl;
    }
    if (isNumber(fieldFrontend[x][y])) {
        cout << "You can't mark a cell that is open! Try again" << endl;
    }
    else {
        fieldFrontend[x][y] = '^';
        if (fieldBackend[x][y] == '*') {
            minesMarked++;
        }
    }
}
void unmarkCell(char fieldFrontend[][MAX_SIZE], const char fieldBackend[][MAX_SIZE], size_t size, int x, int y, unsigned int& minesMarked) {
    if (!fieldFrontend || !fieldBackend) {
        return;
    }
    if (fieldFrontend[x][y] == '^') {
        fieldFrontend[x][y] = ' ';
        if (fieldBackend[x][y] == '*') {
            minesMarked--;
        }
    }
    else if (fieldFrontend[x][y] != '^') {
        cout << "The cell can't be unmarked because it is not marked! Try again" << endl;
    }
}
void takeAction(char fieldFrontend[][MAX_SIZE], const char fieldBackend[][MAX_SIZE], size_t size, char* command, int x, int y,
    bool& theGameIsNotOver, unsigned int& minesMarked, unsigned int& numberOfCellsToBeOpen) {
    if (!fieldFrontend || !fieldBackend || !command) {
        return;
    }
    if (myStrcmp(command, "open") == 0) {
        theGameIsNotOver = openCell(fieldFrontend, fieldBackend, size, x, y, numberOfCellsToBeOpen);
    }
    else if (myStrcmp(command, "mark") == 0) {
        markCell(fieldFrontend, fieldBackend, size, x, y, minesMarked);
    }
    else if (myStrcmp(command, "unmark") == 0) {
        unmarkCell(fieldFrontend, fieldBackend, size, x, y, minesMarked);
    }
}
bool playTheGame(char fieldFrontend[][MAX_SIZE], const char fieldBackend[][MAX_SIZE], size_t size, bool& theGameIsNotOver, unsigned int numberOfMines, unsigned int& numberOfCellsToBeOpen) {
    if (!fieldFrontend || !fieldBackend) {
        return 0;
    }
    char command[MAX_SIZE_OF_COMMAND];
    int xCoordinate, yCoordinate;
    writeTheCommand(command, xCoordinate, yCoordinate, size);
    unsigned int minesMarked = 0;
    takeAction(fieldFrontend, fieldBackend, size, command, xCoordinate, yCoordinate, theGameIsNotOver, minesMarked, numberOfCellsToBeOpen);
    if (minesMarked == numberOfMines || numberOfCellsToBeOpen == 0) {
        return 1;
    }
    return 0;
}

int main()
{
    rules();
    size_t size;
    setTheNumberOfSize(size);
    unsigned int numberOfMines;
    setTheNumberOfMines(numberOfMines, size);
    char fieldBackend[MAX_SIZE][MAX_SIZE];
    putTheMinesOnRandomPossitions(fieldBackend, size, numberOfMines);
    unsigned int numberOfCellsToBeOpen = (size * size) - numberOfMines;
    bool theGameIsNotOver = true;
    char fieldFrontend[MAX_SIZE][MAX_SIZE];
    putNumbersInCellsNextToMine(fieldBackend, size);
    fillFieldFrontendWithSpace(fieldFrontend, size);
    bool hasWon = 0;
    printTheField(fieldFrontend, size);
    while (theGameIsNotOver && !hasWon) {
        hasWon = playTheGame(fieldFrontend, fieldBackend, size, theGameIsNotOver, numberOfMines, numberOfCellsToBeOpen);
        printTheField(fieldFrontend, size);
    }
    if (theGameIsNotOver == false) {
        cout << "GAME OVER";
    }
    else
        cout << "CONGRATULATION! YOU WON!";
    return 0;
}
