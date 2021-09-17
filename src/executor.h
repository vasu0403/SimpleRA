#include"semanticParser.h"

void executeCommand();

void executeCLEAR();
void executeCROSS();
void executeDISTINCT();
void executeEXPORT();
void executeExportMATRIX();
void executeINDEX();
void executeJOIN();
void executeLIST();
void executeLOAD();
void executeLoadMATRIX();
void executePRINT();
void executePrintMATRIX();
void executePROJECTION();
void executeRENAME();
void executeSELECTION();
void executeSORT();
void executeSOURCE();
void executeTransposeMATRIX();

bool evaluateBinOp(int value1, int value2, BinaryOperator binaryOperator);
void printRowCount(int rowCount);