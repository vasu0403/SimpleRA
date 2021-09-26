#include "matrixCursor.h"

class Matrix {
    bool findMatrixProperties();
public:
    string sourceFileName = "";
    string matrixName = "";
    uint size = 0;
    uint numZeros = 0;
    uint sizePerBlock = 43;
    uint blockingFactor = 0;
    bool isSparse = false;

    uint columnCount = 0;
    vector<uint> rowsPerBlockCount;

    // for sparse matrix
    uint maxRowsPerBlockSparse = 664;
    uint blockCount = 0;
    uint numRowsInLastBlock = 0;
    

    Matrix();
    Matrix(string matrixName);
    bool blockify();

    bool load();
    bool normalBlockify();
    bool normalStupidBlockify();
    bool compressedBlockify();
    
    void print();
    void normalPrint();
    void sparsePrint();

    void unload();
    void getNextPage(MatrixCursor *matrixCursor);

    void transpose();
    void normalTranspose();
    void sparseTranspose();
    void inPlaceTranspose(vector<vector<int>>& matrix);
    
    void makePermanent();
    void makePermanentNormal();
    void makePermanentNormalStupid();
    void makePermanentSparse();


/**
 * @brief Static function that takes a vector of valued and prints them out in a
 * comma seperated format.
 *
 * @tparam T current usaages include int and string
 * @param row 
 */
template <typename T>
void writeRow(vector<T> row, ostream &fout)
{
    logger.log("Matrix::printRow");
    for (int columnCounter = 0; columnCounter < row.size(); columnCounter++)
    {
        if(row[columnCounter] == -1) {
            break;
        }
        if (columnCounter != 0)
            fout << ",";
        fout << row[columnCounter];
    }
    fout << endl;
}

/**
 * @brief Static function that takes a vector of valued and prints them out in a
 * comma seperated format.
 *
 * @tparam T current usaages include int and string
 * @param row 
 */
template <typename T>
void writeRow(vector<T> row)
{
    logger.log("Matrix::printRow");
    ofstream fout(this->sourceFileName, ios::app);
    this->writeRow(row, fout);
    fout.close();
}

template <typename T>
void exportRow(vector<T> row, bool first, bool end, ostream &fout) {
    logger.log("Matrix::exportRow");
    if(row[0] == -1) return;
    for (int columnCounter = 0; columnCounter < row.size(); columnCounter++) {
        if(row[columnCounter] == -1) {
            break;
        }
        if (!first || columnCounter != 0)
            fout << ",";
        fout << row[columnCounter];
    }
    if(end) { 
        fout << endl;
    }
}
};