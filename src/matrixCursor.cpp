#include "global.h"

MatrixCursor::MatrixCursor(string matrixName, int pageIndex)
{
    logger.log("MatrixCursor::MatrixCursor");
    this->matrixPage = matrixBufferManager.getPage(matrixName, pageIndex);
    this->pagePointer = 0;
    this->matrixName = matrixName;
    this->pageIndex = pageIndex;
}

/**
 * @brief This function reads the next row from the page. The index of the
 * current row read from the page is indicated by the pagePointer(points to row
 * in page the cursor is pointing to).
 *
 * @return vector<int> 
 */
vector<int> MatrixCursor::getNext()
{
    logger.log("MatrixCursor::getNext");
    vector<int> result = this->matrixPage.getRow(this->pagePointer);
    this->pagePointer++;
    if(result.empty()){
        matrixCatalogue.getMatrix(this->matrixName)->getNextPage(this);
        if(!this->pagePointer){
            result = this->matrixPage.getRow(this->pagePointer);
            this->pagePointer++;
        } 
    }
    return result;
}

vector<int> MatrixCursor::getLine(int lineNumber)
{
    logger.log("MatrixCursor::getLine");
    vector<int> result = this->matrixPage.getRow(lineNumber);
    this->pagePointer = lineNumber + 1;
    return result;
}

/**
 * @brief Function that loads Page indicated by pageIndex. Now the cursor starts
 * reading from the new page.
 *
 * @param pageIndex 
 */
void MatrixCursor::nextPage(int pageIndex)
{
    logger.log("MatrixCursor::nextPage");
    this->matrixPage = matrixBufferManager.getPage(this->matrixName, pageIndex);
    this->pageIndex = pageIndex;
    this->pagePointer = 0;
}