// Opencv44ConsoleApplication.cpp : 定义控制台应用程序的入口点。
//
//头文件还必须得写在头文件中*******
#include "stdafx.h"

//0——》OK
//1——》放胖


int main()
{
	Mat img_ok = imread("C:\\Users\\czwucx\\Desktop\\single_light\\mn_pic\\7.bmp");
	//9为发胖,6、12为道子,7为ok；10为内尘//C:\\Users\\czwucx\\Desktop\\single_light\\mn_pic\\7.bmp
	//E:\杂光图片\2020430NG\38
	Mat img_ng = imread("C:\\Users\\czwucx\\Desktop\\single_light\\mn_pic\\16.bmp");
	//Mat img_ng = imread("E:\\杂光图片\\2020430NG\\37/1.bmp");
	if (img_ok.empty() == true)
	{
		cout << "erro: no image was selected" << endl;

	}
	//imshow("inside_dust", source_img);

	//图片缩小为原来的0.2

	Mat img1_size, img2_size;
	resize(img_ok, img1_size, Size(), 0.2, 0.2);
	resize(img_ng, img2_size, Size(), 0.5, 0.5);
	//imshow("size_image", img2_size);
	Mat gray1_img,gray2_img;
	cvtColor(img1_size, gray1_img, COLOR_RGB2GRAY);
	cvtColor(img2_size, gray2_img, COLOR_RGB2GRAY);
	//
	//string path_ok = "C:\\Users\\czwucx\\Desktop\\single_light\\杂光图片\\7.9_ok\\filename3.txt";
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



	//光源发胖检测
	//fat_light_detect fat_detect;
	//int flag = fat_detect.fat_detect(gray2_img);

	//内尘检测
	//inside_dust_detect dust_detect;
	//int flag = dust_detect.my_inside_dust_detect(gray2_img);
	
	//杂光检测
	flare_detecte_V3 fd;
	int flag = fd.my_flare_detecte_V3(img2_size);
	cout << "检测出的状态" << flag << endl;
	switch (flag)
	{
		case 0:
			cout<<"OK";
			break;
		case 1:
			cout<<"光源发胖NG";
			break;
		case 2:
			cout << "内尘NG";
			break;
		case 3:
			cout << "长道子NG";
			break;
		case 4:
			cout << "短道子NG";
		case 5:
			cout << "边缘道子NG";
		default:
			break;
	}

	waitKey(500000);
    return 0;
}

