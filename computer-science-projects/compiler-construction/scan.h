/*
 * scan.h
 *
 *  Created on: Jan 27, 2013
 *      Author: Haleeq
 */

#ifndef SCAN_H_
#define SCAN_H_

#include "globals.h"

/* function getToken returns the
 * next token in source file
 */
void getToken(void);

HANDLE hIn = INVALID_HANDLE_VALUE, hOut = INVALID_HANDLE_VALUE;
HANDLE hInMap = NULL, hOutMap = NULL;
LPTSTR pIn = NULL, pInFile = NULL, pOut = NULL, pOutFile = NULL;
LPCTSTR fIn = NULL;


#endif /* SCAN_H_ */
