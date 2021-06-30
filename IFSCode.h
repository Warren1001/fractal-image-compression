#ifndef IFSCODE
#define IFSCODE

#include <vector>
#include <list>
#include "AffineTransformation.h"

class IFSCode {
protected:
	std::list<AffineTransformation> transformations;
public:
	IFSCode(const std::list<AffineTransformation>& transformations) {
		this->transformations = transformations;
	}

	void transform(int* x, int* y) {
		for (AffineTransformation transformation : transformations) {
			transformation.transform(x, y);
		}
	}

	void transform(double* x, double* y) {
		for (AffineTransformation transformation : transformations) {
			transformation.transform(x, y);
		}
	}

};

#endif
