
#include <iostream>
#include <iomanip>
#include <cstdlib>
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
	std::cout << "T:(" << typeid(T).hash_code() << ") - k:" << "(" << typeid(k).hash_code() << ")" << std::endl;
	return typeid(T).hash_code() == typeid(k).hash_code();
}

int main() {

	//IFSCode ifsCode = IFSCode({ AffineTransformation(0.5, 0.25, 0.25, 0.5, 1, 2) });
	IFSCode ifsCode = IFSCode({
		//AffineTransformation(0, 0, 0, 0.16, 0, 0),
		AffineTransformation(0.85, 0.04, -0.04, 0.85, 500, 1.6 + 500),
		AffineTransformation(0.2, -0.26, 0.23, 0.22, 500, 1.6 + 500),
		AffineTransformation(-0.15, 0.28, 0.26, 0.24, 500, 0.44 + 500) });
	int width, height, channels;
	uint8_t* source_data = stbi_load("image.bmp", &width, &height, &channels, 3);
	//std::cout << width << "," << height << "," << channels << std::endl;
	//std::cout << stbi_failure_reason() << std::endl;

	BMPImage source = BMPImage(source_data, width, height, channels);

	int d_width = width * 4;
	int d_height = height * 4;

	BMPImage dest = BMPImage(d_width, d_height, channels);

	for (int curr_x = 0; curr_x < width; curr_x++) {
		for (int curr_y = 0; curr_y < height; curr_y++) {

			int x = curr_x;
			int y = curr_y;
			RGB* source_rgb = source.getRGB(curr_x, curr_y);

			//if (source_rgb.empty()) continue;

			ifsCode.transform(&x, &y);
			RGB* dest_rgb = dest.getRGB(x, y);

			if (dest_rgb->empty()) {
				dest.setRGB(x, y, new RGB(*source_rgb));
				//std::cout << "dest_rgb was empty at (" << x << "," << y << ")" << std::endl;
			}
			else {
				if (dynamic_cast<AveragedRGB*>(dest_rgb) != nullptr) {
					AveragedRGB* averaged_rgb = dynamic_cast<AveragedRGB*>(dest_rgb);
					averaged_rgb->addRGB(*source_rgb);
					//std::cout << "adding to AveragedRGB at (" << x << "," << y << ")" << std::endl;
				}
				else {
					dest.setRGB(x, y, new AveragedRGB(*source_rgb));
					//std::cout << "setting an AveragedRGB at (" << x << "," << y << ")" << std::endl;
				}
			}

			//std::cout << "(" << curr_x << "," << curr_y << ")[" << source_rgb->getRed() << "," << source_rgb->getGreen() << "," << source_rgb->getBlue() << "]->(" << x << "," << y << ")[" << dest_rgb->getRed() << "," << dest_rgb->getGreen() << "," << dest_rgb->getBlue() << "]" << std::endl;

		}

		//std::cout << std::setprecision(3) << ((double)(curr_x + 1) / width) * 100 << "% Progress" << std::endl;

	}

	unsigned char* dest_data = dest.serialize();
	stbi_write_bmp("output.bmp", d_width, d_height, channels, dest_data);
	std::cout << "wrote compressed image" << std::endl;

	delete[] dest_data;
	dest.clean();
	source.clean();

	BMPImage fern_image = BMPImage(2001, 2001, 3);

	AffineTransformation ifsCodes[] = {
		AffineTransformation(0, 0, 0, 0.16, 0, 0 + 250),
		AffineTransformation(0.85, 0.04, -0.04, 0.85, 0, 1.6 + 250),
		AffineTransformation(0.2, -0.26, 0.23, 0.22, 0, 1.6 + 250),
		AffineTransformation(-0.15, 0.28, 0.26, 0.24, 0, 0.44 + 250) };

	double x = 0.0;
	double y = 0.0;

	int n = 10000000;

	for (int i = 0; i < n; i++) {
		int r = rand() % 100 + 1;
		if (r == 1) {
			ifsCodes[0].transform(&x, &y);
			fern_image.setRGB((int)x + 1000, 2000 - (int)y, new RGB(34, 139, 34));
		}
		else if (r <= 8) {
			ifsCodes[2].transform(&x, &y);
			fern_image.setRGB((int)x + 1000, 2000 - (int)y, new RGB(34, 139, 34));
		}
		else if (r <= 15) {
			ifsCodes[3].transform(&x, &y);
			fern_image.setRGB((int)x + 1000, 2000 - (int)y, new RGB(34, 139, 34));
		}
		else if (r <= 100) {
			ifsCodes[1].transform(&x, &y);
			fern_image.setRGB((int)x + 1000, 2000 - (int)y, new RGB(34, 139, 34));
		}
		//std::cout << std::setprecision(6) << ((double)(i + 1) / (n + 1)) * 100 << "% Progress" << std::endl;
	}

	unsigned char* fern_data = fern_image.serialize();
	stbi_write_bmp("fern.bmp", 2001, 2001, 3, fern_data);
	std::cout << "wrote fern image" << std::endl;

	delete[] fern_data;
	fern_image.clean();

	std::cout << "finished" << std::endl;
	return 0;
}
