// Opencv44ConsoleApplication.cpp : �������̨Ӧ�ó������ڵ㡣
//
//ͷ�ļ��������д��ͷ�ļ���*******
#include "stdafx.h"

//0������OK
//1����������

//���ϲ��ԣ����Ͼ�ͷ���ļ��У�indexΪ���Լ���
void package_test(string filepath,int index_begin,int index_end);
//���ž�ͷ����
void single_test(string filepath);

int main()
{
	fv_SParam param;
	//Mat img_ok = imread("C:\\Users\\czwucx\\Desktop\\single_light\\mn_pic\\7.bmp");
	//9Ϊ����,6��12Ϊ����,7Ϊok��10Ϊ�ڳ�//C:\\Users\\czwucx\\Desktop\\single_light\\mn_pic\\7.bmp
	//C:\Users\czwucx\Desktop\single_light\�ӹ�ͼƬ\7.18\����
	//E:\\�ӹ�ͼƬ\\2020430NG\\37/16.bmp;;;C:\\Users\\czwucx\\Desktop\\single_light\\�ӹ�ͼƬ\\2020427\\OK\\20
	//Mat img_ng = imread("C:\\Users\\czwucx\\Desktop\\single_light\\mn_pic\\16.bmp");
	Mat ho_Image = imread("C:\\Users\\czwucx\\Desktop\\single_light\\mn_pic\\19.bmp");
	//Mat ho_Image = imread("C:\\Users\\czwucx\\Desktop\\single_light\\�ӹ�ͼƬ\\�Զ���̨�����ͼ1203\\1-15/2.bmp");
	if (ho_Image.empty() == true)
	{
		cout << "erro: no image was selected" << endl;

	}
	Mat ho_Image_Zoom;
	//resize(img_ok, img1_size, Size(), 0.5, 0.5);
	resize(ho_Image, ho_Image_Zoom, Size(), param.hv_AbnormalZoomSize, param.hv_AbnormalZoomSize);

	Mat hsv_img, gray;
	cvtColor(ho_Image_Zoom, hsv_img, COLOR_BGR2HSV);
	//hsvͨ������
	vector<Mat> hsv_channels;
	split(hsv_img, hsv_channels);
	//Vͨ��
	Mat ho_Image_AllV;
	ho_Image_AllV = hsv_channels.at(2);
	//��ֵ�˲�
	Mat ho_Image_Median;
	medianBlur(ho_Image_AllV, ho_Image_Median, param.hv_AbnormalMedianT);

	//��������
	//string path = "C:\\Users\\czwucx\\Desktop\\single_light\\�ӹ�ͼƬ\\2020427\\OK\\filename3.txt";
	//string path = "E:\\�ӹ�ͼƬ\\2020430NG\\filename3.txt";
	string path = "C:\\Users\\czwucx\\Desktop\\single_light\\�ӹ�ͼƬ\\7.9_ok\\filename3.txt";
	//string path = "C:\\Users\\czwucx\\Desktop\\single_light\\�ӹ�ͼƬ\\2020427\\OK\\filename3.txt";
	evaluate my_evaluate;
	//string output_name = "fat_test_7.9ok.txt";
	//my_evaluate.my_evaluate(path, output_name);

	string files_path = "E:\\�ӹ�ͼƬ\\����ͼ��\\1\\1-";
	int num = 10;
	//package_test(files_path,27,50);

	//���ž�ͷ���
	string file_path = "E:\\�ӹ�ͼƬ\\����ͼ��\\1\\1-14";
	//single_test(file_path);

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



	int edgeDZ_distance = 600;
	int long_DZ_length = 100;
	int shortDZ_nums = 3;
	int fat_radius = 350;
	int inside_dust=1000;

	fatf ff;
	int flag=0;
	
	//flag = ff.abonormalTestf(param.judgeFlag, ho_Image_AllV, param);
	//flag = ff.fat_detect(flag,ho_Image_Median, param);
	//flag = ff.regionfOffLine(flag, ho_Image_Median, ho_Image_AllV, param);

	//flare_detect_V4 fd;
	//flag = fd.my_flare_detecte_V4(ho_Image, edgeDZ_distance,long_DZ_length, shortDZ_nums, fat_radius,inside_dust);
	//cout << "������״̬:" <<endl;
	

	/*switch (flag)
	{
		case 0:
			cout<<"OK";
			break;
		case 1:
			cout<<"NG����Դ����";
			break;
		case 2:
			cout << "NG���ڳ�";
			break;
		case 3:
			cout << "NG������";
			break;
		case 4:
			cout << "NG��������";
			break;
		case 5:
			cout << "NG�������";
			break;
		case 6:
			cout << "NG����Ե����";
			break;
		case 10:
			cout << "�쳣";
			break;
		default:
			break;
	}*/

	waitKey(60000);
	destroyAllWindows();
    return 0;
}

void package_test(string filepath,int num1,int num2)
{
	evaluate my_evaluate;
	ofstream OutFile("C:\\Users\\czwucx\\Desktop/һ�в��ԵĽ��.txt");
	string path =filepath;
	string jugement;
	int index = num2;
	for (int i = num1; i <= index; i++)
	{
		string path1 = path;
		path1 = path1 + to_string(i);
		string output_name = "C:\\Users\\czwucx\\Desktop/pic_filename.txt";
		my_evaluate.folder_evaluate(path1, output_name);
		string result = "C:\\Users\\czwucx\\Desktop/result.txt";
		jugement = my_evaluate.my_evaluate_test(output_name, result);
		OutFile << jugement << endl;
	}
	OutFile.close();
}

void single_test(string filepath)
{
	evaluate my_evaluate;
	string jugement="��ͼ";
	string path1 = filepath;
	string output_name = "C:\\Users\\czwucx\\Desktop/pic_filename.txt";
	my_evaluate.folder_evaluate(path1, output_name);
	string result = "C:\\Users\\czwucx\\Desktop/result.txt";
	jugement = my_evaluate.my_evaluate_test(output_name, result);
}
