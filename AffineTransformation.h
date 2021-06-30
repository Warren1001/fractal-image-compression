#ifndef AFFINETRANSFORMATION
#define AFFINETRANSFORMATION

#include <vector>
#include <list>

class AffineTransformation {
public:
	double x1, y1, x2, y2, xc, yc;
	AffineTransformation(double x1, double y1, double x2, double y2, double xc, double yc);
	void transform(int* x, int* y);
};

#endif
