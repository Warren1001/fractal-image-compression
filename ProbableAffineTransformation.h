#ifndef PROBABLEAFFINETRANSFORMATION
#define PROBABLEAFFINETRANSFORMATION

#include "AffineTransformation.h"

class ProbableAffineTransformation : AffineTransformation {
public:
	int probability;
	ProbableAffineTransformation(double x1, double y1, double x2, double y2, double xc, double yc, int probability);
	void transform(int* x, int* y);
	void transform(double* x, double* y);
	int getProbability();
};

#endif
