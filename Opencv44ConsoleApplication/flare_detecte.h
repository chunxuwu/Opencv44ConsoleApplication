#pragma once
class flare_detecte
{
public:
	flare_detecte();
	~flare_detecte();
	int my_inside_dust_detect2(Mat source_img);
	int DZ_detect2(Mat img, Point pt);
	int color_detect(Mat img);
};

