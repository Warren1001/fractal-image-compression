#ifndef IRGBCLASS
#define IRGBCLASS

class IRGB {
public:
	virtual int getRed() = 0;
	virtual int getGreen() = 0;
	virtual int getBlue() = 0;
	virtual bool empty() = 0;
};

#endif
