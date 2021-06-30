
#include "BMPImage.h"
#include "stb_image.h"
#include "IRGB.h"
#include <vector>
#include <iostream>

BMPImage::BMPImage(unsigned char* image_data, int width, int height, int channels) {
	rgbValues = new RGB*[width * height];
	this->width = width;
	this->height = height;
	this->channels = channels;

	size_t img_size = width * height * channels;
	for (unsigned char* point = image_data; point < (image_data + img_size); point += channels) {
		int x = ((point - image_data) / channels) % width;
		int y = ((point - image_data) / channels) / width;
		rgbValues[x * height + y] = new RGB(*point, *(point + 1), *(point + 2));
	}

	stbi_image_free(image_data);
}

BMPImage::BMPImage(int width, int height, int channels) {
	rgbValues = new RGB*[width * height];
	this->width = width;
	this->height = height;
	this->channels = channels;

	for (int i = 0; i < width * height; i++) {
		rgbValues[i] = new RGB();
	}

}

void BMPImage::setRGB(int x, int y, RGB* rgb) {
	if (x < 0) std::cout << "x was " << x << std::endl;
	else if (y < 0) std::cout << "y was " << y << std::endl;
	else if (x * height + y >= width * height) std::cout << "out of bounds: (" << x << "," << y << ")" << std::endl;
	else {
		rgbValues[x * height + y] = rgb;
		//std::cout << "(" << x << "," << y << ")" << std::endl;
	}
}

RGB* BMPImage::getRGB(int x, int y) {
	if (x < 0 || x >= (width * height)) std::cout << "x was " << x << std::endl;
	if (y < 0 || y >= (width * height)) std::cout << "y was " << y << std::endl;
	return rgbValues[x * height + y];
}

unsigned char* BMPImage::serialize() {
	size_t img_size = width * height * channels;
	uint8_t* image_data = new uint8_t[img_size];
	for (int i = 0; i < img_size; i += channels) {
		int x = (i / channels) % width;
		int y = (i / channels) / width;
		RGB* rgb = rgbValues[x * height + y];
		if (rgb == nullptr) {
			std::cout << "rgb was null for (" << x << "," << y << ")" << std::endl;
			continue;
		}
		image_data[i] = rgb->getRed();
		image_data[i + 1] = rgb->getGreen();
		image_data[i + 2] = rgb->getBlue();
	}
	return image_data;
}

void BMPImage::clean() {
	for (int i = 0; i < width * height; i++) {
		delete rgbValues[i];
	}
	delete[] rgbValues;
	std::cout << "cleared bmpimage" << std::endl;
}
