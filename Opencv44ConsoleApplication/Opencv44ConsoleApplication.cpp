// Opencv44ConsoleApplication.cpp : �������̨Ӧ�ó������ڵ㡣
#include "stdafx.h"

//���ϲ��ԣ����Ͼ�ͷ���ļ��У�indexΪ���Լ���
void package_test(string filepath,int index_begin,int index_end);
//���ž�ͷ����
void single_test(string filepath);
void show_result(int flag);

int main()
{
	fv_SParam param;
	//Mat img_ok = imread("C:\\Users\\czwucx\\Desktop\\single_light\\mn_pic\\7.bmp");
	//9Ϊ����,6��12Ϊ����,7Ϊok��10Ϊ�ڳ�//C:\\Users\\czwucx\\Desktop\\single_light\\mn_pic\\7.bmp
	//E:\\�ӹ�ͼƬ\\2020430NG\\37/16.bmp;;;C:\\Users\\czwucx\\Desktop\\single_light\\�ӹ�ͼƬ\\2020427\\OK\\20
	//Mat img_ng = imread("C:\\Users\\czwucx\\Desktop\\single_light\\mn_pic\\16.bmp");
	Mat ho_Image = imread("E:\\flare_pic\\test_pic\\1\\1-33\\3.bmp");
	//Mat ho_Image = imread("C:\\Users\\czwucx\\Desktop\\single_light\\�ӹ�ͼƬ\\�Զ���̨�����ͼ1203\\1-15/2.bmp");
	if (ho_Image.empty() == true)
	{
		cout << "no image was selected" << endl;
		return -1;
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
	int flag = 0;
	//���в���
	string files_path = "E:\\�ӹ�ͼƬ\\����ͼ��\\1\\1-";	
	//package_test(files_path,1,50);

	//���ž�ͷ���
	string file_path = "E:\\flare_pic\\test_pic\\1\\1-17";
	single_test(file_path);

	//���Ų���
	fatf ff;
	
	//flag = ff.abonormalTestf(param.judgeFlag, ho_Image_AllV, param);
	//flag = ff.fat_detect(flag,ho_Image_Median, param);
	//flag = ff.regionfOffLine_test(flag, ho_Image_Median, ho_Image_AllV, param);
	//show_result(flag);

	//image_subtract minrect;
	//minrect.min_rect_test(ho_Image_AllV);
	waitKey(600000);
	destroyAllWindows();
    return 0;
}

//���о�ͷ����
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
		string result = "C:\\Users\\czwucx\\Desktop/opencv_result.txt";
		jugement = my_evaluate.my_evaluate_test(output_name, result);
		OutFile << jugement << endl;
	}
	OutFile.close();
}
//�����ļ��в���
void single_test(string filepath)
{
	evaluate my_evaluate;
	string jugement="��ͼ";
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
		cout << "NG����Դ����";
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
	}
}
