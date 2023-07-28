#include <iostream>
#include <cstdlib>
#include <chrono>
#include <unistd.h>
#include <random>
#include <thread>
#define OS_MAC

void clearScreen(void);

int lines = 0;
int columns = 0;

void fillMatrix(char** matrix) {
    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < columns; j++) {
            matrix[i][j] = ' ';
        }
    }
}

void clearScreen(void) {
    #if defined(OS_WIN)

    #elif defined(OS_MAC)
    std::cout << "\033[2J;" << "\033[1;1H"; 
    #endif
}

void firstGen(char** matrix, int m) {
    int count = 0;
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<int> distL(0, lines - 1);
    std::uniform_int_distribution<int> distC(0, columns - 1);
    while (count < m) {
        int Lines = distL(engine);
        int Columns = distC(engine);
        if (matrix[Lines][Columns] != '*') {
            matrix[Lines][Columns] = '*';
            count++;
        }
    }
}

int numberOfLivingNeighbours(char** matrix, int Lines, int Columns) {
    int livingNeighbours = 0;
    for (int rowOffset = -1; rowOffset <= 1; rowOffset++) {
        for (int colOffset = -1; colOffset <= 1; colOffset++) {
            if (rowOffset == 0 && colOffset == 0) {
                continue;
            }

            int neighbourLine = Lines + rowOffset;
            int neighbourColumn = Columns + colOffset;

            if (neighbourLine >= 0 && neighbourLine < lines && neighbourColumn >= 0 && neighbourColumn < columns) {
                if (matrix[neighbourLine][neighbourColumn] == '*') {
                    livingNeighbours++;
                }
            }
        }
    }
    return livingNeighbours;
}

void NewGeneration(char** matrix, char** newMatrix) {
    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < columns; j++) {
            int livingNeighbours = numberOfLivingNeighbours(matrix, i, j);
            if (matrix[i][j] == '*') {
                if (livingNeighbours < 2 || livingNeighbours > 3) {
                    newMatrix[i][j] = ' ';
                } else {
                    newMatrix[i][j] = '*';
                }
            } else {
                if (livingNeighbours == 3) {
                    newMatrix[i][j] = '*';
                } else {
                    newMatrix[i][j] = ' ';
                }
            }
        }
    }
}

void output(char** matrix) {
    std::cout << "|" << std::string(columns * 2 + 1, '-') << "|" << std::endl;
    for (int i = 0; i < lines; i++) {
        std::cout << "| ";
        for (int j = 0; j < columns; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << "|" << std::endl;
    }
    std::cout << "|" << std::string(columns * 2 + 1, '-') << "|" << std::endl;
}

template <typename T>
void customSwap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

void genLoop(char** matrix, const int Gen) {
    char** newMatrix = new char*[lines];
    for (int i = 0; i < lines; i++) {
        newMatrix[i] = new char[columns];
    }

    for (int a = 0; a < Gen - 1; a++) {
        std::this_thread::sleep_for(std::chrono::microseconds(400000));
        clearScreen();
        NewGeneration(matrix, newMatrix);
        std::cout << "\nGeneration " << a + 2 << ":\n";
        output(newMatrix);

        for (int i = 0; i < lines; i++) {
            for (int j = 0; j < columns; j++) {
                matrix[i][j] = newMatrix[i][j];
            }
        }
    }

    for (int i = 0; i < lines; i++) {
        delete[] newMatrix[i];
    }
    delete[] newMatrix;
    newMatrix = nullptr;
}




int main() {
    int m;
    int Gen;
    int size;
    std::cout << "\nNumber of living cells in the first generation: ";
    std::cin >> m;
    std::cout << "\nHow many generations should be simulated?: ";
    std::cin >> Gen;
    std::cout << "\nHow big should the environment be?: ";
    std::cin >> size;
    std::cout << "\n";
    lines = size;
    columns = size;

    char** matrix = new char*[lines];
    for (int i = 0; i < lines; i++) {
        matrix[i] = new char[columns];
    }

    fillMatrix(matrix);
    firstGen(matrix, m);
    std::cout << "\nGeneration " << 1 << ":\n";
    output(matrix);
    genLoop(matrix, Gen);

    for (int i = 0; i < lines; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
    return 0;
}
