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
	//Mat size_img;
	//resize(source_img, size_img, Size(), 0.2, 0.2);
	//转为灰度图

	//Mat gray_img;
	//cvtColor(source_img, gray_img, COLOR_RGB2GRAY);

	//全局均值
	Scalar my_mean;
	my_mean = mean(source_img);
	//cout << "全局均值：" << my_mean << endl;
	int mean_threshold;
	mean_threshold = my_mean[0] + 100;
	//imshow("gray_image", gray_img);
	//自适应阈值分割
	//Mat threshold_img_1;
	//threshold(gray_img, threshold_img_1, 100, 255, THRESH_BINARY);
	//imshow("binary_threshold_100", threshold_img_1);
	//Mat threshold_img_2;
	//threshold(gray_img, threshold_img_2, 90, 255, THRESH_BINARY);
	//imshow("binary_threshold_90", threshold_img_2);
	//Mat threshold_img_3;
	//threshold(gray_img, threshold_img_3, 130, 255, THRESH_BINARY);
	//imshow("binary_threshold_130", threshold_img_3);

	//Mat threshold_img_4;
	//threshold(gray_img, threshold_img_4, 140, 255, THRESH_BINARY);
	//imshow("size_threshold_140", threshold_img_4);

	//Mat threshold_img_5;
	//threshold(gray_img, threshold_img_5, 150, 255, THRESH_BINARY);
	//imshow("size_threshold_150", threshold_img_5);

	//Mat threshold_img_6;
	//threshold(gray_img, threshold_img_6, 160, 255, THRESH_BINARY);
	//imshow("size_threshold_160", threshold_img_6);

	Mat threshold_img_7;
	threshold(source_img, threshold_img_7, mean_threshold, 255, THRESH_BINARY);
	//imshow("size_threshold_190", threshold_img_7);
	//开操作=先腐蚀后膨胀
	Mat binary_img;
	Mat k = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	erode(threshold_img_7, binary_img, k, Point(-1, -1), 1, 0);
	Mat k2 = getStructuringElement(MORPH_ELLIPSE, Size(11, 11), Point(-1, -1));
	dilate(binary_img, binary_img, k2, Point(-1, -1), 1, 0);
	//morphologyEx(threshold_img_7, binary_img, MORPH_OPEN, k);
	//morphologyEx(binary_img, binary_img, MORPH_CLOSE, k);
	//imshow("binary_image", binary_img);

	Mat labels, stats, centroids;
	int num_labels = connectedComponentsWithStats(binary_img, labels, stats, centroids, 8, 4);
	//cout << "连通域总数：" << num_labels - 1<<endl;
	//cout << num_labels << endl;

	//使用不同的颜色标记连通域
	vector<Vec3b> colors(num_labels);	

	// background color
	colors[0] = Vec3b(255, 0, 0);
	// object color
	//
	RNG rng(12345);
	for (int i = 1; i < num_labels; i++)
	{
		colors[i] = Vec3b(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		//如果连通域小于100，则把连通域置为黑色背景。
		if (stats.at<int>(i , CC_STAT_AREA) < 50)
			colors[i] = Vec3b(0, 0, 0);
	}

	// render result
	//Mat color_image = Mat::zeros(binary_img.size(), binary_img.type());
	//cout << binary_img.type() << endl;

	//利用统计信息标记连通域
	int area = 0;
	Vec2d pt;
	for (int i = 1; i < num_labels; i++)
	{
	    pt = centroids.at<Vec2d>(i, 0);
		cout << pt[0] << endl << pt[1] << endl;
		int x = stats.at<int>(i, CC_STAT_LEFT);
		int y = stats.at<int>(i, CC_STAT_TOP);
		int width = stats.at<int>(i, CC_STAT_WIDTH);
		int height = stats.at<int>(i, CC_STAT_HEIGHT);
		area = stats.at<int>(i, CC_STAT_AREA);
		//printf("area : %d, center point(%.2f, %.2f)\n", area, pt[0], pt[1]);
		circle(binary_img, Point(pt[0], pt[1]), 65, Scalar(0, 0, 0), -1, LINE_8, 0);
		//circle(source_img, Point(pt[0], pt[1]), 60, Scalar(0, 0, 0), -1, LINE_8, 0);
		//rectangle(source_img, Rect(x, y, width, height), Scalar(255,0,0), 1, 8, 0);
		//rectangle(binary_img, Rect(x, y, width, height), Scalar(255, 0, 0), 1, 8, 0);
	}
	//imshow("color_image", binary_img);
	cout << pt << endl;
	int flag = DZ_detect(binary_img, pt);

	if (area > 11650)
	{
		//cout << "this image is NG !!!" << endl;
		return 2;
	}
	else
	{
		//cout << "this image is OK !!!" << endl;
		return 0;
	}



}

//输入去除中心光源的二值图像，检测道子
int inside_dust_detect::DZ_detect(Mat img, Vec2d pt)
{
	cout << pt << endl;
	Mat DZ_img = img.clone();
	int nums_DZ=0;
	//查找目标区域
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;//存储查找到的第i个轮廓的后[i][0]、前[i][1]、父[i][2]、子轮廓[i][3]
	//double t = (double)getTickCount();
	findContours(img, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
	for (size_t t = 0; t < contours.size(); t++)
	{
		RotatedRect rrt = minAreaRect(contours[t]);
		//道子的角度
		float angle =rrt.angle;
		//道子中心
		Point2f center = rrt.center;
		cout <<angle << endl;
		//求矩形质心和灯珠连线的角度
		float k=0;
		k = (pt[1] - center.y) / (pt[0] - center.x);
		float line_arctan = atan(k);
		//角度计算
		float angle1 = line_arctan*57.3;
		cout << angle1<<endl;
		float angle_diff = fabs(angle1 - angle);
		//如果角度差小于2，则判断为道子
		if (angle_diff < 2)
		{
			nums_DZ++;
		}
		Point2f pts[4];
		rrt.points(pts);
		for(int i=0;i<4;i++)
		{
			line(DZ_img, pts[i % 4], pts[(i + 1) % 4], Scalar(255, 255, 250), 2, 8, 0);
		}
	}

	return 0;
}
