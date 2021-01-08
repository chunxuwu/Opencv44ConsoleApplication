// Opencv44ConsoleApplication.cpp : 定义控制台应用程序的入口点。
#include "stdafx.h"

//整合测试，整合镜头的文件夹，index为测试几颗
void package_test(string filepath,int index_begin,int index_end);
//单颗镜头测试
void single_test(string filepath);
void show_result(int flag);

int main()
{
	fv_SParam param;
	double t, t1, t2, t3, t4, t5, t7, t8;
	//int medianblur_size = param.
	//Mat img_ok = imread("C:\\Users\\czwucx\\Desktop\\single_light\\mn_pic\\7.bmp");
	//9为发胖,6、12为道子,7为ok；10为内尘//C:\\Users\\czwucx\\Desktop\\single_light\\mn_pic\\7.bmp
	//E:\\杂光图片\\2020430NG\\37/16.bmp;;;C:\\Users\\czwucx\\Desktop\\single_light\\杂光图片\\2020427\\OK\\20
	//Mat img_ng = imread("C:\\Users\\czwucx\\Desktop\\single_light\\mn_pic\\16.bmp");
	//Mat ho_Image = imread("E:\\flare_pic\\test_pic\\1\\1-31\\0.bmp");
	Mat ho_Image = imread("C:\\Users\\czwucx\\Desktop\\0108NG/1.bmp");
	if (ho_Image.empty() == true)
	{
		cout << "no image was selected" << endl;
		return -1;
	}
	t1 = (double)getTickCount();
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

	//blur(scale_img)
	t2 = 1000 * ((double)getTickCount() - t1) / getTickFrequency();
	Mat ho_Image_Median;
	medianBlur(ho_Image_AllV, ho_Image_Median, param.hv_AbnormalMedianT);
	t3 = 1000 * ((double)getTickCount() - t1) / getTickFrequency();
	cout << "中值滤波耗时" << t3 - t2 << endl;
	//高斯滤波
	Mat gaussi_pic;
	GaussianBlur(ho_Image_Median, gaussi_pic, Size(13,13),15,15,4);

	Mat scale_img;
	multiply(ho_Image_Median, 0.63, scale_img);
	//halcon动态阈值分割
	Mat mean_img;
	blur(scale_img, mean_img, Size(105, 105));
	Mat pro_dynthreshold;
	subtract(scale_img, mean_img, pro_dynthreshold);
	Mat dyn_threshold;
	threshold(pro_dynthreshold, dyn_threshold, 1, 255, 0);

	Mat dst;
	adaptiveThreshold(scale_img, dst, 255, 0, 0, 105, -1);

	Mat dst1;
	adaptiveThreshold(gaussi_pic, dst1, 255, 0, 0, 105, -1);
	//批量测试
	//string path = "C:\\Users\\czwucx\\Desktop\\single_light\\杂光图片\\2020427\\OK\\filename3.txt";
	//string path = "E:\\杂光图片\\2020430NG\\filename3.txt";
	string path = "C:\\Users\\czwucx\\Desktop\\single_light\\杂光图片\\7.9_ok\\filename3.txt";
	//string path = "C:\\Users\\czwucx\\Desktop\\single_light\\杂光图片\\2020427\\OK\\filename3.txt";
	evaluate my_evaluate;
	//string output_name = "fat_test_7.9ok.txt";
	//my_evaluate.my_evaluate(path, output_name);
	int flag = 0;
	//整盒测试
	string files_path = "E:\\杂光图片\\测试图库\\1\\1-";	
	//package_test(files_path,1,50);

	//单颗镜头检测
	string file_path = "E:\\flare_pic\\test_pic\\1\\1-6";
	//single_test(file_path);

	//单张测试
	fatf ff;
	
	//flag = ff.abonormalTestf(param.judgeFlag, ho_Image_AllV, param);
	//flag = ff.fat_detect(flag,ho_Image_Median, param);
	flag = ff.regionfOffLine_test(flag, ho_Image_Median, ho_Image_AllV, param);
	show_result(flag);

	//image_subtract TTT;
	//TTT.blur_test(ho_Image_AllV);

	waitKey(600000);
	destroyAllWindows();
    return 0;
}

//整盒镜头测试
void package_test(string filepath,int num1,int num2)
{
	evaluate my_evaluate;
	ofstream OutFile("C:\\Users\\czwucx\\Desktop/一盒测试的结果.txt");
	string path =filepath;
	string jugement;
	int index = num2;
	for (int i = num1; i <= index; i++)
	{
		string path1 = path;
		path1 = path1 + to_string(i);
		string output_name = "C:\\Users\\czwucx\\Desktop/pic_filename.txt";
		my_evaluate.folder_evaluate(path1, output_name);
		string result = "C:\\Users\\czwucx\\Desktop/opencv_result.txt";
		jugement = my_evaluate.my_evaluate_test(output_name, result);
		OutFile << jugement << endl;
	}
	OutFile.close();
}
//单个文件夹测试
void single_test(string filepath)
{
	evaluate my_evaluate;
	string jugement="无图";
	string path1 = filepath;
	string output_name = "C:\\Users\\czwucx\\Desktop/pic_filename.txt";
	my_evaluate.folder_evaluate(path1, output_name);
	string result = "C:\\Users\\czwucx\\Desktop/opencv_result.txt";
	jugement = my_evaluate.my_evaluate_test(output_name, result);
}

void show_result(int flag)
{
	switch (flag)
	{
	case 0:
		cout << "OK";
		break;
	case 1:
		cout << "NG—光源发胖";
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
}
