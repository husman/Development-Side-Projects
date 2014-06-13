#include "scan.h"

static int linepos = 0; /* current position in LineBuf */
static int EOF_flag = FALSE; /* corrects ungetNextChar behavior on EOF */

/* getNextChar fetches the next non-blank character
 from lineBuf, reading in a new line if lineBuf is
 exhausted */
int getNextChar() {
	if (pFileData < pFileDataEnd) {
		linepos++;
		return *(pFileData++);
	} else {
		EOF_flag = TRUE;
		return EOF;
	}
}

/* ungetNextChar backtracks one character
 in lineBuf */
void ungetNextChar() {
	if (!EOF_flag) {
		linepos--;
		pFileData--;
	}
}

/****************************************/
/* the primary function of the scanner  */
/****************************************/
/* function getToken returns the
 * next token in source file
 */
void getToken(void) {
	int c = getNextChar();
	printf("%c --> char\n", c);
} /* end getToken */
