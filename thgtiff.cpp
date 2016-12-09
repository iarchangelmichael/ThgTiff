#include "thgtiff.h"

int main(int args, char* arg[]){
	//char *file = "THG_Gl_wR_1200.tiff";

	if(args != 2){
		printf("Usage: thgtiff file.tiff.\r\n");
		return 0;
	}

	//int r = TiffToBmp(arg[1], (char*)"result.bmp");

	int r = TiffToLight(arg[1], (char*)"result.bmp");

	return 0;
}
