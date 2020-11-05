#include "stdafx.h"
#include "fat_light_detect.h"


fat_light_detect::fat_light_detect()
{
}


fat_light_detect::~fat_light_detect()
{
}

int fat_light_detect::fat_detect(Mat img)
{
	Mat gray_img = img;
	//如果不是灰度图，就转成灰度图
	if (gray_img.channels() != 1)
	{
		cvtColor(gray_img, gray_img, COLOR_BGR2GRAY);
	}
	//计算直方图

	Mat threshold_img;
	//阈值分割
	threshold(gray_img, threshold_img, 210, 255,  THRESH_BINARY);
	Mat edge;
	Canny(threshold_img,edge, 80, 240, 3, false);
	//去噪声
	Mat dilate_img;
	Mat k = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	dilate(edge, dilate_img, k);
	//拟合原
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(dilate_img, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());
	for (size_t t = 0; t < contours.size(); t++)
	{
		if (contours[t].size() > 50)
		{
			// 最小外接轮廓
			RotatedRect rrt = fitEllipse(contours[t]);
			float w = rrt.size.width;
			float h = rrt.size.height;
			if (h > 120) return 1;
			cout << "外接轮廓的宽:" << w << endl;
			cout << "外接轮廓的高:" << h << endl;
			Point centers = rrt.center;
			cout << "椭圆中心" << centers << endl;
			ellipse(gray_img, rrt, Scalar(0, 0, 255), 2, 8);
			
		}
	}
	//imshow("gray_img", gray_img);
	return 0;
}
