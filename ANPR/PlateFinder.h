#pragma once

#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <opencv\cxcore.h>
#include <iostream>
class PlateFinder
{
private:
	IplConvKernel* S1;
	IplConvKernel* S2;
	IplImage *plate;
public:
	PlateFinder();
	virtual ~PlateFinder();
	void ImageRestoration(IplImage *src); // Hàm tiền xử lý ảnh gốc
};

