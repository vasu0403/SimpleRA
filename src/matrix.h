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
    

    Matrix();
    Matrix(string matrixName);
    bool blockify();
    bool compressedBlockify();
    bool load();
    bool normalBlockify();
    void normalPrint();
    void print();
    void unload();
    void getNextPage(MatrixCursor *matrixCursor);
    void transpose();
    void normalTranspose();
    void inPlaceTranspose(vector<vector<int>>& matrix);


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
            fout << ", ";
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
};