#include "global.h"
/**
 * @brief Construct a new Page object. Never used as part of the code
 *
 */
MatrixPage::MatrixPage() {
    logger.log("MatrixPage::MatrixPage");
    this->pageName = "";
    this->matrixName = "";
    this->pageIndex = -1;
    this->countCols = 0;
    this->countRows = 0;
    this->rows.clear();
}

/**
 * @brief Construct a new Page:: Page object given the table name and page
 * index. When tables are loaded they are broken up into blocks of BLOCK_SIZE
 * and each block is stored in a different file named
 * "<tablename>_Page<pageindex>". For example, If the Page being loaded is of
 * table "R" and the pageIndex is 2 then the file name is "R_Page2". The page
 * loads the rows (or tuples) into a vector of rows (where each row is a vector
 * of integers).
 *
 * @param tableName 
 * @param pageIndex 
 */
MatrixPage::MatrixPage(string matrixName, int pageIndex)
{
    logger.log("MatrixPage::MatrixPage");
    this->matrixName = matrixName;
    this->pageIndex = pageIndex;
    this->pageName = "../data/temp/" + this->matrixName + "_Page" + to_string(pageIndex);
    Matrix matrix = *matrixCatalogue.getMatrix(matrixName);
    this->countRows = matrix.rowsPerBlockCount[pageIndex];
    this->countCols = matrix.columnCount;
    vector<int> row(this->countCols, 0);
    this->rows.assign(this->countRows, row);
 
    ifstream fin(pageName, ios::in);
    int number;
    for (uint rowCounter = 0; rowCounter < this->countRows; rowCounter++)
    {
        for (int columnCounter = 0; columnCounter < this->countCols; columnCounter++)
        {
            fin >> number;
            this->rows[rowCounter][columnCounter] = number;
        }
    }
    fin.close();
}

/**
 * @brief Get row from page indexed by rowIndex
 * 
 * @param rowIndex 
 * @return vector<int> 
 */
vector<int> MatrixPage::getRow(int rowIndex)
{
    logger.log("MatrixPage::getRow");
    vector<int> result;
    result.clear();
    if (rowIndex >= this->countRows)
        return result;
    return this->rows[rowIndex];
}

MatrixPage::MatrixPage(string matrixName, int pageIndex, vector<vector<int>> rows, int countRows)
{
    logger.log("MatrixPage::MatrixPage");
    this->matrixName = matrixName;
    this->pageIndex = pageIndex;
    this->rows = rows;
    this->countRows = countRows;
    this->countCols = rows[0].size();
    this->pageName = "../data/temp/"+this->matrixName + "_Page" + to_string(pageIndex);
}

/**
 * @brief writes current page contents to file.
 * 
 */
void MatrixPage::writePage(bool append)
{
    logger.log("MatrixPage::writePage");
    ofstream fout;
    if(append) {
        fout.open(this->pageName, ios::app);
    } else {
        fout.open(this->pageName, ios::trunc);
    }
    for (int rowCounter = 0; rowCounter < this->countRows; rowCounter++)
    {
        for (int columnCounter = 0; columnCounter < this->rows[rowCounter].size(); columnCounter++)
        {
            if (columnCounter != 0)
                fout << " ";
            fout << this->rows[rowCounter][columnCounter];
        }
        fout << endl;
    }
    fout.close();
}
