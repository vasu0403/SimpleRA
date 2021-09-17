#include "global.h"

MatrixBufferManager::MatrixBufferManager()
{
    logger.log("MatrixBufferManager::MatrixBufferManager");
}

/**
 * @brief Function called to read a page from the buffer manager. If the page is
 * not present in the pool, the page is read and then inserted into the pool.
 *
 * @param tableName 
 * @param pageIndex 
 * @return Page 
 */
MatrixPage MatrixBufferManager::getPage(string matrixName, int pageIndex)
{
    logger.log("MatrixBufferManager::getPage");
    string pageName = "../data/temp/"+ matrixName + "_Page" + to_string(pageIndex);
    if (this->inPool(pageName))
        return this->getFromPool(pageName);
    else
        return this->insertIntoPool(matrixName, pageIndex);
}

/**
 * @brief Checks to see if a page exists in the pool
 *
 * @param pageName 
 * @return true 
 * @return false 
 */
bool MatrixBufferManager::inPool(string pageName)
{
    logger.log("MatrixBufferManager::inPool");
    for (auto page : this->pages)
    {
        if (pageName == page.pageName)
            return true;
    }
    return false;
}

/**
 * @brief If the page is present in the pool, then this function returns the
 * page. Note that this function will fail if the page is not present in the
 * pool.
 *
 * @param pageName 
 * @return Page 
 */
MatrixPage MatrixBufferManager::getFromPool(string pageName)
{
    logger.log("MatrixBufferManager::getFromPool");
    for (auto page : this->pages)
        if (pageName == page.pageName)
            return page;
}

/**
 * @brief Inserts page indicated by tableName and pageIndex into pool. If the
 * pool is full, the pool ejects the oldest inserted page from the pool and adds
 * the current page at the end. It naturally follows a queue data structure. 
 *
 * @param tableName 
 * @param pageIndex 
 * @return Page 
 */
MatrixPage MatrixBufferManager::insertIntoPool(string matrixName, int pageIndex)
{
    logger.log("MatrixBufferManager::insertIntoPool");
    MatrixPage matrixPage(matrixName, pageIndex);
    if (this->pages.size() >= BLOCK_COUNT)
        pages.pop_front();
    pages.push_back(matrixPage);
    return matrixPage;
}

/**
 * @brief The buffer manager is also responsible for writing pages. This is
 * called when new tables are created using assignment statements.
 *
 * @param tableName 
 * @param pageIndex 
 * @param rows 
 * @param rowCount 
 */
void MatrixBufferManager::writePage(string matrixName, int pageIndex, vector<vector<int>> rows, int rowCount)
{
    logger.log("MatrixBufferManager::writePage");
    MatrixPage matrixPage(matrixName, pageIndex, rows, rowCount);
    matrixPage.writePage();
}

/**
 * @brief Deletes file names fileName
 *
 * @param fileName 
 */
void MatrixBufferManager::deleteFile(string fileName)
{
    
    if (remove(fileName.c_str()))
        logger.log("BufferManager::deleteFile: Err");
        else logger.log("BufferManager::deleteFile: Success");
}

/**
 * @brief Overloaded function that calls deleteFile(fileName) by constructing
 * the fileName from the tableName and pageIndex.
 *
 * @param tableName 
 * @param pageIndex 
 */
void MatrixBufferManager::deleteFile(string matrixName, int pageIndex)
{
    logger.log("MatrixBufferManager::deleteFile");
    string fileName = "../data/temp/"+matrixName + "_Page" + to_string(pageIndex);
    this->deleteFile(fileName);
}