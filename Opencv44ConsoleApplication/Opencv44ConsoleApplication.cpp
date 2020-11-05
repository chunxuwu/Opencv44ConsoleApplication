// Opencv44ConsoleApplication.cpp : 定义控制台应用程序的入口点。
//
//头文件还必须得写在头文件中*******
#include "stdafx.h"

//0——》OK
//1——》放胖


int main()
{
	Mat img_ok = imread("C:\\Users\\czwucx\\Desktop\\single_light\\mn_pic\\7.bmp");
	//9为发胖
	Mat img_ng = imread("C:\\Users\\czwucx\\Desktop\\single_light\\mn_pic\\9.bmp");
	if (img_ok.empty() == true)
	{
		cout << "erro: no image was selected" << endl;

	}
	//imshow("inside_dust", source_img);

	//图片缩小为原来的0.2

	Mat img1_size, img2_size;
	resize(img_ok, img1_size, Size(), 0.2, 0.2);
	resize(img_ng, img2_size, Size(), 0.2, 0.2);
	//imshow("size_image", img2_size);

	//
	string path_ok = "C:\\Users\\czwucx\\Desktop\\single_light\\杂光图片\\7.9_ok\\filename3.txt";
	string path_ng = "";
	//string path = "C:\\Users\\czwucx\\Desktop\\single_light\\杂光图片\\7.9_inside_dust\\filename3.txt";
	//evaluate my_evaluate;
	//my_evaluate.my_evaluate(path);

	//图像相减
	//image_subtract img_sub;
	//img_sub.my_image_subtract(img2_size, img1_size);

	//主要质心区域
	//Mat no_center_img;
	//subtract_center_area sca;
	//no_center_img = sca.subtract_area(img_ng);

	//little_dust_detect dust_detect;
	//dust_detect.little_dust(no_center_img);

	//光源发胖检测
	fat_light_detect fat_detect;
	int flag = fat_detect.fat_detect(img2_size);
	
	switch (flag)
	{
	case 0:
		cout<<"OK";
	case 1:
		cout<<"发胖";
	default:
		break;
	}

	waitKey(50000);
    return 0;
}

