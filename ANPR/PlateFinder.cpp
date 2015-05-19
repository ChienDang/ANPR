#include "PlateFinder.h"


PlateFinder::PlateFinder()
{
	S1 = cvCreateStructuringElementEx(3, 1, 1, 0, CV_SHAPE_RECT, NULL);
	S2 = cvCreateStructuringElementEx(6, 1, 3, 0, CV_SHAPE_RECT, NULL);
	plate = NULL;
}


PlateFinder::~PlateFinder()
{
	if (plate)
	{
		S1 = NULL;
		S2 = NULL;
		plate = NULL;
	}
}

void PlateFinder::ImageRestoration(IplImage *src)
{
	int w = src->width;
	int h = src->height;

	IplImage *mImg = cvCreateImage(cvSize(w / 2, h / 2), IPL_DEPTH_8U, 1); // Ảnh sử dụng cho biến đổi hình thái học
	IplImage *src_pyrdown = cvCreateImage(cvSize(w / 2, h / 2), IPL_DEPTH_8U, 1);
	IplImage *tmp = cvCreateImage(cvSize(w / 2, h / 2), IPL_DEPTH_8U, 1);
	IplImage *thresholded = cvCreateImage(cvSize(w / 2, h / 2), IPL_DEPTH_8U, 1);
	IplImage *mini_thresh = cvCreateImage(cvSize(w / 2, h / 2), IPL_DEPTH_8U, 1);
	IplImage *dst = cvCreateImage(cvSize(w / 2, h / 2), IPL_DEPTH_8U, 1); // Ảnh đã làm rõ vùng biển số

	cvPyrDown(src, src_pyrdown);
	//Biến đổi hình thái học
	cvMorphologyEx(src_pyrdown, mImg, tmp, S2, CV_MOP_BLACKHAT);
	cvNormalize(mImg, mImg, 0, 255, CV_MINMAX);

	//Nhị phân hóa ảnh mImg
	cvThreshold(mImg, thresholded, (int)10 * cvAvg(mImg).val[0], 255, CV_THRESH_BINARY);
	cvZero(dst);
	cvCopy(thresholded, mini_thresh);

	cvShowImage("Source", src);
	cvShowImage("mImg", mImg);
	cvShowImage("mini_thresh", mini_thresh);


}
