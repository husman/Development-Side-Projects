#include "globals.h"

int openFile(LPCTSTR filename)
{
	LARGE_INTEGER fileSize;

	/* Open the input file. */
	fHandle = CreateFile (filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fHandle == INVALID_HANDLE_VALUE) {
		MessageBox(NULL, "Failure opening input file.", "File I/O Error", MB_OK);
		return -1;
	}

	/* Get the input file size. */
	if (!GetFileSizeEx(fHandle, &fileSize)) {
		MessageBoxA(NULL, "Failure getting file size.", "File I/O Error", MB_OK);
		return -1;
	}
	/* This is a necessary, but NOT sufficient, test for mappability on 32-bit systems.
	 * Also see the long comment a few lines below */
	if (fileSize.HighPart > 0 && sizeof(SIZE_T) == 4) {
		MessageBoxA(NULL, "This file is too large to map on a Win32 system.", "File I/O Error", MB_OK);
		return -1;
	}

	/* Create a file mapping object on the input file. Use the file size. */
	fMapHandle = CreateFileMapping (fHandle, NULL, PAGE_READONLY, 0, 0, NULL);
	if (fMapHandle == NULL) {
		MessageBoxA(NULL, "Failure Creating input map.", "File I/O Error", MB_OK);
		return -1;
	}

	/* Map the input file */
	/* Comment: This may fail for large files, especially on 32-bit systems
	 * where you have, at most, 3 GB to work with (of course, you have much less
	 * in reality, and you need to map two files.
	 * This program works by mapping the input and output files in their entirity.
	 * You could enhance this program by mapping one block at a time for each file,
	 * much as blocks are used in the ReadFile/WriteFile implementations. This would
	 * allow you to deal with very large files on 32-bit systems. I have not taken
	 * this step and leave it as an exercise.
	 */
	pFileData = MapViewOfFile(fMapHandle, FILE_MAP_READ, 0, 0, 0);
	if (pFileData == NULL) {
		MessageBoxA(NULL, "Failure Mapping input file.", "File I/O Error", MB_OK);
		return -1;
	}

	pFileDataEnd = pFileData + fileSize.QuadPart;

	return 0;
}

void closeFile()
{
	/* Close all views and handles. */
	UnmapViewOfFile(pFileData);
	CloseHandle(fHandle);
	CloseHandle(fMapHandle);
}
