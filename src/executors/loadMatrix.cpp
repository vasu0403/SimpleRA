#include "global.h"
/**
 * @brief
 * SYNTAX: LOAD MATRIX matrix_name
 */
bool syntacticParseLoadMATRIX() {
    logger.log("syntacticParseLoadMatrix");
    if(tokenizedQuery.size() != 3) {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = LOAD_MATRIX;
    parsedQuery.loadMatrixName = tokenizedQuery[2];
    return true;
}

bool semanticParseLoadMATRIX() {
    logger.log("symanticParseLoadMatrix");
    if(matrixCatalogue.isMatrix(parsedQuery.loadMatrixName)) {
        cout << "SEMANTIC ERROR: Matrix already exists" << endl;
        return false;
    }
    if (!isFileExists(parsedQuery.loadMatrixName)) {
        cout << "SEMANTIC ERROR: Data file doesn't exist" << endl;
        return false;
    }
    return true;
}

void executeLoadMATRIX() {
    logger.log("executeLoadMATRIX");
    Matrix *matrix = new Matrix(parsedQuery.loadMatrixName);
    if(matrix->load()) {
        matrixCatalogue.insertMatrix(matrix);
        cout << "Loaded Matrix" << endl;
    }
    return;
}