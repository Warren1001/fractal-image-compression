
#include "AffineTransformation.h"

AffineTransformation::AffineTransformation(double x1, double y1, double x2, double y2, double xc, double yc) {
	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x2;
	this->y2 = y2;
	this->xc = xc;
	this->yc = yc;
}

void AffineTransformation::transform(int* x, int* y) {
	int prevX = *x;
	int prevY = *y;
	*x = (int)(this->x1 * prevX + this->y1 * prevY + xc);
	*y = (int)(this->x2 * prevX + this->y2 * prevY + yc);
}

void AffineTransformation::transform(double* x, double* y) {
	double prevX = *x;
	double prevY = *y;
	*x = this->x1 * prevX + this->y1 * prevY + xc;
	*y = this->x2 * prevX + this->y2 * prevY + yc;
}
