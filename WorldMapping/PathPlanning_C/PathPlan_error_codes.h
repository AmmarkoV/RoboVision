#ifndef _PATHPLANCORE_ERROR_H
#define _PATHPLANCORE_ERROR_H

unsigned short const NO_ERROR=0;
unsigned short const OUT_OF_MEMORY_WHILE_CREATING_MATRIX=1;
unsigned short const OUT_OF_MEMORY_WHILE_CREATING_LIST=2;
unsigned short const TARGET_OUT_OF_BOUNDS=3;
unsigned short const SOURCE_OUT_OF_BOUNDS=4;
unsigned short const LAST_OPERATION_OUT_OF_BOUNDS=5;
unsigned short const REACHED_LIST_LIMIT=6;
unsigned short const INCORRECT_PARAMETERS=7;
unsigned short const SOURCE_AT_ZERO=8;
unsigned short const WORLDMATRIX_NOT_INITIALIZED=9;

// "ERRORS :P k kala.."
unsigned short const PATH_ESTABLISHED=100;
unsigned short const NO_PATH_ESTABLISHED=101;

#endif
