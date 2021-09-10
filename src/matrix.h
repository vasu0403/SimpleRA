#include "cursor.h"

class Matrix {

public:
    string sourceFileName = "";
    string matrixName = "";
    uint columnCount = 0;
    long long int rowCount = 0;

    Matrix();
    Matrix(string matrixName);
    bool load();
    void unload();
};