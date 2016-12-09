#include <string.h>
#include <stdlib.h>

#include "tiffio.h"
#pragma comment(lib,"C:\\Program Files (x86)\\GnuWin32\\lib\\libtiff.lib")

// Typedef
typedef unsigned long       DWORD;
typedef long LONG;
typedef unsigned short      WORD;

#define __S_IREAD       0400    /* Read by owner.  */
#define __S_IWRITE      0200    /* Write by owner.  */
#define S_IREAD		__S_IREAD
#define S_IWRITE	__S_IWRITE

#ifndef O_BINARY
	#define O_BINARY	0
#endif

#ifdef __GNUC__
	#include <unistd.h>
	#include <fcntl.h>
#endif

#pragma pack(1)
// Bmp header
typedef struct tagBITMAPINFOHEADER{
        DWORD      biSize;
        LONG       biWidth;
        LONG       biHeight;
        WORD       biPlanes;
        WORD       biBitCount;
        DWORD      biCompression;
        DWORD      biSizeImage;
        LONG       biXPelsPerMeter;
        LONG       biYPelsPerMeter;
        DWORD      biClrUsed;
        DWORD      biClrImportant;
} BITMAPINFOHEADER, *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;

typedef struct tagBITMAPFILEHEADER {
        WORD    bfType;
        DWORD   bfSize;
        WORD    bfReserved1;
        WORD    bfReserved2;
        DWORD   bfOffBits;
} BITMAPFILEHEADER, *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;

#pragma pack()



int TiffToBmp(char *file, char *to){
	// Open Tiff
	TIFF* tif = TIFFOpen(file, "r");
    if(!tif){
		printf("Bad image.\r\n");
		return 0;
	}

	// Open File
	int bmpd = open(to, O_CREAT|O_RDWR|O_BINARY, S_IREAD | S_IWRITE);
	if(bmpd <= 0){
		printf("Fail to create bmp.\r\n");
		return 0;
	}

	// Get size
	unsigned int width, height;
	unsigned int twidth, theight;
	TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
	TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
	TIFFGetField(tif, TIFFTAG_TILEWIDTH, &twidth);
	TIFFGetField(tif, TIFFTAG_TILELENGTH, &theight);

	int bpp = 3;
	int ewidth = bpp * width + 4;
	int dsize = ewidth * height;

	// Bmp
	BITMAPINFOHEADER head;
	memset(&head, 0, sizeof(BITMAPINFOHEADER));
	head.biSize = sizeof(BITMAPINFOHEADER);
	head.biWidth = width;
	head.biHeight = height;
	head.biPlanes = 1;
	head.biBitCount = 8 * bpp;
	head.biSizeImage = dsize;

	// Bmp file header
	BITMAPFILEHEADER hdr;
	hdr.bfType = 0x4d42;
	hdr.bfSize = dsize + sizeof(BITMAPFILEHEADER);
	hdr.bfReserved1 = hdr.bfReserved2 = 0;
	hdr.bfOffBits = sizeof(BITMAPFILEHEADER) + head.biSize;

	// Write bmp header
	if(write(bmpd, &hdr, 14) != 14){
		printf("Fail to write(1) bmp.\r\n");
		return 0;
	}

	// Write bmp header
	if(write(bmpd, &head, sizeof(BITMAPINFOHEADER)) != sizeof(BITMAPINFOHEADER)){
		printf("Fail to write(2) bmp.\r\n");
		return 0;
	}

	// Tiff to RGBA
	unsigned int npixels = width * height;
	unsigned int *raster=(unsigned int*) _TIFFmalloc(npixels * sizeof(unsigned int));
	TIFFReadRGBAImage(tif, width, height, raster, 0); 
	
	//unsigned char *scanline = (unsigned char *)_TIFFmalloc(TIFFScanlineSize(tif));
    for(unsigned int y = 0; y < height; y ++){
        for(unsigned int x = 0; x < width; x ++){
			if(write(bmpd, &raster[x + y * width], 3) != 3){
				printf("Fail to write(3) bmp.\r\n");
				return 0;
			}
        }
    }

	// Free
	_TIFFfree(raster);
   // _TIFFfree(scanline);

	// Close
	close(bmpd);
	TIFFClose(tif);

	return 1;
}


int TiffToLightSearch(unsigned int *mat, unsigned int width, unsigned int height);

int TiffToLight(char *file, char *to){
	// Open Tiff
	TIFF* tif = TIFFOpen(file, "r");
    if(!tif){
		printf("Bad image.\r\n");
		return 0;
	}

	// Get size
	unsigned int width, height;
	unsigned int twidth, theight;
	TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
	TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
	TIFFGetField(tif, TIFFTAG_TILEWIDTH, &twidth);
	TIFFGetField(tif, TIFFTAG_TILELENGTH, &theight);

	// Tiff to RGBA
	unsigned int npixels = width * height;
	unsigned int *raster=(unsigned int*) _TIFFmalloc(npixels * sizeof(unsigned int));
	TIFFReadRGBAImage(tif, width, height, raster, 0); 

	// Open bmp
	int bmpd = open(to, O_CREAT|O_RDWR|O_BINARY, S_IREAD | S_IWRITE);
	if(bmpd <= 0){
		printf("Fail to create bmp.\r\n");
		return 0;
	}

	int bpp = 3;
	int ewidth = bpp * width + 4;
	int dsize = ewidth * height;

	// Bmp
	BITMAPINFOHEADER head;
	memset(&head, 0, sizeof(BITMAPINFOHEADER));
	head.biSize = sizeof(BITMAPINFOHEADER);
	head.biWidth = width;
	head.biHeight = height;
	head.biPlanes = 1;
	head.biBitCount = 8 * bpp;
	head.biSizeImage = dsize;

	// Bmp file header
	BITMAPFILEHEADER hdr;
	hdr.bfType = 0x4d42;
	hdr.bfSize = dsize + sizeof(BITMAPFILEHEADER);
	hdr.bfReserved1 = hdr.bfReserved2 = 0;
	hdr.bfOffBits = sizeof(BITMAPFILEHEADER) + head.biSize;

	// Write bmp header
	if(write(bmpd, &hdr, 14) != 14){
		printf("Fail to write(1) bmp.\r\n");
		return 0;
	}

	// Write bmp header
	if(write(bmpd, &head, sizeof(BITMAPINFOHEADER)) != sizeof(BITMAPINFOHEADER)){
		printf("Fail to write(2) bmp.\r\n");
		return 0;
	}

	// Matrix
	unsigned int *mat = new unsigned int[width * height];
	memset(mat, 0, width * height * sizeof(*mat));

	// Create matrix
    for(unsigned int y = 0; y < height; y ++){
        for(unsigned int x = 0; x < width; x ++){
			int r = TIFFGetR(raster[x + y * width]);
			int g = TIFFGetG(raster[x + y * width]);
			int b = TIFFGetB(raster[x + y * width]);

			if(r != g || g != b){
				printf("Error! Pixel color failed: R != G || G != B.\r\n");
				return 0;
			}

			if(r > 64){
				// Light found
				mat[x + y * width] = 1;

				raster[x + y * width] &= 0xff0000;
			}

			if(write(bmpd, &raster[x + y * width], 3) != 3){
				printf("Fail to write(3) bmp.\r\n");
				return 0;
			}
        }
    }

	// Search lights
	while(TiffToLightSearch(mat, width, height));

	// Free
	delete[] mat;

	// Free
	_TIFFfree(raster);

	// Close
	close(bmpd);
	TIFFClose(tif);

	return 1;
}

// Find Next light pixel
int TiffToLightFoundNext(unsigned int *mat, unsigned int x, unsigned int y, unsigned int width, unsigned int height){
	if(mat[x + y * width] != 1)
		return 0;
	
	// Set as used
	mat[x + y * width] = 3;

	int count = 1;

	// Next pixels
	if(x > 0)
		count += TiffToLightFoundNext(mat, x - 1, y, width, height);
	if(x < width - 1)
		count += TiffToLightFoundNext(mat, x + 1, y, width, height);
	if(y > 0)
		count += TiffToLightFoundNext(mat, x, y - 1, width, height);
	if(y < height - 1)
		count += TiffToLightFoundNext(mat, x, y + 1, width, height);

	return count;
}

// Clear this light
int TiffToLightFoundClear(unsigned int *mat, unsigned int x, unsigned int y, unsigned int width, unsigned int height){
	if(mat[x + y * width] != 3)
		return 0;
	
	// Set as used
	mat[x + y * width] = 0;

	int count = 1;

	// Next pixels
	if(x > 0)
		count += TiffToLightFoundClear(mat, x - 1, y, width, height);
	if(x < width - 1)
		count += TiffToLightFoundClear(mat, x + 1, y, width, height);
	if(y > 0)
		count += TiffToLightFoundClear(mat, x, y - 1, width, height);
	if(y < height - 1)
		count += TiffToLightFoundClear(mat, x, y + 1, width, height);

	return count;
}

// Light pixel found
int TiffToLightFound(unsigned int *mat, unsigned int x, unsigned int y, unsigned int width, unsigned int height){
	int count = TiffToLightFoundNext(mat, x, y, width, height);
	TiffToLightFoundClear(mat, x, y, width, height);

	printf("Light found! First: [%d, %d], count: %d.\r\n", x, y, count);
	return 1;
}

// Search lights
int TiffToLightSearch(unsigned int *mat, unsigned int width, unsigned int height){
    for(unsigned int y = 0; y < height; y ++){
        for(unsigned int x = 0; x < width; x ++){
			// Light pixel found
			if(mat[x + y * width])
				TiffToLightFound(mat, x, y, width, height);		
        }
    }

	return 0;
}