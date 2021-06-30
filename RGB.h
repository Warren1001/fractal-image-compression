#ifndef RGBCLASS
#define RGBCLASS

#include "IRGB.h"

class RGB : IRGB {
protected:
	int r, g, b;
	bool constructedEmpty;
public:
	RGB(int r, int g, int b) {
		this->r = r;
		this->g = g;
		this->b = b;
		constructedEmpty = false;
	}

	RGB() {
		r = 255;
		g = 255;
		b = 255;
		constructedEmpty = true;
	}

	RGB(const RGB& rgb) {
		r = rgb.r;
		g = rgb.g;
		b = rgb.b;
		constructedEmpty = false;
	}

	virtual ~RGB() {}

	int getRed() {
		return r;
	}

	int getGreen() {
		return g;
	}

	int getBlue() {
		return b;
	}

	bool empty() {
		return constructedEmpty || (r == 255 && g == 255 && b == 255);
	}
};

#endif
