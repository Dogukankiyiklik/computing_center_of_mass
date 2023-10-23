// 152120211104 - Doðukan KIYIKLIK
// 15212021     - Safiullah SEDIQI
// 15212021     - Yusuf Eren HOÞGÖR

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

// this struct for storing center of mass' row and column coordinates of each blobs
struct Result {
    float centerRow;
    float centerColumn;
};

// functions on the below for printing index number and '+--+' before and after table printed
void tableIndex(int count, int columns);
void tablePlus(int columns);

// recursive function that compute pixels of each blobs
int noOfPixels(char** blobsArr, char** blobCoppyArr, int rows, int columns, int startX, int startY, char target);

// function that calculate center of mass of each blobs
Result centerOfMass(char** blobCoppyArr, int rows, int columns);

int main() {
    // getting the file name from user 
    string fileName;
    cout << "enter the file name: ";
    cin >> fileName;

    // creating file
    ifstream myFile(fileName);

    // check if the file couldn't be opened
    if (!myFile) {
        cout << "File could not be opened";
        return 1;
    }

    // getting rows and columns from txt file
    int rows;
    int columns;
    myFile >> rows >> columns;

    // creating array for storig blobs in the txt file
    char** blobsArr = new char* [rows];
    for (int i = 0; i < rows; i++) {
        blobsArr[i] = new char[columns];
    }

    // creating coppy array (this array will be used calculating center of mass)
    char** blobCoppyArr = new char* [rows];
    for (int i = 0; i < rows; i++) {
        blobCoppyArr[i] = new char[columns];
    }

    // skipping first row of txt file. (because we don't want to store rows and columns informations in the array)
    string dummy;
    getline(myFile, dummy);

    // reading input from txt file and inserting into array
    for (int i = 0; i < rows; i++) {
        string line;
        getline(myFile, line);

        for (int j = 0; j < line.length(); j++) {
            blobsArr[i][j] = line[j];
        }
        // filling remaining spaces with ' ' blank
        for (int j = line.length(); j < columns; j++) {
            blobsArr[i][j] = ' ';
        }
    }

    // closing txt file after finished insert
    myFile.close();

    // printing index numbers of table and printing '+--+'
    int count = 0;
    tableIndex(count, columns);
    tablePlus(columns);

    // printing array
    int var = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (j == 0) {
                var = i;
                if (var > 9) {
                    var = 0;
                }
                cout << var << "|" << blobsArr[i][j];
            }
            else if (j == columns - 1) {
                if (var > 9) {
                    var = 0;
                }
                cout << blobsArr[i][j] << "|" << var;
            }
            else {
                cout << blobsArr[i][j];
            }
        }
        cout << endl;
    }

    // printing index numbers of table and printing '+--+'
    tablePlus(columns);
    tableIndex(count, columns);
    cout << endl << endl;

    // printing information table
    cout << "+------------------------------------------+" << endl;
    cout << "| BLOB |" << " NoOfPixels |" << " CoM Row |" << " CoM Column |" << endl;
    cout << "+------------------------------------------+" << endl;

    // computing pixel number of blobs
    int blobNumber = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (blobsArr[i][j] == 'x') {
                blobNumber++;
                // calculating how many pixels each blob has
                int pixelCount = noOfPixels(blobsArr, blobCoppyArr, rows, columns, i, j, 'x');

                // filling remaining spaces with ' ' blank on the coppy array (this will be used calculating center of mass, we fill ramining sapces with ' ' blank because calculating each of blobs individually)
                for (int i = 0; i < rows; i++) {
                    for (int j = 0; j < columns; j++) {
                        if (!(blobCoppyArr[i][j] == 'x')) {
                            blobCoppyArr[i][j] = ' ';
                        }
                    }
                }

                // calculating center of mass and assign values to variables
                Result result = centerOfMass(blobCoppyArr, rows, columns);
                result.centerRow = result.centerRow / pixelCount;
                result.centerColumn = result.centerColumn / pixelCount;

                result.centerRow = rows - result.centerRow - 1;
                result.centerColumn = columns - result.centerColumn - 1;

                // printing how many pixels each blob has and center of mass' row and column coordinates
                // setw() => for set the width of the next output field
                // setprecision() => for set the precision for floating-point values 
                cout << "|" << setw(6) << blobNumber << "|" << setw(12) << pixelCount << "|" << fixed << setprecision(2) << setw(9) << result.centerRow << "|" << fixed << setprecision(2) << setw(12) << result.centerColumn << "|" << endl;
            }
        }
    }
    cout << "+------------------------------------------+" << endl;

    // deleting arrays
    for (int i = 0; i < rows; i++) {
        delete[] blobsArr[i];
    }
    delete[] blobsArr;

    for (int i = 0; i < rows; i++) {
        delete[] blobCoppyArr[i];
    }
    delete[] blobCoppyArr;

    return 0;
}

void tableIndex(int count, int columns) {
    count = 0;
    for (int i = 0; i < columns; i++) {
        if (i == 0) {
            cout << "  ";
        }
        if (count > 9) {
            count = 0;
        }
        cout << count;
        count++;
    }
    cout << endl;
}

void tablePlus(int columns) {
    cout << " +";
    for (int i = 0; i < columns; i++) {
        cout << "-";
    }
    cout << "+" << endl;
}

int noOfPixels(char** blobsArr, char** blobCoppyArr, int rows, int columns, int startX, int startY, char target) {
    // Check if the current position is outside the array or if it's not the target character
    if (startX < 0 || startX >= rows || startY < 0 || startY >= columns || blobsArr[startX][startY] != target) {
        return 0;
    }

    // Mark the current position as visited and 
    blobCoppyArr[startX][startY] = blobsArr[startX][startY];
    blobsArr[startX][startY] = ' ';

    // Recursive calls for the neighboring pixels
    int count = 1;  // Count the current pixel
    count += noOfPixels(blobsArr, blobCoppyArr, rows, columns, startX - 1, startY, target);  // Up
    count += noOfPixels(blobsArr, blobCoppyArr, rows, columns, startX + 1, startY, target);  // Down
    count += noOfPixels(blobsArr, blobCoppyArr, rows, columns, startX, startY - 1, target);  // Left
    count += noOfPixels(blobsArr, blobCoppyArr, rows, columns, startX, startY + 1, target);  // Right

    return count;
}

Result centerOfMass(char** blobCoppyArr, int rows, int columns) {
    Result result;
    result.centerRow = 0;
    result.centerColumn = 0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (blobCoppyArr[i][j] == 'x') {
                result.centerRow += rows - i - 1;
                result.centerColumn += columns - j - 1;
                blobCoppyArr[i][j] = ' ';
            }
        }
    }
    return result;
}