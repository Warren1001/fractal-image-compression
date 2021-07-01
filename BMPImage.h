#ifndef BMPIMAGE
#define BMPIMAGE

#include <vector>
#include <list>
#include "RGB.h"

class BMPImage {
protected:
	RGB** rgbValues;
	int width, height, channels;
public:
	BMPImage(unsigned char* image_data, int width, int height, int channels);
	BMPImage(int width, int height, int channels);
	void setRGB(int x, int y, RGB* rgb);
	RGB* getRGB(int x, int y);
	unsigned char* serialize();
	void clean();
};

#endif
