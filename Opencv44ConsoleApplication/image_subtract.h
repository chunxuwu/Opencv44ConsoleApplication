#pragma once
class image_subtract
{
public:
	image_subtract();
	~image_subtract();
	void my_image_subtract(Mat &img1, Mat &img2);
	void min_rect_test(Mat &img);
	void blur_test(Mat &img);
};

