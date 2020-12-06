#pragma once
class flare_detecte_V3
{
public:
	flare_detecte_V3();
	~flare_detecte_V3();
	int my_flare_detecte_V3(Mat image,int edgeDZ_distance,int long_DZ_length, int shortDZ_num,int fat_radius,int inside_dust);
	int DZ_detect3(Mat img, Point pt, int long_DZ, int edge_DZ, int shortDZ_nums, int dust_area);
};

