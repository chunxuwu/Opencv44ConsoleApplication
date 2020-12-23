#include "stdafx.h"
#include "inside_dust_detect.h"


inside_dust_detect::inside_dust_detect()
{
}


inside_dust_detect::~inside_dust_detect()
{
}


int inside_dust_detect::my_inside_dust_detect(Mat source_img)
{
	Mat src;
	//����ɫͼ�õ�
	cvtColor(source_img, src, COLOR_GRAY2BGR);
	//ȫ�־�ֵ
	Scalar my_mean;
	my_mean = mean(source_img);
	//cout << "ȫ�־�ֵ��" << my_mean << endl;
	int mean_threshold;
	mean_threshold = 210;
	//ȫ����ֵ�ָ�
	Mat mean_img;
	mean_img = source_img;
	//blur(source_img, mean_img,Size(3,3));
	Mat threshold_img_7;
	threshold(source_img, threshold_img_7, mean_threshold, 255, THRESH_BINARY);
	//imshow("size_threshold_190", threshold_img_7);
	//������=�ȸ�ʴ������
	Mat binary_img;
	Mat k = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	erode(threshold_img_7, binary_img, k, Point(-1, -1), 1, 0);
	Mat k2 = getStructuringElement(MORPH_ELLIPSE, Size(9, 9), Point(-1, -1));
	dilate(binary_img, binary_img, k2, Point(-1, -1), 1, 0);
	//morphologyEx(threshold_img_7, binary_img, MORPH_OPEN, k);
	//morphologyEx(binary_img, binary_img, MORPH_CLOSE, k);
	Point centers;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(binary_img, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());
	for (size_t t = 0; t < contours.size(); t++)
	{
		if (contours[t].size() > 50)
		{
			// ��С�������
			RotatedRect rrt = minAreaRect(contours[t]);
			float w = rrt.size.width;
			float h = rrt.size.height;
			//��Դ�����ж�
			if (min(w, h) > 140) return 1;
			//cout << "��������Ŀ�:" << w << endl;
			//cout << "��������ĸ�:" << h << endl;
			centers = rrt.center;
			//cout << "��Բ����" << centers << endl;
			ellipse(src, rrt, Scalar(0, 0, 255), 2, 8);
			ellipse(threshold_img_7, rrt,(255), 2, 8);
			circle(src, Point(centers), 70, Scalar(0, 0, 0), -1, LINE_8, 0);
			//circle(mean_img, Point(centers), 70, Scalar(0, 0, 0), -1, LINE_8, 0);
		}
	}
	////����ڽ�Բ
	Mat raw_dist(binary_img.size(), CV_32F);
	for (int i = 0; i < binary_img.rows; i++)
	{
		for (int j = 0; j < binary_img.cols; j++)
		{
			raw_dist.at<float>(i, j) = (float)pointPolygonTest(contours[0], Point2f((float)j, (float)i), true);
		}
	}
	//��ȡ����ڽ�Բ�뾶
	double minVal, maxVal;
	Point maxDistPt;
	minMaxLoc(raw_dist, &minVal, &maxVal, NULL, &maxDistPt);
	minVal = abs(minVal);
	maxVal = abs(maxVal);
	maxVal += 7;
	circle(mean_img, maxDistPt, (int)maxVal, Scalar(0, 0, 0), -1, LINE_8, 0);

	int flag = DZ_detect(mean_img, centers);
	return flag;
}

//����ȥ�����Ĺ�Դ�Ķ�ֵͼ�񣬼�����
int inside_dust_detect::DZ_detect(Mat img, Point pt)
{
	//cout << pt << endl;
	Mat threshold_img;
	threshold(img, threshold_img, 160, 255, THRESH_BINARY);

	Mat binary_img;
	Mat k = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	erode(threshold_img, binary_img, k, Point(-1, -1), 1, 0);
	Mat k2 = getStructuringElement(MORPH_ELLIPSE, Size(5, 5), Point(-1, -1));
	dilate(binary_img, binary_img, k2, Point(-1, -1), 1, 0);

	Mat DZ_img = img.clone();
	int nums_DZ=0;
	//����Ŀ������
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;//�洢���ҵ��ĵ�i�������ĺ�[i][0]��ǰ[i][1]����[i][2]��������[i][3]
	//double t = (double)getTickCount();
	findContours(binary_img, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	for (size_t t = 0; t < contours.size(); t++)
	{
		RotatedRect rrt = minAreaRect(contours[t]);
		//���ӵĽǶ�
		float angle =rrt.angle;
		//��������
		Point2f center = rrt.center;
		//cout <<angle << endl;
		//�������
		//����������ֱ�����ڳ�
		double area = contourArea(contours[t]);
		if (area > 300) return 2;

		//��������ĺ͵������ߵĽǶ�
		float k=0;
		k = (pt.y - center.y) / (pt.x - center.x);
		float line_arctan = atan(k);
		//�Ƕȼ���
		double angle1 = line_arctan*57.3;
		if (angle1 > 0) angle1 = angle1 - 90;
		//cout << angle1<<endl;
		float angle_diff = fabs(angle1 - angle);

		float height = rrt.size.height;
		float weight = rrt.size.width;
		float rate = max(height, weight)/min(height,weight);
		//����ǶȲ�С��3�����ݺ�ȴ���1.5���ж�Ϊ����
		if (angle_diff < 3 && rate>1.5)
		{
			//�Ƿ�Ϊ��Ե���ӡ��������������Բ�ĵľ���
			double distance = sqrt(pow((pt.y - center.y), 2) + pow((pt.x - center.x), 2));
			if (distance > 150) return 3;
			//�Ƿ�Ϊ������
			
			if (max(height,weight) > 7) return 3;
			//�Ƿ�Ϊ�����
			nums_DZ++;
			if (nums_DZ > 4) return 3;

			Point2f pts[4];
			rrt.points(pts);
			for(int i=0;i<4;i++)
			{
				line(DZ_img, pts[i % 4], pts[(i + 1) % 4], Scalar(0, 0, 255), 2, 8, 0);
			}
		}
		//�ڳ�����ж�
		else
		{
			area += area;
			//double area = contourArea(contours[t]);
			if (area > 300) return 2;
		}
	}

	return 0;
}
