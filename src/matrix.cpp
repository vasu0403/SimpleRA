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

void Matrix::transpose() {
    logger.log("Matrix::transpose");
    if(this->isSparse) {
        this->sparseTranspose();
    } else {
        this->normalTranspose();
    }
}

void Matrix::inPlaceTranspose(vector<vector<int>>& matrix) {
    for(int row = 0; row < this->sizePerBlock; row++) {
        for(int col = 0; col < row; col++) {
            swap(matrix[row][col], matrix[col][row]);
        }
    }
}

void Matrix::normalTranspose() {
    for(int blockRow = 0; blockRow < this->blockingFactor; blockRow++) {
        for(int blockCol = 0; blockCol <= blockRow; blockCol++) {
            if(blockCol == blockRow) {
                int pageIndex = blockRow * this->blockingFactor + blockCol;
                MatrixCursor matrixCursor(this->matrixName, pageIndex);
                vector<vector<int>> submatrix(this->sizePerBlock);
                for(int rowInd = 0; rowInd < this->sizePerBlock; rowInd++) {
                    vector<int> row = matrixCursor.getNext();
                    submatrix[rowInd] = row;
                }
                this->inPlaceTranspose(submatrix);
                matrixBufferManager.writePage(this->matrixName, pageIndex, submatrix, this->sizePerBlock, false);
            } else {
                int pageIndex1 = blockRow * this->blockingFactor + blockCol;
                int pageIndex2 = blockCol * this->blockingFactor + blockRow;
                MatrixCursor matrixCursor1(this->matrixName, pageIndex1);
                MatrixCursor matrixCursor2(this->matrixName, pageIndex2);
                vector<vector<int>> submatrix1(this->sizePerBlock), submatrix2(this->sizePerBlock);
                vector<int> row;
                for(int rowInd = 0; rowInd < this->sizePerBlock; rowInd++) {
                    row = matrixCursor1.getNext();
                    submatrix1[rowInd] = row;
                    row = matrixCursor2.getNext();
                    submatrix2[rowInd] = row;
                }
                this->inPlaceTranspose(submatrix1);
                this->inPlaceTranspose(submatrix2);
                matrixBufferManager.writePage(this->matrixName, pageIndex1, submatrix2, this->sizePerBlock, false);
                matrixBufferManager.writePage(this->matrixName, pageIndex2, submatrix1, this->sizePerBlock, false);
            }
        }
    }
    matrixBufferManager.clearPool(this->matrixName);
}

void Matrix::sparseTranspose() {
    MatrixCursor matrixCursor(this->matrixName, 0);
    for(int block = 0; block < this->blockCount; block++) {
        int numRows = this->rowsPerBlockCount[block];
        vector<vector<int>> thisBlock(numRows);
        vector<int> row;
        for(int i = 0; i < numRows; i++) {
            row = matrixCursor.getNext();
            thisBlock[i] = {row[1], row[0], row[2]};
        }
        matrixBufferManager.writePage(this->matrixName, block, thisBlock, numRows, false);   
    }
    matrixBufferManager.clearPool(this->matrixName);
    for(int i = 0; i < this->blockCount; i++) {
        for(int j = i + 1; j < this->blockCount; j++) {
            int rowCount1 = this->rowsPerBlockCount[i];
            int rowCount2 = this->rowsPerBlockCount[j];
            vector<vector<int>> block1(rowCount1), block2(rowCount2), bothBlocks(rowCount1 + rowCount2);
            MatrixCursor matrixCursor1(this->matrixName, i);
            MatrixCursor matrixCursor2(this->matrixName, j);
            for(int rowInd = 0; rowInd < rowCount1; rowInd++) {
                bothBlocks[rowInd] = matrixCursor1.getNext();
            }
            for(int rowInd = 0; rowInd < rowCount2; rowInd++) {
                bothBlocks[rowCount1 + rowInd] = matrixCursor2.getNext();
            }
            sort(bothBlocks.begin(), bothBlocks.end());
            for(int rowInd = 0; rowInd < rowCount1; rowInd++) {
                block1[rowInd] = bothBlocks[rowInd];
            }
            for(int rowInd = 0; rowInd < rowCount2; rowInd++) {
                block2[rowInd] = bothBlocks[rowCount1 + rowInd];
            }
            matrixBufferManager.writePage(this->matrixName, i, block1, rowCount1, false);   
            matrixBufferManager.writePage(this->matrixName, j, block2, rowCount2, false);  
            matrixBufferManager.clearPool(this->matrixName);
        }
    }
    if(this->blockCount == 1) {
        int rowCount = this->rowsPerBlockCount[0];
        vector<vector<int>> block(rowCount);
        MatrixCursor matrixCursor(this->matrixName, 0);
        for(int rowInd = 0; rowInd < rowCount; rowInd++) {
            block[rowInd] = matrixCursor.getNext();
        }
        sort(block.begin(), block.end());
        matrixBufferManager.writePage(this->matrixName, 0, block, rowCount, false);   
    }
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
        return this->normalStupidBlockify();
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
    ifstream fin(this->sourceFileName, ios::in);
    vector<int> defaultRow(3, -1);
    vector<vector<int>> block(this->maxRowsPerBlockSparse, defaultRow);
    string word;
    int counter = 0;
    this->blockCount = 0;
    this->columnCount = 3;
    for(int row = 0; row < size; row++) {
        int flushed = 0;
        for(int col = 0; col < size; col++) {
            fin >> word;
            word.erase(remove(word.begin(), word.end(), ','), word.end());
            if(stoi(word) == 0) continue;
            block[counter++] = {row, col, stoi(word)};
            if(counter == this->maxRowsPerBlockSparse) {
                this->rowsPerBlockCount.emplace_back(counter);
                matrixBufferManager.writePage(this->matrixName, this->blockCount++, block, this->maxRowsPerBlockSparse, false);
                this->numRowsInLastBlock = counter;
                counter = 0;
            }
        }
    }
    if(counter) {
        this->numRowsInLastBlock = counter;
        this->rowsPerBlockCount.emplace_back(counter);
        matrixBufferManager.writePage(this->matrixName, this->blockCount++, block, counter, false);
    }
    return true;
}

bool Matrix::normalStupidBlockify() {
    logger.log("Matrix::normalStupidBlockify");
    ifstream fin(this->sourceFileName, ios::in);
    this->blockingFactor = ceil((float)this->size / this->sizePerBlock);
    vector<int> defaultRow(this->sizePerBlock, -1);
    vector<int> row(this->sizePerBlock, -1);
    vector<vector<int>> rows(1, defaultRow);
    string word;
    this->columnCount = this->sizePerBlock;

    int extendedSize = this->blockingFactor * this->sizePerBlock;
    for(int i = 0; i < extendedSize; i++) {
        int counter = 0;
        row = defaultRow;
        for(int j = 0; j < extendedSize; j++) {
            if(i < this->size && j < this->size) {
                fin >> word;
                word.erase(remove(word.begin(), word.end(), ','), word.end());
                row[counter] = stoi(word);
            }
            counter++;
            if(counter == this->sizePerBlock) {
                uint blockRowIndex = (i / this->sizePerBlock);
                uint blockColIndex = (j / this->sizePerBlock);
                uint blockCount = (blockRowIndex * this->blockingFactor) + blockColIndex;
                rows[0] = row;
                bool append = (i % this->sizePerBlock != 0);
                matrixBufferManager.writePage(this->matrixName, blockCount, rows, 1, append);
                counter = 0;
                row = defaultRow;
            }
        }
        if(counter) {
            uint blockRowIndex = (i / this->sizePerBlock);
            uint blockColIndex = ((this->size - 1) / this->sizePerBlock);
            uint blockCount = (blockRowIndex * this->blockingFactor) + blockColIndex;
            bool append = (i % this->sizePerBlock != 0);
            rows[0] = row;
            matrixBufferManager.writePage(this->matrixName, blockCount, rows, 1, append);
        }
    }
    for(int i = 0; i < blockingFactor * blockingFactor; i++) {
        this->rowsPerBlockCount.emplace_back(this->sizePerBlock);
    }
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
            matrixBufferManager.writePage(this->matrixName, blockCount, blocks[blockColIndex], this->sizePerBlock, false);
        }
    }
    return true;
}



bool Matrix::findMatrixProperties() {
    logger.log("Matrix::findMatrixProperties");
    cout << this->sourceFileName << endl;
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
            for(int col = 0; col < size; col++) {
                if(fin >> word) {
                    word.erase(remove(word.begin(), word.end(), ','), word.end());
                    if(stoi(word) == 0) numZeros++;     // do error checking here ??
                } else {
                    cout << "SEMANTIC ERROR: Input does not follow properties of a matrix" << endl;
                    return false;
                }
            }
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

void Matrix::normalPrint() {
    logger.log("Matrix::normalPrint");
    uint count = min((uint)20, this->size);

    MatrixCursor matrixCursor(this->matrixName, 0);
    vector<int> row;
    for(int rowCounter = 0; rowCounter < count; rowCounter++) {
        row = matrixCursor.getNext();
        this->writeRow(row, cout);
    }
    printRowCount(this->size);
}

void Matrix::print() {
    logger.log("Matrix::print");
    if(this->isSparse) {
        ;
    } else {
        this->normalPrint();
    }
}

void Matrix::unload() {
    return;
}

/**
 * @brief This function returns one row of the matrix using the cursor object. It
 * returns an empty row if all rows have been read.
 *
 * @param matrixCursor 
 * @return vector<int> 
 */
void Matrix::getNextPage(MatrixCursor *matrixCursor) {
    logger.log("Matrix::getNext");
    if (matrixCursor->pageIndex < (this->blockingFactor * this->blockingFactor) - 1) {
        matrixCursor->nextPage(matrixCursor->pageIndex+1);
    }
}

void Matrix::makePermanent()
{
    logger.log("Matrix::makePermanent");
    matrixBufferManager.deleteFile(this->sourceFileName);
    string newSourceFile = "../data/" + this->matrixName + ".csv";
    ofstream fout(newSourceFile, ios::out);
    
    MatrixCursor matrixCursor(this->matrixName, 0);

    vector<int> row(this->sizePerBlock, -1);
    vector<vector<int>> defaultBlock(this->sizePerBlock, row);
    vector<vector<int>> blocks[this->blockingFactor];
    for(int block = 0; block < this->blockingFactor; block++) {
        blocks[block] = defaultBlock;
    }

    for(int blockRowIndex = 0; blockRowIndex < this->blockingFactor; blockRowIndex++) {
        for(int blockColIndex = 0; blockColIndex < this->blockingFactor; blockColIndex++) {
            for(int rowInd = 0; rowInd < this->sizePerBlock; rowInd++) {
                blocks[blockColIndex][rowInd] = matrixCursor.getNext();
            }
        }
        for(int rowInd = 0; rowInd < this->sizePerBlock; rowInd++) {
            for(int blockColIndex = 0; blockColIndex < this->blockingFactor; blockColIndex++) {
                bool first = (blockColIndex == 0);
                bool end = (blockColIndex == this->blockingFactor - 1);
                this->exportRow(blocks[blockColIndex][rowInd], first, end, fout);
            }
        }
    }
    fout.close();
}
void Matrix::stupidMakePermanent() {
    logger.log("Matrix::stupidMakePermanent");
    matrixBufferManager.deleteFile(this->sourceFileName);
    string newSourceFile = "../data/" + this->matrixName + ".csv";
    ofstream fout(newSourceFile, ios::out);

    MatrixCursor matrixCursor(this->matrixName, 0);
    vector<int> row(this->sizePerBlock, -1);

    for(int blockRowIndex = 0; blockRowIndex < this->blockingFactor; blockRowIndex++) {
        for(int i = 0; i < this->sizePerBlock; i++) {
            for(int blockColIndex = 0; blockColIndex < this->blockingFactor; blockColIndex++) {
                int pageIndex = blockRowIndex * blockingFactor + blockColIndex;
                matrixCursor.nextPage(pageIndex);
                row = matrixCursor.getLine(i);
                bool first = (blockColIndex == 0);
                bool end = (blockColIndex == this->blockingFactor - 1);
                this->exportRow(row, first, end, fout);
            }
        }
    }
}