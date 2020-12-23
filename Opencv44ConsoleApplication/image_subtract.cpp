#include "stdafx.h"
#include "image_subtract.h"


image_subtract::image_subtract()
{
}


image_subtract::~image_subtract()
{
}

void image_subtract::my_image_subtract(Mat &image1, Mat &image2)
{

		if ((image1.rows != image2.rows) || (image1.cols != image2.cols))
		{
			if (image1.rows > image2.rows)
			{
				resize(image1, image1, image2.size(), 0, 0, INTER_LINEAR);
			}
			else if (image1.rows < image2.rows)
			{
				resize(image2, image2, image1.size(), 0, 0, INTER_LINEAR);
			}
		}

		Mat image1_gary, image2_gary;
		if (image1.channels() != 1)
		{
			cvtColor(image1, image1_gary, COLOR_BGR2GRAY);
		}
		if (image2.channels() != 1)
		{
			cvtColor(image2, image2_gary, COLOR_BGR2GRAY);
		}

		Mat frameDifference, absFrameDifferece;
		Mat previousGrayFrame = image2_gary.clone();
		//ͼ1��ͼ2
		subtract(image1_gary, image2_gary, frameDifference, Mat(),-1);

		//ȡ����ֵ
		absFrameDifferece = abs(frameDifference);

		//λ��ĸı�
		absFrameDifferece.convertTo(absFrameDifferece, CV_8UC1, 1, 0);
		imshow("absFrameDifferece", absFrameDifferece);
		Mat segmentation;

		//��ֵ������һ���ܹؼ���Ҫ���ö�ֵ����ֵ��
		threshold(absFrameDifferece, segmentation, 70, 255, THRESH_BINARY);
		imshow("segmentation", segmentation);
		//��ֵ�˲�
		medianBlur(segmentation, segmentation, 3);

		//��̬ѧ����(��������)
		//��̬ѧ�����õ�������
		Mat morphologyKernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
		morphologyEx(segmentation, segmentation, MORPH_CLOSE, morphologyKernel, Point(-1, -1), 2, BORDER_REPLICATE);

		//��ʾ��ֵ��ͼƬ
		imshow("segmentation", segmentation);

		//�ұ߽�
		vector< vector<Point> > contours;
		vector<Vec4i> hierarchy;
		findContours(segmentation, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE, Point(0, 0));//CV_RETR_TREE
		vector< vector<Point> > contours_poly(contours.size());

		vector<Rect> boundRect;
		boundRect.clear();

		for (int index = 0; index < contours.size(); index++)
		{
			approxPolyDP(Mat(contours[index]), contours_poly[index], 3, true);
			Rect rect = boundingRect(Mat(contours_poly[index]));
			rectangle(image1, rect, Scalar(0, 255, 0), 2);
		}
		imshow("Ч��ͼ", image1);
	}

void image_subtract::min_rect_test(Mat &img)
{
	Mat background = Mat::zeros(img.size(), CV_8UC1);
	Rect rect(100, 100, 1300, 100);
	rectangle(background, rect, 255, -1);
	vector<vector<Point>>contours;
	findContours(background, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
	RotatedRect rrt = minAreaRect(contours[0]);
	float hv_eliminateDZ_Row = rrt.center.y;
	float hv_eliminateDZ_Column = rrt.center.x;
	float hv_eliminateDZ_Phi = rrt.angle;
	float rate = rrt.size.width/rrt.size.height;
	Point2f pts[4];
	rrt.points(pts);
	cout << rate << endl;
}

