#include "global.h"

/**
 * @brief inserts a matrix into the matrix catalogue
 * 
 * @param matrix 
 *
 */
void MatrixCatalogue::insertMatrix(Matrix* matrix) {
    logger.log("MatrixCatalogue::insertMatrix");
    matrices[matrix->matrixName] = matrix;
}

void MatrixCatalogue::deleteMatrix(string matrixName) {
    logger.log("MatrixCatalogue::delteMatrix");
    this->matrices[matrixName]->unload();
    delete this->matrices[matrixName];
    this->matrices.erase(matrixName);
}

bool MatrixCatalogue::isMatrix(string matrixName) {
    logger.log("MatrixCatalogue::isMatrix");
    if(this->matrices.count(matrixName)) {
        return true;
    }
    return false;
}

void MatrixCatalogue::print() {
    logger.log("MatrixCatalogue::print");
    for(pair<string, Matrix*> matrix: this->matrices) {
        cout << matrix.first << endl;
    }
}

MatrixCatalogue::~MatrixCatalogue(){
    logger.log("MatrixCatalague::~MatrixCatalague"); 
    for(pair<string, Matrix*> matrix: this->matrices){
        matrix.second->unload();
        delete matrix.second;
    }
}