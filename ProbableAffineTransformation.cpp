
#include "ProbableAffineTransformation.h"

ProbableAffineTransformation::ProbableAffineTransformation(double x1, double y1, double x2, double y2, double xc, double yc, int probability) : AffineTransformation(x1, y1, x2, y2, xc, yc) {
	this->probability = probability;
}

void ProbableAffineTransformation::transform(int* x, int* y) {
	int prevX = *x;
	int prevY = *y;
	*x = (int)(this->x1 * prevX + this->y1 * prevY + xc);
	*y = (int)(this->x2 * prevX + this->y2 * prevY + yc);
}

void ProbableAffineTransformation::transform(double* x, double* y) {
	double prevX = *x;
	double prevY = *y;
	*x = this->x1 * prevX + this->y1 * prevY + xc;
	*y = this->x2 * prevX + this->y2 * prevY + yc;
}

int ProbableAffineTransformation::getProbability() {
	return probability;
}
