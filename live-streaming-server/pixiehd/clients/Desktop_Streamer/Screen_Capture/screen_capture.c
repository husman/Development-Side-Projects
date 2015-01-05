// Helper function to retrieve current position of file pointer:
inline int GetFilePointer(HANDLE FileHandle){
	return SetFilePointer(FileHandle, 0, 0, FILE_CURRENT);
}
//---------------------------------------------------------------------------

/**
 * Saves a screenshot of the screen
*/
int SaveBMPFile(char *filename, HBITMAP bitmap, HDC bitmapDC, int width, int height)
{
	HBITMAP OffscrBmp=NULL; // bitmap that is converted to a DIB
	HDC OffscrDC=NULL;      // offscreen DC that we can select OffscrBmp into
	LPBITMAPINFO lpbi=NULL; // bitmap format info; used by GetDIBits
	LPVOID lpvBits=NULL;    // pointer to bitmap bits array
	HANDLE BmpFile=INVALID_HANDLE_VALUE;    // destination .bmp file
	BITMAPFILEHEADER bmfh;  // .bmp file header

	// We need an HBITMAP to convert it to a DIB:
	if ((OffscrBmp = CreateCompatibleBitmap(bitmapDC, width, height)) == NULL)
		return 0;

	// The bitmap is empty, so let's copy the contents of the surface to it.
	// For that we need to select it into a device context. We create one.
	if ((OffscrDC = CreateCompatibleDC(bitmapDC)) == NULL)
		return 0;

	// Select OffscrBmp into OffscrDC:
	HBITMAP OldBmp = (HBITMAP)SelectObject(OffscrDC, OffscrBmp);

	// Now we can copy the contents of the surface to the offscreen bitmap:
	BitBlt(OffscrDC, 0, 0, width, height, bitmapDC, 0, 0, SRCCOPY);

	// GetDIBits requires format info about the bitmap. We can have GetDIBits
	// fill a structure with that info if we pass a NULL pointer for lpvBits:
	// Reserve memory for bitmap info (BITMAPINFOHEADER + largest possible
	// palette):
	lpbi = (LPBITMAPINFO)malloc(sizeof(char)*sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD));
	if (lpbi == NULL)
		return 0;


	ZeroMemory(&lpbi->bmiHeader, sizeof(BITMAPINFOHEADER));
	lpbi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	// Get info but first de-select OffscrBmp because GetDIBits requires it:
	SelectObject(OffscrDC, OldBmp);
	if (!GetDIBits(OffscrDC, OffscrBmp, 0, height, NULL, lpbi, DIB_RGB_COLORS))
		return 0;

	// Reserve memory for bitmap bits:
	lpvBits = (LPVOID)malloc(sizeof(char)*lpbi->bmiHeader.biSizeImage);
	if (lpvBits == NULL)
		return 0;

	// Have GetDIBits convert OffscrBmp to a DIB (device-independent bitmap):
	if (!GetDIBits(OffscrDC, OffscrBmp, 0, height, lpvBits, lpbi, DIB_RGB_COLORS))
		return 0;

	// Create a file to save the DIB to:
	if ((BmpFile = CreateFile(filename,
			GENERIC_WRITE,
			0, NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL)) == INVALID_HANDLE_VALUE)

		return 0;

	DWORD Written;    // number of bytes written by WriteFile

	// Write a file header to the file:
	bmfh.bfType = 19778;        // 'BM'
	// bmfh.bfSize = ???        // we'll write that later
	bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
	// bmfh.bfOffBits = ???     // we'll write that later
	if (!WriteFile(BmpFile, &bmfh, sizeof(bmfh), &Written, NULL))
		return 0;

	if (Written < sizeof(bmfh))
		return 0;

	// Write BITMAPINFOHEADER to the file:
	if (!WriteFile(BmpFile, &lpbi->bmiHeader, sizeof(BITMAPINFOHEADER), &Written, NULL))
		return 0;

	if (Written < sizeof(BITMAPINFOHEADER))
		return 0;

	// Calculate size of palette:
	int PalEntries;
	// 16-bit or 32-bit bitmaps require bit masks:
	if (lpbi->bmiHeader.biCompression == BI_BITFIELDS)
		PalEntries = 3;
	else
		// bitmap is palettized?
		PalEntries = (lpbi->bmiHeader.biBitCount <= 8) ?
				// 2^biBitCount palette entries max.:
				(int)(1 << lpbi->bmiHeader.biBitCount)
				// bitmap is TrueColor -> no palette:
				: 0;
	// If biClrUsed use only biClrUsed palette entries:
	if(lpbi->bmiHeader.biClrUsed)
		PalEntries = lpbi->bmiHeader.biClrUsed;

	// Write palette to the file:
	if(PalEntries){
		if (!WriteFile(BmpFile, &lpbi->bmiColors, PalEntries * sizeof(RGBQUAD), &Written, NULL))
			return 0;

		if (Written < PalEntries * sizeof(RGBQUAD))
			return 0;
	}

	// The current position in the file (at the beginning of the bitmap bits)
	// will be saved to the BITMAPFILEHEADER:
	bmfh.bfOffBits = GetFilePointer(BmpFile);

	// Write bitmap bits to the file:
	if (!WriteFile(BmpFile, lpvBits, lpbi->bmiHeader.biSizeImage, &Written, NULL))
		return 0;

	if (Written < lpbi->bmiHeader.biSizeImage)
		return 0;

	// The current pos. in the file is the final file size and will be saved:
	bmfh.bfSize = GetFilePointer(BmpFile);

	// We have all the info for the file header. Save the updated version:
	SetFilePointer(BmpFile, 0, 0, FILE_BEGIN);
	if (!WriteFile(BmpFile, &bmfh, sizeof(bmfh), &Written, NULL))
		return 0;

	if (Written < sizeof(bmfh))
		return 0;

	return 1;
}

/**
 * This function will store the color pixels of the desktop in YUV format.
 * Each color value will be written to a separate file. They are written to
 * Separate files for quick debugging/experiments.
*/
int ScreenCapture(int x, int y, int width, int height, char *filename)
{
	BYTE* bitPointer;
	int red, green, blue;
	int Y, U, V;
	FILE *output_file_y, *output_file_u, *output_file_v;
	int i, z;

	HDC hdc = GetDC(0);
	// get a DC compat. w/ the screen
	HDC hDc = CreateCompatibleDC(hdc);
	
	/* to save to a BITMAP file */
	// make a bmp in memory to store the capture in
	//HBITMAP hBmp = CreateCompatibleBitmap(GetDC(0), width, height);

	BITMAPINFO bitmap;
	bitmap.bmiHeader.biSize = sizeof(bitmap.bmiHeader);
	bitmap.bmiHeader.biWidth = width;
	bitmap.bmiHeader.biHeight = -height;
	bitmap.bmiHeader.biPlanes = 1;
	bitmap.bmiHeader.biBitCount = 32;
	bitmap.bmiHeader.biCompression = BI_RGB;
	bitmap.bmiHeader.biSizeImage = width * 4 * height;
	bitmap.bmiHeader.biClrUsed = 0;
	bitmap.bmiHeader.biClrImportant = 0;

	HBITMAP hBitmap2 = CreateDIBSection(hDc, &bitmap, DIB_RGB_COLORS, (void**)(&bitPointer), NULL, 0);
	SelectObject(hDc, hBitmap2);
	output_file_y = fopen("result_y.yuv", "wb");
	output_file_u = fopen("result_u.yuv", "wb");
	output_file_v = fopen("result_v.yuv", "wb");

	z = 0;
	int row = 0, col = 0;

	for(z=24; z<25; z++) {
		BitBlt(hDc, 0, 0, width, height, GetDC(0), 0, 0, SRCCOPY);
		row = 0, col = 0;
		for (i=0; i<(width * 4 * height); i+=4)
		{
			red = (int)bitPointer[i+2];
			green = (int)bitPointer[i+1];
			blue = (int)bitPointer[i];
			
			// do we have an alpha channel?
			//alpha = (int)bitPointer[i+3];

			// Single Luminance value
			Y = 0.299 * red + 0.587 * green + 0.114 * blue;
			fprintf(output_file_y, "%c", (unsigned char)Y);
			
			// Chrominance values
			if(row % 2 == 0 && col % 2 == 0) {
				U = -0.1687 * red - 0.3313* green + 0.5 * blue + 128;
				V = 0.5 * red - 0.4187 * green - 0.813 * blue + 128;
				fprintf(output_file_u, "%c", (unsigned char)U);
				fprintf(output_file_v, "%c", (unsigned char)V);
			}
			col++;
		}
	}
	
	/* to save to a BITMAP file */
	// join em up
	//SelectObject(hDc, hBmp);
	// copy from the screen to my bitmap
	//BitBlt(hDc, 0, 0, width, height, GetDC(0), x, y, SRCCOPY);
	// save my bitmap
	//int ret = SaveBMPFile(filename, hBmp, hDc, width, height);
	// free the bitmap memory
	//DeleteObject(hBmp);
	
	DeleteObject(hBitmap2);
	
	fclose(output_file_y);
	fclose(output_file_u);
	fclose(output_file_v);

	return 1;
}
