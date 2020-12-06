#pragma once
class flare_detect_V4
{
public:
	flare_detect_V4();
	~flare_detect_V4();
	int my_flare_detecte_V4(Mat image, int edgeDZ_distance, int long_DZ_length, int shortDZ_num, int fat_radius, int inside_dust);
	int DZ_detect4(Mat img, Point pt, int long_DZ, int edge_DZ, int shortDZ_nums, int dust_area);
};

