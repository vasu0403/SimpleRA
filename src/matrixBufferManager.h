#ifndef MATRIX_BUFFER_MANAGER
#define MATRIX_BUFFER_MANAGER
#include"matrixPage.h"

class MatrixBufferManager{

    deque<MatrixPage> pages; 
    bool inPool(string pageName);
    MatrixPage getFromPool(string pageName);
    MatrixPage insertIntoPool(string tableName, int pageIndex);

    public:
    
    MatrixBufferManager();
    MatrixPage getPage(string tableName, int pageIndex);
    void writePage(string pageName, vector<vector<int>> rows);
    void deleteFile(string tableName, int pageIndex);
    void deleteFile(string fileName);
    void writePage(string tableName, int pageIndex, vector<vector<int>> rows, int rowCount);
};

#endif