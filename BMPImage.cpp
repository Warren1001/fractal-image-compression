
#include "BMPImage.h"
#include "stb_image.h"
#include "IRGB.h"
#include <vector>
#include <iostream>
#include "AveragedRGB.h"

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
	RGB* current_rgb = getRGB(x, y);
	if (current_rgb->empty()) {
		rgbValues[x * height + y] = new RGB(*rgb);
	}
	else if (dynamic_cast<AveragedRGB*>(current_rgb) != nullptr) {
		AveragedRGB* averaged_rgb = dynamic_cast<AveragedRGB*>(current_rgb);
		averaged_rgb->addRGB(*rgb);
	}
	else {
		rgbValues[x * height + y] = new AveragedRGB(*rgb);
	}
}

RGB* BMPImage::getRGB(int x, int y) {
	return rgbValues[x * height + y];
}

unsigned char* BMPImage::serialize() {
	size_t img_size = width * height * channels;
	uint8_t* image_data = new uint8_t[img_size];
	for (int i = 0; i < img_size; i += channels) {
		int x = (i / channels) % width;
		int y = (i / channels) / width;
		RGB* rgb = rgbValues[x * height + y];
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
}
