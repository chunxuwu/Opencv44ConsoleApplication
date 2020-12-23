#include "stdafx.h"
#include "flare_detecte.h"


flare_detecte::flare_detecte()
{
}


flare_detecte::~flare_detecte()
{
}

int flare_detecte::my_inside_dust_detect2(Mat source_img)
{
	Mat src=source_img,gray;
	//����ɫͼ�õ�
	cvtColor(source_img, gray, COLOR_BGR2GRAY);
	//ȫ�־�ֵ
	Scalar my_mean;
	my_mean = mean(source_img);
	//cout << "ȫ�־�ֵ��" << my_mean << endl;
	int mean_threshold;
	mean_threshold = 210;
	//ȫ����ֵ�ָ�
	Mat mean_img;
	mean_img = gray;
	//blur(source_img, mean_img,Size(3,3));
	Mat threshold_img_7;
	threshold(gray, threshold_img_7, mean_threshold, 255, THRESH_BINARY);
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
			cout << "��������Ŀ�:" << w << endl;
			cout << "��������ĸ�:" << h << endl;
			centers = rrt.center;
			//��Դ�����ж�
			if (min(w, h) > 130) return 1;
			cout << "��Բ����" << centers << endl;
			//ellipse(src, rrt, Scalar(0, 0, 255), 2, 8);
			//ellipse(threshold_img_7, rrt, (255), 2, 8);
			//circle(src, Point(centers), 70, Scalar(0, 0, 0), -1, LINE_8, 0);
			float radius = min(w, h)/2+7;
			float radius1 = radius + 20;
			circle(mean_img, centers, radius, Scalar(0, 0, 0), -1, LINE_8, 0);
			circle(src, centers, radius1, Scalar(0, 0, 0), -1, LINE_8, 0);
		}
	}
	//////����ڽ�Բ
	//Mat raw_dist(binary_img.size(), CV_32F);
	//for (int i = 0; i < binary_img.rows; i++)
	//{
	//	for (int j = 0; j < binary_img.cols; j++)
	//	{
	//		raw_dist.at<float>(i, j) = (float)pointPolygonTest(contours[0], Point2f((float)j, (float)i), true);
	//	}
	//}
	////��ȡ����ڽ�Բ�뾶
	//double minVal, maxVal;
	//Point maxDistPt;
	//minMaxLoc(raw_dist, &minVal, &maxVal, NULL, &maxDistPt);
	//minVal = abs(minVal);
	//maxVal = abs(maxVal);
	//maxVal += 7;
	//circle(mean_img, maxDistPt, (int)maxVal, Scalar(0, 0, 0), -1, LINE_8, 0);

	int flag = DZ_detect2(mean_img, centers);
	if (flag == 0)
	{
		flag = color_detect(src);
	}
	return flag;
}

//����ȥ�����Ĺ�Դ�Ķ�ֵͼ�񣬼�����
int flare_detecte::DZ_detect2(Mat img, Point pt)
{
	//cout << pt << endl;
	//��̬ѧ�ݶ�
	/*Mat basic;
	Mat se = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	morphologyEx(img, basic, MORPH_GRADIENT, se);*/
	Mat threshold_img;
	threshold(img, threshold_img, 160, 255, THRESH_BINARY);

	Mat binary_img;
	Mat k = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	erode(threshold_img, binary_img, k, Point(-1, -1), 1, 0);
	Mat k2 = getStructuringElement(MORPH_ELLIPSE, Size(5, 5), Point(-1, -1));
	dilate(binary_img, binary_img, k2, Point(-1, -1), 1, 0);

	Mat DZ_img = img.clone();
	int nums_DZ = 0;
	//����Ŀ������
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;//�洢���ҵ��ĵ�i�������ĺ�[i][0]��ǰ[i][1]����[i][2]��������[i][3]
							//double t = (double)getTickCount();
	findContours(binary_img, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	double area = 0;
	for (size_t t = 0; t < contours.size(); t++)
	{
		RotatedRect rrt = minAreaRect(contours[t]);
		//���ӵĽǶ�
		float angle = rrt.angle;
		//��������
		Point2f center = rrt.center;
		cout <<"������Ӿ��εĽǶ�:"<<angle << endl;
		//�������
		//����������ֱ�����ڳ�
		double areai = contourArea(contours[t]);
		cout << "��������������" << areai << endl;
		if (areai > 300) return 2;

		//��������ĺ͵������ߵĽǶ�
		float k = 0;
		k = (pt.y - center.y) / (pt.x - center.x);
		float line_arctan = atan(k);
		//�Ƕȼ���
		double angle1 = line_arctan*57.3;
		if (angle1 > 0) angle1 = angle1 - 90;
		cout << "����������Բ�ĵĽǶ�:"<<angle1<<endl;
		float angle_diff = fabs(angle1 - angle);

		float height = rrt.size.height;
		float weight = rrt.size.width;
		float rate = max(height, weight) / min(height, weight);
		//����ǶȲ�С��3�����ݺ�ȴ���1.5���ж�Ϊ����
		if (angle_diff < 3 && rate>1.5)
		{
			//�Ƿ�Ϊ��Ե���ӡ��������������Բ�ĵľ���
			double distance = sqrt(pow((pt.y - center.y), 2) + pow((pt.x - center.x), 2));
			if (distance > 150) return 3;
			//�Ƿ�Ϊ������

			if (max(height, weight) > 7) return 3;
			//�Ƿ�Ϊ�����
			nums_DZ++;
			if (nums_DZ > 4) return 3;

			Point2f pts[4];
			rrt.points(pts);
			for (int i = 0; i<4; i++)
			{
				line(DZ_img, pts[i % 4], pts[(i + 1) % 4], Scalar(0, 0, 255), 2, 8, 0);
			}
		}
		//�ڳ�����ж�
		else
		{
			area += areai;
			//double area = contourArea(contours[t]);
			if (area > 100) return 2;
		}
	}

	return 0;
}

int flare_detecte::color_detect(Mat img)
{
	Mat hsv, mask;
	cvtColor(img, hsv, COLOR_BGR2HSV);
	Mat se = getStructuringElement(MORPH_RECT, Size(5, 5), Point(-1, -1));
	inRange(hsv, Scalar(70, 70, 50), Scalar(90, 100, 100), mask);
	morphologyEx(mask, mask, MORPH_OPEN, se);
	
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(mask, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	size_t index = -1;
	double max = 0;
	for (size_t t = 0; t < contours.size(); t++) 
	{
		double area = contourArea(contours[t]);
		if (area > max) 
		{
			max = area;
			index = t;
		}
	}
	if (index >= 0)
	{
		RotatedRect rect = minAreaRect(contours[index]);
		ellipse(img, rect, Scalar(0, 255, 0), 2, 8);
		return 2;
	}
	return 0;
}
