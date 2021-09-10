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
    return true;
}

void executeLoadMATRIX() {
    logger.log("executeLoadMATRIX");
    cout << parsedQuery.loadMatrixName << endl;
    cout << "EXECUTING .." << endl;
    return;
}