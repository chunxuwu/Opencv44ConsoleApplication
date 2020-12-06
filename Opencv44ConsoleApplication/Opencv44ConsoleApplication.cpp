// Opencv44ConsoleApplication.cpp : 定义控制台应用程序的入口点。
//
//头文件还必须得写在头文件中*******
#include "stdafx.h"

//0——》OK
//1——》放胖


int main()
{
	fv_SParam param;
	//Mat img_ok = imread("C:\\Users\\czwucx\\Desktop\\single_light\\mn_pic\\7.bmp");
	//9为发胖,6、12为道子,7为ok；10为内尘//C:\\Users\\czwucx\\Desktop\\single_light\\mn_pic\\7.bmp
	//C:\Users\czwucx\Desktop\single_light\杂光图片\7.18\发胖
	//E:\\杂光图片\\2020430NG\\37/16.bmp;;;C:\\Users\\czwucx\\Desktop\\single_light\\杂光图片\\2020427\\OK\\20
	//Mat img_ng = imread("C:\\Users\\czwucx\\Desktop\\single_light\\mn_pic\\16.bmp");
	Mat ho_Image = imread("C:\\Users\\czwucx\\Desktop\\single_light\\mn_pic\\10.bmp");
	if (ho_Image.empty() == true)
	{
		cout << "erro: no image was selected" << endl;

	}
	Mat ho_Image_Zoom;
	//resize(img_ok, img1_size, Size(), 0.5, 0.5);
	resize(ho_Image, ho_Image_Zoom, Size(), param.hv_AbnormalZoomSize, param.hv_AbnormalZoomSize);

	Mat hsv_img, gray;
	cvtColor(ho_Image_Zoom, hsv_img, COLOR_BGR2HSV);
	//hsv通道分离
	vector<Mat> hsv_channels;
	split(hsv_img, hsv_channels);
	//V通道
	Mat ho_Image_AllV;
	ho_Image_AllV = hsv_channels.at(2);
	//中值滤波
	Mat ho_Image_Median;
	medianBlur(ho_Image_AllV, ho_Image_Median, 2*(param.hv_AbnormalMedianT)+1);

	//批量测试
	//string path = "C:\\Users\\czwucx\\Desktop\\single_light\\杂光图片\\2020427\\OK\\filename3.txt";
	//string path = "E:\\杂光图片\\2020430NG\\filename3.txt";
	string path = "C:\\Users\\czwucx\\Desktop\\single_light\\杂光图片\\7.9_ok\\filename3.txt";
	//string path = "C:\\Users\\czwucx\\Desktop\\single_light\\杂光图片\\2020427\\OK\\filename3.txt";
	evaluate my_evaluate;
	String output_name = "fat_test_7.9ok.txt";
	//my_evaluate.my_evaluate(path, output_name);

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



	int edgeDZ_distance = 600;
	int long_DZ_length = 100;
	int shortDZ_nums = 3;
	int fat_radius = 350;
	int inside_dust=1000;

	fatf ff;
	int flag=0;
	
	//flag = ff.abonormalTestf(param.judgeFlag, ho_Image_AllV, param);
	//flag = ff.fat_detect(flag,ho_Image_Median, param);
	flag = ff.regionfOffLine(flag, ho_Image_Median, ho_Image_AllV, param);

	//flare_detect_V4 fd;
	//flag = fd.my_flare_detecte_V4(ho_Image, edgeDZ_distance,long_DZ_length, shortDZ_nums, fat_radius,inside_dust);
	//cout << "检测出的状态:" <<endl;
	

	switch (flag)
	{
		case 0:
			cout<<"OK";
			break;
		case 1:
			cout<<"NG—光源发胖";
			break;
		case 2:
			cout << "NG—内尘";
			break;
		case 3:
			cout << "NG—道子";
			break;
		case 4:
			cout << "NG—长道子";
			break;
		case 5:
			cout << "NG—多道子";
			break;
		case 6:
			cout << "NG—边缘道子";
			break;
		case 10:
			cout << "异常";
			break;
		default:
			break;
	}


	//switch (flag)
	//{
	//case 0:
	//	cout << "OK";
	//	break;
	//case 1:
	//	cout << "NG";
	//	break;
	//case 2:
	//	cout << "NG";
	//	break;
	//case 3:
	//	cout << "NG";
	//	break;
	//case 4:
	//	cout << "NG";
	//case 5:
	//	cout << "NG";
	//default:
	//	break;
	//}


	waitKey(60000);
	destroyAllWindows();
    return 0;
}

