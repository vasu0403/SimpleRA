#include "global.h"
/**
 * @brief
 * SYNTAX: LOAD MATRIX matrix_name
 */
bool syntacticParseTransposeMATRIX() {
    logger.log("syntacticParseTransposeMATRIX");
    if(tokenizedQuery.size() != 3) {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = TRANSPOSE_MATRIX;
    parsedQuery.transposeMatrixName = tokenizedQuery[2];
    return true;
}

bool semanticParseTransposeMATRIX() {
    logger.log("symanticParseTransposeMATRIX");
    if(!matrixCatalogue.isMatrix(parsedQuery.transposeMatrixName)) {
        cout << "SEMANTIC ERROR: Matrix does not exist" << endl;
        return false;
    }
    return true;
}

void executeTransposeMATRIX() {
    logger.log("executeTransposeMATRIX");
    Matrix *matrix = matrixCatalogue.getMatrix(parsedQuery.transposeMatrixName);
    matrix->transpose();
    return;
}