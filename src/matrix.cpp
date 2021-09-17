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
        return this->blockify();
    }
    return false;
}


/**
 * @brief This function splits the original matrix into smaller square matrices and stores them in multiple pages 
 *
 * @return true if the matrix has been successfully blockified 
 * @return false if an error occurred 
 */
bool Matrix::blockify() {
    logger.log("Matrix::blockify");
    if(this->isSparse) {
        return this->compressedBlockify();
    } else {
        return this->normalBlockify();
    }
}

/**
 * @brief This function blockifies the matrix when the input matrix is not sparse 
 *
 * @return true if the matrix has been successfully blockified 
 * @return false if an error occurred 
 */
bool Matrix::compressedBlockify() {
    logger.log("Matrix::compressedBlockify");
    return true;
}

bool Matrix::normalBlockify() {
    ifstream fin(this->sourceFileName, ios::in);
    this->blockingFactor = ceil((float)this->size / this->sizePerBlock);
    vector<int> row(this->sizePerBlock, -1);
    vector<vector<int>> defaultBlock(this->sizePerBlock, row);
    vector<vector<int>> blocks[this->blockingFactor];
    string line, word;
    for(int block = 0; block < this->blockingFactor; block++) {
        blocks[block] = defaultBlock;
    }
    for(int blockRowIndex = 0; blockRowIndex < this->blockingFactor; blockRowIndex++) {
        for(int i = 0; i < this->sizePerBlock; i++) {
            if(!getline(fin, line)) {
                line = "";
            }
            stringstream s(line);
            for(int blockColIndex = 0; blockColIndex < this->blockingFactor; blockColIndex++) {
                for(int j = 0; j < this->sizePerBlock; j++) {
                    if(getline(s, word, ',')) {
                        blocks[blockColIndex][i][j] = stoi(word);            
                    } else {
                        blocks[blockColIndex][i][j] = -1;
                    }
                }
            }
        }
        for(int blockColIndex = 0; blockColIndex < this->blockingFactor; blockColIndex++) {
            uint blockCount = (blockRowIndex * this->blockingFactor) + blockColIndex;
            bufferManager.writePage(this->matrixName, blockCount, blocks[blockColIndex], this->sizePerBlock);
        }
    }
    return true;
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
                fin.close();
                cout << "SEMANTIC ERROR: Matrix elements need to be of the type integer" << endl;
                return false;
            }
        }
        for(int row = 1; row < size; row++) {
            if(!getline(fin, line)) {
                fin.close();
                cout << "SEMANTIC ERROR: No of rows are less than number of columns!" << endl;
                return false;
            }
            stringstream s(line);
            for(int col = 0; col < size; col++) {
                if(!getline(s, word, ',')) {
                    fin.close();
                    cout << "SEMANTIC ERROR: Data does not follow the structure of a matrix" << endl;
                }
                try {
                    if(stoi(word) == 0) numZeros++;
                } catch(...) {
                    fin.close();
                    cout << "SEMANTIC ERROR: Matrix elements need to be of the type integer" << endl;
                    return false;
                }
            }
        }
        if(getline(fin, line)) {
            fin.close();
            cout << "SEMANTIC ERROR: No of rows are greater than number of columns!" << endl;
            return false;
        }
        this->size = size;
        this->numZeros = numZeros;
        if((numZeros * 100) / (size * size) >= 60) {
            this->isSparse = true;
        }
        fin.close();
        return true;
    } else {
        fin.close();
        cout << "SEMANTIC ERROR: File is empty" << endl;
        return false;
    }
}

void Matrix::unload() {
    return;
}
