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

	//Sử dụng hình chữ nhật 8x16 trượt trên toàn bộ ảnh để khử đi các nhiễu ko cần thiết

	int cnt;
	int nonZero1, nonZero2, nonZero3, nonZero4;
	CvRect rect;

	for (int i = 0; i < mini_thresh->width - 32; i+=4)
	{
		for (int j = 0; j < mini_thresh->height - 16; j+=4)
		{
			rect = cvRect(i, j, 16, 8);
			cvSetImageROI(mini_thresh, rect);
			nonZero1 = cvCountNonZero(mini_thresh);
			cvResetImageROI(mini_thresh);

			rect = cvRect(i+16, j, 16, 8);
			cvSetImageROI(mini_thresh, rect);
			nonZero2 = cvCountNonZero(mini_thresh);
			cvResetImageROI(mini_thresh);

			rect = cvRect(i, j+8, 16, 8);
			cvSetImageROI(mini_thresh, rect);
			nonZero3 = cvCountNonZero(mini_thresh);
			cvResetImageROI(mini_thresh);

			rect = cvRect(i+16, j+8, 16, 8);
			cvSetImageROI(mini_thresh, rect);
			nonZero4 = cvCountNonZero(mini_thresh);
			cvResetImageROI(mini_thresh);

			cnt = 0;
			if (nonZero1 > 15) { cnt++; }
			if (nonZero2 > 15) { cnt++; }
			if (nonZero3 > 15) { cnt++; }
			if (nonZero4 > 15) { cnt++; }

		
			if (cnt > 2)
			{
				rect = cvRect(i, j, 32, 16);
				cvSetImageROI(dst, rect);
				cvSetImageROI(mini_thresh, rect);
				cvCopy(mini_thresh, dst);
				cvResetImageROI(dst);
				cvResetImageROI(mini_thresh);

			}
		}
	}
	IplImage* dst_clone = cvCloneImage(dst);

	cvDilate(dst, dst, NULL, 2);
	cvErode(dst, dst, NULL, 2);
	cvDilate(dst, dst, S1, 9);
	cvErode(dst, dst, S1, 10);
	cvDilate(dst, dst);

	cvShowImage("Source", src);
	cvShowImage("mImg", mImg);
	cvShowImage("mini_thresh", mini_thresh);
	cvShowImage("dst", dst);
	cvShowImage("dst_clone", dst_clone);

	cvPyrUp(dst, src);

	cvReleaseImage(&src);
	cvReleaseImage(&tmp);
	cvReleaseImage(&mini_thresh);
	cvReleaseImage(&mImg);
	cvReleaseImage(&dst);
	cvReleaseImage(&dst_clone);
	cvReleaseImage(&thresholded);
	cvReleaseImage(&src_pyrdown);


}
