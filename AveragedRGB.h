#ifndef AVERAGEDRGB
#define AVERAGEDRGB

#include "RGB.h"
#include <vector>

class AveragedRGB : public RGB {
private:
	std::vector<RGB> rgbList;
	bool calculated = false;
public:
	AveragedRGB(RGB rgb) : RGB(0, 0, 0) {
		rgbList.push_back(rgb);
	}

	AveragedRGB() {
		std::cout << "the defualt constructor was called ------------------------------------------" << std::endl;
	}

	~AveragedRGB() {
		rgbList.clear();
	}

	void addRGB(RGB rgb) {
		calculated = false;
		rgbList.push_back(rgb);
	}

	int getRed() {
		if (!calculated) calculate();
		return r;
	}

	int getGreen() {
		if (!calculated) calculate();
		return g;
	}

	int getBlue() {
		if (!calculated) calculate();
		return b;
	}

	void calculate() {
		r = 0;
		g = 0;
		b = 0;
		for (RGB rgb : rgbList) {
			r += rgb.getRed();
			g += rgb.getGreen();
			b += rgb.getBlue();
		}
		r /= rgbList.size();
		g /= rgbList.size();
		b /= rgbList.size();
		calculated = true;
	}

};

#endif
