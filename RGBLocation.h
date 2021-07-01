#ifndef RGBLOCATION
#define RGBLOCATION

#include "RGB.h"

class RGBLocation {
private:
	int x, y;
	RGB* rgb;
public:
	RGBLocation(int x, int y, RGB* rgb) {
		this->x = x;
		this->y = y;
		this->rgb = rgb;
	}

	int getX() {
		return x;
	}

	int getY() {
		return y;
	}

	RGB* getRGB() {
		return rgb;
	}

};

#endif
