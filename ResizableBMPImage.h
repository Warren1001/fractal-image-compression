#ifndef RESIZABLEBMPIMAGE
#define RESIZABLEBMPIMAGE

#include <vector>
#include <list>
#include "RGB.h"
#include "RGBLocation.h"

class ResizableBMPImage {
protected:
	std::vector<RGBLocation> rgbLocations;
	int width, height, channels;
public:
	ResizableBMPImage(int channels);
	void setRGB(int x, int y, RGB* rgb);
	int getWidth();
	int getHeight();
	unsigned char* serialize(bool invert_y);
	void clean();
};

#endif
