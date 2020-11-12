// Opencv44ConsoleApplication.cpp : �������̨Ӧ�ó������ڵ㡣
//
//ͷ�ļ��������д��ͷ�ļ���*******
#include "stdafx.h"

//0������OK
//1����������


int main()
{
	Mat img_ok = imread("C:\\Users\\czwucx\\Desktop\\single_light\\mn_pic\\7.bmp");
	//9Ϊ����,6��12Ϊ����,7Ϊok��10Ϊ�ڳ�//C:\\Users\\czwucx\\Desktop\\single_light\\mn_pic\\7.bmp
	//E:\�ӹ�ͼƬ\2020430NG\38
	Mat img_ng = imread("C:\\Users\\czwucx\\Desktop\\single_light\\mn_pic\\16.bmp");
	//Mat img_ng = imread("E:\\�ӹ�ͼƬ\\2020430NG\\37/1.bmp");
	if (img_ok.empty() == true)
	{
		cout << "erro: no image was selected" << endl;

	}
	//imshow("inside_dust", source_img);

	//ͼƬ��СΪԭ����0.2

	Mat img1_size, img2_size;
	resize(img_ok, img1_size, Size(), 0.2, 0.2);
	resize(img_ng, img2_size, Size(), 0.5, 0.5);
	//imshow("size_image", img2_size);
	Mat gray1_img,gray2_img;
	cvtColor(img1_size, gray1_img, COLOR_RGB2GRAY);
	cvtColor(img2_size, gray2_img, COLOR_RGB2GRAY);
	//
	//string path_ok = "C:\\Users\\czwucx\\Desktop\\single_light\\�ӹ�ͼƬ\\7.9_ok\\filename3.txt";
	string path_ng = "";
	//string path = "C:\\Users\\czwucx\\Desktop\\single_light\\�ӹ�ͼƬ\\7.9_inside_dust\\filename3.txt";
	//evaluate my_evaluate;
	//my_evaluate.my_evaluate(path);

	//ͼ�����
	//image_subtract img_sub;
	//img_sub.my_image_subtract(img2_size, img1_size);

	//��Ҫ��������
	//Mat no_center_img;
	//subtract_center_area sca;
	//no_center_img = sca.subtract_area(img_ng);



	//��Դ���ּ��
	//fat_light_detect fat_detect;
	//int flag = fat_detect.fat_detect(gray2_img);

	//�ڳ����
	//inside_dust_detect dust_detect;
	//int flag = dust_detect.my_inside_dust_detect(gray2_img);
	
	//�ӹ���
	flare_detecte_V3 fd;
	int flag = fd.my_flare_detecte_V3(img2_size);
	cout << "������״̬" << flag << endl;
	switch (flag)
	{
		case 0:
			cout<<"OK";
			break;
		case 1:
			cout<<"��Դ����NG";
			break;
		case 2:
			cout << "�ڳ�NG";
			break;
		case 3:
			cout << "������NG";
			break;
		case 4:
			cout << "�̵���NG";
		case 5:
			cout << "��Ե����NG";
		default:
			break;
	}

	waitKey(500000);
    return 0;
}

