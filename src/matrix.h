#include "cursor.h"

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
    bool load();
    void unload();
    bool blockify();
    bool normalBlockify();
    bool compressedBlockify();
};