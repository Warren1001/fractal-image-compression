
#include <string>
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
#include "ResizableBMPImage.h"
#include "ProbableAffineTransformation.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

void printUsage();
void printInvalidCommand(std::string input);
std::vector<std::string> split(std::string s, std::string delimiter);
IFSCode deserializeIFSCode(std::string serialized_ifs);
std::list<ProbableAffineTransformation> deserializeProbableAffineTransformations(std::string serialized_ifs);
void compressImage(std::string input, std::string output, IFSCode ifsCode);
void generateFractalImage(std::string output, int iterations, int scale, std::list<ProbableAffineTransformation> transformations, RGB* rgb);

int main() {

	IFSCode DEFAULT_COMPRESSION_IFS_ONE = IFSCode({
		AffineTransformation(0.85, 0.04, -0.04, 0.85, 0, 1.6) });

	IFSCode DEFAULT_COMPRESSION_IFS_TWO = IFSCode({
		AffineTransformation(0.85, 0.04, -0.04, 0.85, 0, 1.6),
		AffineTransformation(0.2, -0.26, 0.23, 0.22, 0, 1.6) });

	IFSCode DEFAULT_COMPRESSION_IFS_THREE = IFSCode({
		AffineTransformation(0.85, 0.04, -0.04, 0.85, 0, 1.6),
		AffineTransformation(0.2, -0.26, 0.23, 0.22, 0, 1.6),
		AffineTransformation(-0.15, 0.28, 0.26, 0.24, 0, 0.44) });

	std::list<ProbableAffineTransformation> BARNSLEY_FERN_TRANSFORMATIONS = {
		ProbableAffineTransformation(0, 0, 0, 0.16, 0, 0, 1),
		ProbableAffineTransformation(0.85, 0.04, -0.04, 0.85, 0, 1.6, 85),
		ProbableAffineTransformation(0.2, -0.26, 0.23, 0.22, 0, 1.6, 7),
		ProbableAffineTransformation(-0.15, 0.28, 0.26, 0.24, 0, 0.44, 7) };

	RGB* BARNSLEY_FERN_COLOR = new RGB(34, 139, 34);

	char again = 'y';

	printUsage();

	while (again == 'y' || again == 'Y') {

		std::string input;
		getline(std::cin, input);

		if (input.rfind("compress", 0) == 0) {

			std::vector<std::string> args = split(input, " ");

			if (args.size() != 4) printInvalidCommand(input);
			else {

				std::string file_name_input = args[1];
				std::string file_name_output = args[2];
				std::string serialized_ifs = args[3];

				if (serialized_ifs == "one" || serialized_ifs == "1") compressImage(file_name_input, file_name_output, DEFAULT_COMPRESSION_IFS_ONE);
				else if (serialized_ifs == "two" || serialized_ifs == "2") compressImage(file_name_input, file_name_output, DEFAULT_COMPRESSION_IFS_TWO);
				else if (serialized_ifs == "three" || serialized_ifs == "3") compressImage(file_name_input, file_name_output, DEFAULT_COMPRESSION_IFS_THREE);
				else {

					IFSCode ifsCode = deserializeIFSCode(serialized_ifs);

					if (ifsCode.empty()) printInvalidCommand(input);
					else compressImage(file_name_input, file_name_output, ifsCode);

				}

			}

		}
		else if (input.rfind("generate", 0) == 0) {

			std::vector<std::string> args = split(input, " ");

			if (args.size() >= 4) {

				std::string file_name_output = args[1];
				std::string iterations_str = args[2];
				int iterations = std::stoi(iterations_str);

				if (args[3] == "default") generateFractalImage(file_name_output, iterations, 100, BARNSLEY_FERN_TRANSFORMATIONS, BARNSLEY_FERN_COLOR);
				else if (args[4] == "default") {
					std::string scale_str = args[3];
					int scale = std::stoi(scale_str);
					generateFractalImage(file_name_output, iterations, scale, BARNSLEY_FERN_TRANSFORMATIONS, BARNSLEY_FERN_COLOR);
				}
				else if (args[5] == "default") {
					std::string scale_str = args[3];
					int scale = std::stoi(scale_str);
					std::string rgb_str = args[4];
					std::vector<std::string> rgb_args = split(rgb_str, ",");
					if (rgb_args.size() != 3) printInvalidCommand(input);
					else {
						int r = std::stoi(rgb_args[0]);
						int g = std::stoi(rgb_args[1]);
						int b = std::stoi(rgb_args[2]);
						generateFractalImage(file_name_output, iterations, scale, BARNSLEY_FERN_TRANSFORMATIONS, new RGB(r, g, b));
					}
				}
				else if (args.size() == 6) {

					std::string scale_str = args[3];
					int scale = std::stoi(scale_str);
					std::string rgb_str = args[4];
					std::string serialized_ifs = args[5];
					std::vector<std::string> rgb_args = split(rgb_str, ",");
					std::list<ProbableAffineTransformation> transformations = deserializeProbableAffineTransformations(serialized_ifs);

					if (rgb_args.size() != 3 || transformations.empty()) printInvalidCommand(input);
					else {

						int r = std::stoi(rgb_args[0]);
						int g = std::stoi(rgb_args[1]);
						int b = std::stoi(rgb_args[2]);

						generateFractalImage(file_name_output, iterations, scale, transformations, new RGB(r, g, b));

					}

				}
				else printInvalidCommand(input);
			}
			else printInvalidCommand(input);

		}
		else if (input == "usage") printUsage();
		else if (input == "stop") again = 'n';
		else printInvalidCommand(input);

	}
	
	return 0;
}

void printUsage() {
	std::cout << "Usage: compress [file_name_input].bmp [file_name_output].bmp one" << std::endl;
	std::cout << "Usage: compress [file_name_input].bmp [file_name_output].bmp two" << std::endl;
	std::cout << "Usage: compress [file_name_input].bmp [file_name_output].bmp three" << std::endl;
	std::cout << "Usage: compress [file_name_input].bmp [file_name_output].bmp [x1,y1,x2,y2,xc,yc;x1,y1,x2,y2,xc,yc;...]" << std::endl;
	std::cout << "Usage: generate [file_name_output].bmp [num_interations] default" << std::endl;
	std::cout << "Usage: generate [file_name_output].bmp [num_interations] [scale] default" << std::endl;
	std::cout << "Usage: generate [file_name_output].bmp [num_interations] [scale] [r,g,b] default" << std::endl;
	std::cout << "Usage: generate [file_name_output].bmp [num_interations] [scale] [r,g,b] [x1,y1,x2,y2,xc,yc,p;x1,y1,x2,y2,xc,yc,p;...]" << std::endl;
	std::cout << "Usage: usage" << std::endl;
	std::cout << "Usage: stop" << std::endl;
}

void printInvalidCommand(std::string input) {
	std::cout << "'" << input << "' is not a valid command, please use the following:" << std::endl;
	printUsage();
}

std::vector<std::string> split(std::string s, std::string delimiter) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}

IFSCode deserializeIFSCode(std::string serialized_ifs) {
	std::vector<std::string> ifs_args = split(serialized_ifs, ";");
	std::list<AffineTransformation> transformations;
	for (std::string ifs_arg : ifs_args) {
		std::vector<std::string> serialized_transformation = split(ifs_arg, ",");
		if (serialized_transformation.size() == 6) {
			double x1 = std::stod(serialized_transformation[0]);
			double y1 = std::stod(serialized_transformation[1]);
			double x2 = std::stod(serialized_transformation[2]);
			double y2 = std::stod(serialized_transformation[3]);
			double xc = std::stod(serialized_transformation[4]);
			double yc = std::stod(serialized_transformation[5]);
			transformations.push_back(AffineTransformation(x1, y1, x2, y2, xc, yc));
		}
		else {
			return IFSCode();
		}
	}
	return IFSCode(transformations);
}

std::list<ProbableAffineTransformation> deserializeProbableAffineTransformations(std::string serialized_ifs) {
	std::vector<std::string> ifs_args = split(serialized_ifs, ";");
	std::list<ProbableAffineTransformation> transformations;
	for (std::string ifs_arg : ifs_args) {
		std::vector<std::string> serialized_transformation = split(ifs_arg, ",");
		if (serialized_transformation.size() == 7) {
			double x1 = std::stod(serialized_transformation[0]);
			double y1 = std::stod(serialized_transformation[1]);
			double x2 = std::stod(serialized_transformation[2]);
			double y2 = std::stod(serialized_transformation[3]);
			double xc = std::stod(serialized_transformation[4]);
			double yc = std::stod(serialized_transformation[5]);
			int p = std::stoi(serialized_transformation[6]);
			transformations.push_back(ProbableAffineTransformation(x1, y1, x2, y2, xc, yc, p));
		}
		else {
			return std::list<ProbableAffineTransformation>();
		}
	}
	return transformations;
}

void compressImage(std::string input, std::string output, IFSCode ifsCode) {

	int width, height, channels;
	uint8_t* source_data = stbi_load(input.c_str(), &width, &height, &channels, 3);
	BMPImage source = BMPImage(source_data, width, height, channels);
	ResizableBMPImage dest = ResizableBMPImage(channels);

	for (int curr_x = 0; curr_x < width; curr_x++) {
		for (int curr_y = 0; curr_y < height; curr_y++) {

			double x = curr_x;
			double y = curr_y;
			RGB* source_rgb = source.getRGB(curr_x, curr_y);

			ifsCode.transform(&x, &y);

			dest.setRGB((int)x, (int)y, source_rgb);

		}

	}

	unsigned char* dest_data = dest.serialize(false);
	stbi_write_bmp(output.c_str(), dest.getWidth(), dest.getHeight(), channels, dest_data);

	delete[] dest_data;
	dest.clean();
	source.clean();

	std::cout << "Finished compressing '" << input << "'." << std::endl;
}

void generateFractalImage(std::string output, int iterations, int scale, std::list<ProbableAffineTransformation> transformations, RGB* rgb) {

	ResizableBMPImage fractal_image = ResizableBMPImage(3);
	double x = 0.0;
	double y = 0.0;

	for (int i = 0; i < iterations; i++) {

		int r = rand() % 100 + 1;

		for (ProbableAffineTransformation transformation : transformations) {
			r -= transformation.getProbability();
			if (r <= 0 && i > 10) {
				transformation.transform(&x, &y);
				if (i > 10) fractal_image.setRGB((int)(x * scale), (int)(y * scale), rgb);
				break;
			}
		}

	}

	unsigned char* fractal_data = fractal_image.serialize(true);
	stbi_write_bmp(output.c_str(), fractal_image.getWidth(), fractal_image.getHeight(), 3, fractal_data);

	delete[] fractal_data;
	fractal_image.clean();

	std::cout << "Finished generating image '" << output << "'." << std::endl;
}
