
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
	*x = (int)(this->x1 * *x + this->y1 * *y + xc);
	*y = (int)(this->x2 * *x + this->y2 * *y + yc);
}