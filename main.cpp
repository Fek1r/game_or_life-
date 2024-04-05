#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>

using namespace std;

const int HEIGHT = 20;
const int WIDTH = 40;

void display(char universe[HEIGHT][WIDTH]) {
    system("clear"); // Clear the screen (works on Unix-like systems)
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            cout << (universe[i][j] == '*' ? "\u25A0" : " ");
        }
        cout << endl;
    }
}

void update(char universe[HEIGHT][WIDTH]) {
    char newUniverse[HEIGHT][WIDTH] = {};
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            int liveNeighbors = 0;
            for (int di = -1; di <= 1; ++di) {
                for (int dj = -1; dj <= 1; ++dj) {
                    if (di == 0 && dj == 0) continue;
                    int ni = (i + di + HEIGHT) % HEIGHT;
                    int nj = (j + dj + WIDTH) % WIDTH;
                    if (universe[ni][nj] == '*') {
                        liveNeighbors++;
                    }
                }
            }
            if (universe[i][j] == '*' && (liveNeighbors == 2 || liveNeighbors == 3)) {
                newUniverse[i][j] = '*';
            } else if (universe[i][j] == ' ' && liveNeighbors == 3) {
                newUniverse[i][j] = '*';
            } else {
                newUniverse[i][j] = ' ';
            }
        }
    }
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            universe[i][j] = newUniverse[i][j];
        }
    }
}

void readBMP(const string& fileName, char universe[HEIGHT][WIDTH]) {
    ifstream bmpFile(fileName, ios::binary);
    if (!bmpFile) {
        cerr << "Cannot open file" << endl;
        return;
    }

    bmpFile.seekg(10);
    int dataOffset;
    bmpFile.read(reinterpret_cast<char*>(&dataOffset), sizeof(int));

    bmpFile.seekg(18);
    int imgWidth, imgHeight;
    bmpFile.read(reinterpret_cast<char*>(&imgWidth), sizeof(int));
    bmpFile.read(reinterpret_cast<char*>(&imgHeight), sizeof(int));

    const int rowPadding = (4 - (imgWidth * 3) % 4) % 4;
    vector<unsigned char> rowBuffer(imgWidth * 3 + rowPadding);

    bmpFile.seekg(dataOffset);
    for (int i = 0; i < imgHeight; ++i) {
        bmpFile.read(reinterpret_cast<char*>(rowBuffer.data()), rowBuffer.size());
        for (int j = 0; j < imgWidth; ++j) {
            int index = j * 3;
            universe[imgHeight - 1 - i][j] = (rowBuffer[index] > 128 && rowBuffer[index + 1] > 128 && rowBuffer[index + 2] > 128) ? '*' : ' ';
        }
    }

    bmpFile.close();
}

int main() {
    char universe[HEIGHT][WIDTH] = {};

    readBMP("game.bmp", universe);

    while (true) {
        display(universe);
        update(universe);
        usleep(200000); // Microsecond delay for visualization of update
    }

    return 0;
}