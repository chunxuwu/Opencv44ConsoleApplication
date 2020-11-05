#include "stdafx.h"
#include "little_dust_detect.h"


little_dust_detect::little_dust_detect()
{
}


little_dust_detect::~little_dust_detect()
{
}

void little_dust_detect::little_dust(Mat img)
{
	//Mat resize_img;
	//resize(img, resize_img, Size(), 0.2, 0.2);
	//imshow("no_center_img", resize_img);
	Mat srcGrayImage;
	if (img.channels() == 3)
	{
		cvtColor(img, srcGrayImage, COLOR_BGR2GRAY);
	}
	else
	{
		img.copyTo(srcGrayImage);
	}
	vector<KeyPoint>detectKeyPoint;
	Mat keyPointImage1, keyPointImage2;

	SimpleBlobDetector::Params params;
	params.minArea = 500;
	params.maxArea = 20000;
	params.filterByArea = true;
	Ptr<SimpleBlobDetector> sbd = SimpleBlobDetector::create(params);
	sbd->detect(srcGrayImage, detectKeyPoint);
	drawKeypoints(img, detectKeyPoint, keyPointImage1, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	drawKeypoints(img, detectKeyPoint, keyPointImage2, Scalar(0, 0, 255), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	//imshow("src image", img);
	//imshow("keyPoint image1", keyPointImage1);
	Mat resize_img;
	resize(keyPointImage2, resize_img, Size(), 0.2, 0.2);
	imshow("keyPointImage1", resize_img);

	//imshow("keyPoint image2", keyPointImage2);
}
