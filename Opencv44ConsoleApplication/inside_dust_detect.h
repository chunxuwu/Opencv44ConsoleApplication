#pragma once
class inside_dust_detect
{
public:
	inside_dust_detect();
	~inside_dust_detect();
	int my_inside_dust_detect(Mat source_img);
	int DZ_detect(Mat img, Point pt);
};

