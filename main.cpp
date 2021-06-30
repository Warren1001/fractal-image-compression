
#include <iostream>
#include <stdint.h>
#include <math.h>
#include <cstdint>
#include "AffineTransformation.h"
#include "IFSCode.h"
#include "BMPImage.h"
#include "AveragedRGB.h"

//#define STBI_ONLY_BMP
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

template<typename T, typename K>
inline bool isType(const K &k) {
	return typeid(T).hash_code() == typeid(k).hash_code();
}

int main() {

	IFSCode ifsCode = IFSCode({ AffineTransformation(0.5, 0.25, 0.25, 0.5, 1, 2) });
	int width, height, channels;
	uint8_t* source_data = stbi_load("image.bmp", &width, &height, &channels, 3);
	//std::cout << width << "," << height << "," << channels << std::endl;
	//std::cout << stbi_failure_reason() << std::endl;

	BMPImage source = BMPImage(source_data, width, height, channels);

	BMPImage dest = BMPImage(width * 2, height * 2, channels);

	RGB empty = RGB();

	for (int i = 0; i < width * 2; i++) {
		for (int o = 0; o < height * 2; o++) {
			dest.setRGB(i, o, empty);
		}
	}

	for (int curr_x = 0; curr_x < width; curr_x++) {
		for (int curr_y = 0; curr_y < height; curr_y++) {

			int x = curr_x;
			int y = curr_y;
			RGB source_rgb = source.getRGB(curr_x, curr_y);

			//if (source_rgb.empty()) continue;

			ifsCode.transform(&x, &y);
			RGB dest_rgb = dest.getRGB(x, y);

			if (dest_rgb.empty()) {
				dest.setRGB(x, y, source_rgb);
			}
			else {
				if (isType<AveragedRGB>(dest_rgb)) {
					AveragedRGB averaged_rgb = dynamic_cast<AveragedRGB&>(dest_rgb);
					averaged_rgb.addRGB(source_rgb);
				}
				else {
					dest.setRGB(x, y, AveragedRGB(source_rgb));
				}
			}

			std::cout << "(" << curr_x << "," << curr_y << ")->(" << x << "," << y << ")=" << dest_rgb.getRed() << "," << dest_rgb.getGreen() << "," << dest_rgb.getBlue() << std::endl;

		}
	}

	unsigned char* dest_data = dest.serialize();
	stbi_write_bmp("output.bmp", width, height, channels, dest_data);

	delete[] dest_data;
	dest.clean();
	source.clean();
	return 0;
}
