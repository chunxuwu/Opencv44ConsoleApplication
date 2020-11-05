#include "stdafx.h"
#include "subtract_center_area.h"


subtract_center_area::subtract_center_area()
{
}


subtract_center_area::~subtract_center_area()
{
}

Mat subtract_center_area::subtract_area(Mat source_img)
{
	Mat gray_img, edge_image, labels, stats, centroids;
	cvtColor(source_img, gray_img, COLOR_BGR2GRAY);
	threshold(gray_img, edge_image, 210, 255, THRESH_BINARY);
	//adaptiveThreshold(gray_img, edge_image, 255, BORDER_REPLICATE, THRESH_BINARY, 5, 3);
	Mat resize_edge;
	resize(edge_image, resize_edge, Size(), 0.2, 0.2);
	imshow("threshold_image", resize_edge);
	//开运算、闭运算
	//Mat binary_image;
	//Mat k = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	//morphologyEx(edge_image, binary_image, MORPH_OPEN, k);
	//morphologyEx(binary_image, binary_image, MORPH_CLOSE, k);
	//imshow("threshold_image", binary_image);

	int num_labels = connectedComponentsWithStats(edge_image, labels, stats, centroids, 8, CV_32S);
	//输出找到的连通域
	cout << "total connected components detected:" << num_labels << endl;
	//使用不同的颜色标记连通域
	vector<Vec3b> colors(num_labels);
	// background color
	colors[0] = Vec3b(110, 10, 60);
	// object color
	RNG rng;
	for (int i = 1; i < num_labels; i++)
	{
		colors[i] = Vec3b(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
		//如果连通域小于100，则把连通域置为黑色背景。
		if (stats.at<int>(i - 1, CC_STAT_AREA) < 400&& stats.at<int>(i - 1, CC_STAT_AREA)<20000)
			colors[i] = Vec3b(0, 0, 0);
	}

	// render result
	Mat color_image = Mat::zeros(source_img.size(), source_img.type());
	////cout << source_img.type() << endl;
	//int w = color_image.cols;
	//int h = color_image.rows;
	//for (int row = 0; row < h; row++)
	//{
	//	for (int col = 0; col < w; col++)
	//	{
	//		int label = labels.at<int>(row, col);
	//		if (label == 0) continue;
	//		color_image.at<Vec3b>(row, col) = colors[label];
	//	}
	//}
	//找到光源中心，
	//利用统计信息标记连通域,
	for (int i = 0; i < num_labels ; i++)
	{
		Vec2d pt = centroids.at<Vec2d>(i, 0);
		//int x = stats.at<int>(i, CC_STAT_LEFT);
		//int y = stats.at<int>(i, CC_STAT_TOP);
		//int width = stats.at<int>(i, CC_STAT_WIDTH);
		//int height = stats.at<int>(i, CC_STAT_HEIGHT);
		int area = stats.at<int>(i, CC_STAT_AREA);
		cout << area << endl;
		if (area >1000 && area< 1000000) 
		{
			printf("area : %d, center point(%.2f, %.2f)\n", area, pt[0], pt[1]);
			circle(source_img, Point(pt[0], pt[1]), 250, Scalar(80, 80, 80), -1, 8, 0);
		}

		//rectangle(color_image, Rect(x, y, width, height), Scalar(255, 0, 0), 1, 8, 0);
	}
	Mat img_size;
	resize(source_img, img_size, Size(), 0.2, 0.2);
	imshow("color_image", img_size);
	//resizeWindow("color_image", 500, 500);
	return source_img;
}
