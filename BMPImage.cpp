
#include "BMPImage.h"
#include "stb_image.h"
#include "IRGB.h"
#include <vector>
#include <iostream>

BMPImage::BMPImage(unsigned char* image_data, int width, int height, int channels) {
	rgbValues = new RGB[width * height];
	this->width = width;
	this->height = height;
	this->channels = channels;

	size_t img_size = width * height * channels;
	std::cout << width << "," << height << "," << channels << std::endl;
	for (unsigned char* point = image_data; point < (image_data + img_size); point += channels) {
		std::cout << "we made it 2" << std::endl;
		int x = ((point - image_data) / channels) % width;
		int y = ((point - image_data) / channels) / width;
		rgbValues[x * height + y] = RGB(*point, *(point + 1), *(point + 2));
		std::cout << "setting to (" << x << "," << y << ") to " << *point << "," << *(point + 1) << "," << *(point + 2) << std::endl;
	}

	stbi_image_free(image_data);
}

BMPImage::BMPImage(int width, int height, int channels) {
	rgbValues = new RGB[width * height];
	this->width = width;
	this->height = height;
	this->channels = channels;
}

void BMPImage::setRGB(int x, int y, RGB rgb) {
	rgbValues[x * height + y] = rgb; // todo changed to width, might change back
}

RGB BMPImage::getRGB(int x, int y) {
	return rgbValues[x * height + y]; // todo changed to width, might change back
}

unsigned char* BMPImage::serialize() {
	size_t img_size = width * height * channels;
	uint8_t* image_data = new uint8_t[img_size];
	for (int i = 0; i < img_size; i += channels) {
		int x = (i / channels) % width;
		int y = (i / channels) / width;
		RGB rgb = rgbValues[x * height + y];
		image_data[i] = rgb.getRed();
		image_data[i + 1] = rgb.getGreen();
		image_data[i + 2] = rgb.getBlue();
	}
	return image_data;
}

void BMPImage::clean() {
	delete[] rgbValues;
}
