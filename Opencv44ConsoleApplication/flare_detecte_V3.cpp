#include "stdafx.h"
#include "flare_detecte_V3.h"


flare_detecte_V3::flare_detecte_V3()
{
}


flare_detecte_V3::~flare_detecte_V3()
{
}

int flare_detecte_V3::my_flare_detecte_V3(Mat image, int edgeDZ_distance, int long_DZ_length,int shortDZ_nums, int fat_radius, int inside_dust)
{
	//判断参数
	int edge_DZ = edgeDZ_distance;//600
	int long_DZ = long_DZ_length;//450
	int fat_judge = fat_radius;//400
	int dust_area = inside_dust;//1000
	int shortDZ = shortDZ_nums;//3

	Mat size_img;
	resize(image, size_img, Size(), 0.5, 0.5);
	Mat hsv_img, gray;
	cvtColor(size_img, hsv_img, COLOR_BGR2HSV);

	//cvtColor(img, gray, COLOR_BGR2GRAY);
	//hsv通道分离
	vector<Mat> hsv_channels;
	split(hsv_img, hsv_channels);
	//V通道
	Mat image_Vchannel;
	image_Vchannel = hsv_channels.at(2);
	//中值滤波
	Mat median_image;
	medianBlur(image_Vchannel, median_image, 115);
	//均值图像
	Mat mean_image;
	//blur(median_image, mean_image, Size(115, 115), Point(-1, -1), 4);
	GaussianBlur(median_image, mean_image, Size(125, 125), 45,45, 4);
	//阈值图像
	Mat dyn_image;
	adaptiveThreshold(mean_image, dyn_image, 255, 0, THRESH_BINARY, 135, 0);
	Mat se1 = getStructuringElement(MORPH_RECT, Size(31, 31), Point(-1, -1));
	//Mat se2 = getStructuringElement(MORPH_RECT, Size(5, 10), Point(-1, -1));
	Mat result;
	morphologyEx(dyn_image, result, MORPH_OPEN, se1);

	Point centers;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(result, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());
	for (size_t t = 0; t < contours.size(); t++)
	{

		if (contours[t].size() > 300)
		{
			// 最小外接轮廓
			RotatedRect rrt = minAreaRect(contours[t]);
			float w = rrt.size.width;
			float h = rrt.size.height;
			//cout << "外接轮廓的宽:" << w << endl;
			//cout << "外接轮廓的高:" << h << endl;
			centers = rrt.center;
			//光源发胖判定
			if (min(w, h) > fat_judge) return 1;
			//cout << "镜头中心" << centers << endl;
			//ellipse(src, rrt, Scalar(0, 0, 255), 2, 8);
			//ellipse(threshold_img_7, rrt, (255), 2, 8);
			//circle(src, Point(centers), 70, Scalar(0, 0, 0), -1, LINE_8, 0);
			float radius = min(w, h) / 2 + 16;
			//float radius1 = radius + 20;
			circle(result, centers, radius, Scalar(0, 0, 0), -1, LINE_8, 0);
			//circle(src, centers, radius1, Scalar(0, 0, 0), -1, LINE_8, 0);
		}
	}
	int flag = DZ_detect3(result, centers, long_DZ, edge_DZ, shortDZ_nums,dust_area);


	return flag;
}

int flare_detecte_V3::DZ_detect3(Mat img, Point pt, int long_DZ, int edge_DZ,int shortDZ,int dust_area)
{
	Mat threshold_img;
	threshold(img, threshold_img, 160, 255, THRESH_BINARY);

	Mat binary_img;
	Mat k = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	erode(threshold_img, binary_img, k, Point(-1, -1), 1, 0);
	Mat k2 = getStructuringElement(MORPH_ELLIPSE, Size(5, 5), Point(-1, -1));
	dilate(binary_img, binary_img, k2, Point(-1, -1), 1, 0);

	Mat DZ_img = img.clone();
	int nums_DZ = 0;
	//查找目标区域
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(binary_img, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	double area = 0;
	for (size_t t = 0; t < contours.size(); t++)
	{
		RotatedRect rrt = minAreaRect(contours[t]);
		//道子的角度
		float angle = rrt.angle;
		//道子中心
		Point2f center = rrt.center;
		//cout << "道子外接矩形的角度:" << angle << endl;
		//道子面积
		//如果面积过大直接判内尘
		double areai = contourArea(contours[t]);
		//cout << "待测区域的面积：" << areai << endl;
		//if (areai > 300) return 2;
		float height = rrt.size.height;
		float weight = rrt.size.width;
		float rate;
		//纵横比
		rate = weight / height;
		//求矩形质心和灯珠连线的角度
		float k = 0;
		k = (pt.y - center.y) / (pt.x - center.x);
		float line_arctan = atan(k);
		//角度计算
		double angle1 = line_arctan*57.3;
		if ((angle1 > 0 && rate < 0.66) || (angle1 <= 0 && rate > 1.5))
		{
			if (angle1 > 0) angle1 = angle1 - 90;


			//cout << "道子中心与圆心的角度:" << angle1 << endl;
			float angle_diff = fabs(angle1 - angle);



			//如果角度差小于3，且纵横比大于1.5则判断为道子
			if (angle_diff < 3)
			{
				//是否为边缘道子。计算道子中心与圆心的距离
				double distance = sqrt(pow((pt.y - center.y), 2) + pow((pt.x - center.x), 2));
				if (distance > long_DZ) return 5;
				//是否为长道子
				if (max(height, weight) > long_DZ) return 3;
				//是否为多道子
				nums_DZ++;
				if (nums_DZ > shortDZ) return 4;

				//Point2f pts[4];
				//rrt.points(pts);
				//for (int i = 0; i < 4; i++)
				//{
				//	line(DZ_img, pts[i % 4], pts[(i + 1) % 4], Scalar(0, 0, 255), 2, 8, 0);
				//}
				////内尘面积判断}
			}
			else 
			{
				area += areai;
				//cout << "内尘面积" << area<<endl;
				if (area > dust_area) return 2;
			}
		}
		else
		{
			area += areai;
			//cout << "内尘面积" << area << endl;
			//double area = contourArea(contours[t]);
			if (area > dust_area) return 2;
		}
	}

	return 0;
}
