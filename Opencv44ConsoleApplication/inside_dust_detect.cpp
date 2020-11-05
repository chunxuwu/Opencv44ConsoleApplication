#include "stdafx.h"
#include "inside_dust_detect.h"


inside_dust_detect::inside_dust_detect()
{
}


inside_dust_detect::~inside_dust_detect()
{
}

void inside_dust_detect::my_inside_dust_detect(Mat source_img)
{
	Mat size_img;
	resize(source_img, size_img, Size(), 0.2, 0.2);
	//转为灰度图

	Mat gray_img;
	cvtColor(size_img, gray_img, COLOR_RGB2GRAY);

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
	threshold(gray_img, threshold_img_7, 190, 255, THRESH_BINARY);
	//imshow("size_threshold_190", threshold_img_7);
	//开运算和闭运算
	Mat binary_img;
	Mat k = getStructuringElement(MORPH_CROSS, Size(7, 7), Point(-1, -1));
	morphologyEx(threshold_img_7, binary_img, MORPH_OPEN, k);
	morphologyEx(binary_img, binary_img, MORPH_CLOSE, k);
	//imshow("binary_image", binary_img);

	Mat labels, stats, centroids;
	int num_labels = connectedComponentsWithStats(binary_img, labels, stats, centroids, 8, 4);
	//cout << num_labels << endl;

	//使用不同的颜色标记连通域
	vector<Vec3b> colors(num_labels);
	// background color
	colors[0] = Vec3b(110, 10, 60);
	// object color
	//
	RNG rng;
	for (int i = 1; i < num_labels; i++)
	{
		colors[i] = Vec3b(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
		//如果连通域小于100，则把连通域置为黑色背景。
		if (stats.at<int>(i - 1, CC_STAT_AREA) < 100)
			colors[i] = Vec3b(0, 0, 0);
	}

	// render result
	//Mat color_image = Mat::zeros(binary_img.size(), binary_img.type());
	//cout << binary_img.type() << endl;

	//利用统计信息标记连通域
	int area = 0;
	for (int i = 1; i < num_labels; i++)
	{
		Vec2d pt = centroids.at<Vec2d>(i, 0);
		int x = stats.at<int>(i, CC_STAT_LEFT);
		int y = stats.at<int>(i, CC_STAT_TOP);
		int width = stats.at<int>(i, CC_STAT_WIDTH);
		int height = stats.at<int>(i, CC_STAT_HEIGHT);
		area = area + stats.at<int>(i, CC_STAT_AREA);
		//printf("area : %d, center point(%.2f, %.2f)\n", area, pt[0], pt[1]);

		rectangle(binary_img, Rect(x, y, width, height), Scalar(255, 0, 255), 1, LINE_4, 0);
	}
	//imshow("color_image", binary_img);
	if (area > 8500)
	{
		cout << "this image is NG !!!" << endl;
		NG_nums++;
	}
	else
	{
		cout << "this image is OK !!!" << endl;
		OK_nums++;
	}
}
