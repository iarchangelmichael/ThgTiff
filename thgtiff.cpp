#include "thgtiff.h"

int main(){
	char *file = "THG_Gl_wR_1200.tiff";

	//int r = TiffToBmp(file, "THG_Gl_wR_1200.bmp");

	int r = TiffToLight(file, "THG_Gl_wR_1200.bmp");

	return 0;
}
