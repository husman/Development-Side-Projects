/*
 *	BigInteger/headers/globals.h
 *
 *	Date:			Aug 31, 2012
 *	Author:			Haleeq Usman
 *	Dev. IDE:		Eclipse IDE for C/C++ Developers
 *		 			Version: Indigo Service Release 2
 *		 			Build id: 20120216-1857
 *	Dev. OS:		Windows 7 Home Premium SP1 (64-bitS)
 *	Dev. System:	Dell XPS 8300, 8GB RAM Intel(R) Core(TM)
 *					i7-2600 CPU @ 3.40Ghz (quad core at 3.40Ghz)
 *
 *	This file contains our static/global variables/constants,
 *	data structures (enum, structs, classes, etc...), and
 *	our project headers.
 *
 */

#ifndef GLOBALS_H_ // process only if GLOBALS_H_ constant is not defined
#define GLOBALS_H_ // prevent multiple processes by defining constant.
// Our standard input/output header: printf, etc...
#include <stdio.h>
// Contains input/ouput functions in the 'std' namespace
// ex: cout, cin, etc...
#include <iostream>
// Contains manipulation functions in the 'std' namespace for formatting
// ex: setprecision, etc...
#include <iomanip>
// Contains cross-platform data types: int8. uint8, int16, etc...
#include <stdint.h>
// We use this to measure how effective our algorithms are.
#include <time.h>
// Used in our threaded algorithm.
#include <windows.h>
// We use this for our BigInteger class
#include <vector>
// Let's include our BigInteger header
#include "BigInteger.h"
// We use this to reverse our BigInteger vector
#include <algorithm>


/* Prototypes for our functions */
void dec_to_binary32(void);

#endif /* GLOBALS_H_ */
