#include "global.h"

/**
 * construct a new Matrix:: Matrix object
 */

Matrix::Matrix() {
    logger.log("Matrix::Matrix");
}

/**
 * @brief Construct a new Matrix:: Matrix object used in the case where the data
 * file is available and LOAD command has been called. This command should be
 * followed by calling the load function;
 *
 * @param matrixName 
 */
Matrix::Matrix(string matrixName) {
    logger.log("Matrix::Matrix");
    this->sourceFileName = "../data/" + matrixName + ".csv";
    this->matrixName = matrixName;
}

/**
 * @brief The load function is used when the LOAD MATRIX command is encountered. It
 * reads data from the source file, splits it into blocks and updates matrix
 * statistics.
 *
 * @return true if the matrix has been successfully loaded 
 * @return false if an error occurred 
 */
bool Matrix::load() {
    logger.log("Matrix::load");
    if (this->findMatrixProperties()) {
        return true;
    }
    return false;
}

bool Matrix::findMatrixProperties() {
    logger.log("Matrix::findMatrixProperties");
    fstream fin(this->sourceFileName, ios::in);
    string line;
    int size = 0, numZeros = 0;
    if(getline(fin, line)) {
        stringstream s(line);
        string word;
        while(getline(s, word, ',')) {
            size++;
            try {
                if(stoi(word) == 0) numZeros++;
            } catch(...) {
                cout << "ERROR: Matrix elements need to be of the type integer" << endl;
                return false;
            }
        }
        for(int row = 1; row < size; row++) {
            if(!getline(fin, line)) {
                cout << "ERROR: No of rows are less than number of columns!" << endl;
                return false;
            }
            stringstream s(line);
            for(int col = 0; col < size; col++) {
                if(!getline(s, word, ',')) {
                    cout << "ERROR: Data does not follow the structure of a matrix" << endl;
                }
                try {
                    if(stoi(word) == 0) numZeros++;
                } catch(...) {
                    cout << "ERROR: Matrix elements need to be of the type integer" << endl;
                    return false;
                }
            }
        }
        if(getline(fin, line)) {
            cout << "ERROR: No of rows are greater than number of columns!" << endl;
            return false;
        }
        this->size = size;
        this->numZeros = numZeros;
        if((numZeros * 100) / (size * size) >= 60) {
            this->isSparse = true;
        }
        return true;
    } else {
        cout << "ERROR: File is empty" << endl;
        return false;
    }
}

void Matrix::unload() {
    return;
}
