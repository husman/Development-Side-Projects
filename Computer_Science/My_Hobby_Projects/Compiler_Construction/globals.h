/*
 * globals.h
 *
 *  Created on: Jan 27, 2013
 *      Author: Haleeq
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <windows.h>
#include <excpt.h>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/* MAXRESERVED = the number of reserved words */
#define MAXRESERVED 43

typedef enum
    /* book-keeping tokens */
   {
	/* Scanner process tokens */
	ENDFILE_TKN, ERROR_TKN,

    /* C99 reserved words */
	AUTO_TKN,		ENUM_TKN,		RESTRICT_TKN,	UNSIGNED_TKN,
	BREAK_TKN,		EXTERN_TKN,		RETURN_TKN,		VOID_TKN,
	CASE_TKN,		FLOAT_TKN,		SHORT_TKN,		VOLATILE_TKN,
	CHAR_TKN,		FOR_TKN,		SIGNED_TKN,		WHILE_TKN,
	CONST_TKN,		GOTO_TKN,		SIZEOF_TKN,		_BOOL_TKN,
	CONTINUE_TKN,	IF_TKN,			STATIC_TKN,		_COMPLEX_TKN,
	DEFAULT_TKN,	INLINE_TKN,		STRUCT_TKN,		_IMAGINARY_TKN,
	DO_TKN,			INT_TKN,		SWITCH_TKN,
	DOUBLE_TKN,		LONG_TKN,		TYPEDEF_TKN,
	ELSE_TKN,		REGISTER_TKN,	UNION_TKN,

	/* Unic specfic tokens */
    THEN_TKN,	END_TKN,	REPEAT_TKN,	UNTIL_TKN,
    READ_TKN,	WRITE_TKN,

    /* multicharacter tokens */
    ID_TKN,		NUM_TKN,	STRING_TKN,

    /* special symbols */
    ASSIGN_TKN,		EQ_TKN,		LT_TKN,		PLUS_TKN,
    MINUS_TKN,		TIMES_TKN,	OVER_TKN,	LPAREN_TKN,
    RPAREN_TKN,		SEMI_TKN
   } TknType;

   /* File open/close */
   HANDLE fHandle, fMapHandle;
   LPTSTR pFileData, pFileDataEnd;
   int openFile(LPCTSTR filename);
   void closeFile(void);

   /* Scanner prototypes */
   int getNextChar(void);
   void ungetNextChar(void);

#endif /* GLOBALS_H_ */
