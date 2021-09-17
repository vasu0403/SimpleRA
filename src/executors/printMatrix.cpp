#include "global.h"
/**
 * @brief 
 * SYNTAX: PRINT relation_name
 */
bool syntacticParsePrintMATRIX()
{
    logger.log("syntacticParsePrintMATRIX");
    if (tokenizedQuery.size() != 3)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = PRINT_MATRIX;
    parsedQuery.printMatrixName = tokenizedQuery[2];
    return true;
}

bool semanticParsePrintMATRIX()
{
    logger.log("semanticParsePrintMATRIX");
    if (!matrixCatalogue.isMatrix(parsedQuery.printMatrixName))
    {
        cout << "SEMANTIC ERROR: Matrix doesn't exist" << endl;
        return false;
    }
    return true;
}

void executePrintMATRIX()
{
    logger.log("executePrintMATRIX");
    Matrix* matrix = matrixCatalogue.getMatrix(parsedQuery.printMatrixName);
    matrix->print();
    return;
}
