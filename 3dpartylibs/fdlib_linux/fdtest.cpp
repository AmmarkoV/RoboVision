#include "loadbmp.h" // from http://gpwiki.org/index.php/LoadBMPCpp
#include "fdlib.h"

int main(int argc, char *argv[]) 
{
	int i, n, x[256], y[256], size[256], w, h, threshold;	
	BMPImg *bi;
	unsigned char *bgrdata, *graydata;
	
	if (argc==1)
	{
	    printf("usage: fdtest bmpfilename [threshold]\n");
	    exit(0);
	}
		
	bi = new BMPImg();
	printf("\nloading %s\n", argv[1]);
	bi->Load(argv[1]);
	w = bi->GetWidth();
	h = bi->GetHeight();
	printf("image is %dx%d pixels\n", w, h);
	bgrdata = bi->GetImg();
	graydata = new unsigned char[w*h];
	
	for (i=0; i<w*h; i++)
	    graydata[i] = (unsigned char) ((.11*bgrdata[3*i] + .59*bgrdata[3*i+1] + .3*bgrdata[3*i+2]));
	
	
	threshold = argc>2 ? atoi(argv[2]) : 0;
	printf("detecting with threshold = %d\n", threshold);
	fdlib_detectfaces(graydata, w, h, threshold);	
		
	n = fdlib_getndetections();
	if (n==1)
	    printf("\n%d face found.\n", n);
	else
 	    printf("\n%d faces found.\n", n);		
	
	for (i=0; i<n; i++)
	{
	    fdlib_getdetection(i, x+i, y+i, size+i);
	    printf("x:%d y:%d size:%d\n", x[i], y[i], size[i]);
	}
	
	delete[] graydata;
	delete bi;
}
