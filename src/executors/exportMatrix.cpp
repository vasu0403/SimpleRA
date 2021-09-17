#include "global.h"

/**
 * @brief 
 * SYNTAX: EXPORT <relation_name> 
 */

bool syntacticParseExportMATRIX()
{
    logger.log("syntacticParseExportMATRIX");
    if (tokenizedQuery.size() != 3)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = EXPORT_MATRIX;
    parsedQuery.exportMatrixName = tokenizedQuery[2];
    return true;
}

bool semanticParseExportMATRIX()
{
    logger.log("semanticParseExportMATRIX");
    //Matrix should exist
    if (matrixCatalogue.isMatrix(parsedQuery.exportMatrixName))
        return true;
    cout << "SEMANTIC ERROR: No such matrix exists" << endl;
    return false;
}

void executeExportMATRIX()
{
    logger.log("executeExportMATRIX");
    Matrix* matrix = matrixCatalogue.getMatrix(parsedQuery.exportMatrixName);
    matrix->makePermanent();
    return;
}