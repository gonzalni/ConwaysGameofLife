#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#define OS_MAC

using namespace std;
void clearScreen(void);

const int lines = 25;
const int columns = 25;

void fillMatrix(string matrix[lines][columns]){
    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < columns; j++) {
            matrix[i][j] = " ";
        }
    }
}

void clearScreen(void) {

  #if defined(OS_WIN)

  #elif defined(OS_MAC)
    cout << "\033[2J;" << "\033[1;1H"; 
  #endif

}

void firstGen(string matrix[lines][columns], int m){
    int count = 0;
    while (count < m) {
        int zei = rand() % lines;
        int spa = rand() % columns;
        if (matrix[zei][spa] != "*") {
            matrix[zei][spa] = "*";
            count++;
        }
    }
}

int ZdlN(const string matrix[lines][columns], int zei, int spa) {
    int lebendeNachbarn = 0;
    for (int rowOffset = -1; rowOffset <= 1; rowOffset++) {
        for (int colOffset = -1; colOffset <= 1; colOffset++) {
            if (rowOffset == 0 && colOffset == 0) {
                continue;
            }

            int nachbarZeile = zei + rowOffset;
            int nachbarSpalte = spa + colOffset;

            if (nachbarZeile >= 0 && nachbarZeile < lines && nachbarSpalte >= 0 && nachbarSpalte < columns) {
                if (matrix[nachbarZeile][nachbarSpalte] == "*") {
                    lebendeNachbarn++;
                }
            }
        }
    }
    return lebendeNachbarn;
}

void NeueGeneration(string matrix[lines][columns]) {
    string neueMatrix[lines][columns];

    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < columns; j++) {
            int lebendeNachbarn = ZdlN(matrix, i, j);
            if (matrix[i][j] == "*") {
                if (lebendeNachbarn < 2 || lebendeNachbarn > 3) {
                    neueMatrix[i][j] = " ";
                } else {
                    neueMatrix[i][j] = "*";
                }
            } else {
                if (lebendeNachbarn == 3) {
                    neueMatrix[i][j] = "*";
                } else {
                    neueMatrix[i][j] = " ";
                }
            }
        }
    }

    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < columns; j++) {
            matrix[i][j] = neueMatrix[i][j];
        }
    }
}

void output(const string matrix[lines][columns]) {
    cout << "|" << string(columns * 2 + 1, '-') << "|" << endl;
    for (int i = 0; i < lines; i++) {
        cout << "| ";
        for (int j = 0; j < columns; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << "|" << endl;
    }
    cout << "|" << string(columns * 2 + 1, '-') << "|" << endl;
}

void genLoop(string matrix[lines][columns], int Gen){
    for (int a = 0; a < Gen - 1; a++) {
        usleep(400000);
        clearScreen();
        NeueGeneration(matrix);
        cout << "\nGeneration " << a + 2  << ":\n";
        output(matrix);
    }
}

int main() {
    srand(time(0));
    string matrix[lines][columns];
    int m;
    int Gen;
    cout << "\nNumber of living cells in the first generation: ";
    cin >> m;
    cout << "\nHow many generations should be simulated?: ";
    cin >> Gen;
    cout << "\n";
    fillMatrix(matrix);
    firstGen(matrix, m);
    cout << "\nGeneration " << 1 << ":\n";
    output(matrix);
    genLoop(matrix, Gen);

    return 0;
}
