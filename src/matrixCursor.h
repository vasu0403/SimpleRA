#ifndef MATRIX_CURSOR
#define MATRIX_CURSOR

#include"matrixBufferManager.h"
/**
 * @brief The cursor is an important component of the system. To read from a
 * table, you need to initialize a cursor. The cursor reads rows from a page one
 * at a time.
 *
 */
class MatrixCursor{
    public:
    MatrixPage matrixPage;
    int pageIndex;
    string matrixName;
    int pagePointer;

    public:
    MatrixCursor(string matrixName, int pageIndex);
    vector<int> getNext();
    void nextPage(int pageIndex);
    vector<int> getLine(int lineNumber);
};

#endif