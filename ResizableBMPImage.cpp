
#include <limits.h>
#include "ResizableBMPImage.h"
#include "RGBLocation.h"
#include "BMPImage.h"
#include <iostream>

ResizableBMPImage::ResizableBMPImage(int channels) {
	width = 0;
	height = 0;
	this->channels = channels;
}

void ResizableBMPImage::setRGB(int x, int y, RGB* rgb) {
	rgbLocations.push_back(RGBLocation(x, y, rgb));
}

int ResizableBMPImage::getWidth() {
	return width;
}

int ResizableBMPImage::getHeight() {
	return height;
}

unsigned char* ResizableBMPImage::serialize(bool invert_y) {

	int sx = INT_MAX;
	int sy = INT_MAX;
	int lx = INT_MIN;
	int ly = INT_MIN;

	for (RGBLocation loc : rgbLocations) {
		int x = loc.getX();
		int y = loc.getY();
		if (x < sx) sx = x;
		else if (x > lx) lx = x;
		if (y < sy) sy = y;
		else if (y > ly) ly = y;
	}

	width = lx - sx + 1;
	height = ly - sy + 1;
	BMPImage image = BMPImage(width, height, channels);

	for (RGBLocation loc : rgbLocations) {
		int y = loc.getY() - sy;
		if (invert_y) y = height - y;
		image.setRGB(loc.getX() - sx, y, loc.getRGB());
	}

	unsigned char* image_data = image.serialize();

	image.clean();

	return image_data;
}

void ResizableBMPImage::clean() {
	rgbLocations.clear();
}
