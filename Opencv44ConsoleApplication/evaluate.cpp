#include "stdafx.h"
#include "evaluate.h"

void evaluate::my_evaluate(String txt_path,String outname)
{
	ifstream file(txt_path);
	int img_index = 0;
	//inside_dust_detect detect;
	int edgeDZ_distance = 600;
	int long_DZ_length = 100;
	int shortDZ_nums = 3;
	int fat_radius = 350;
	int inside_dust = 1000;
	flare_detect_V4 fd;
	fatf ff;

	fv_SParam param;
	double t;
	t = (double)getTickCount();
	ofstream OutFile(outname); //���ù��캯������txt�ı������Ҵ򿪸��ı�
	while (!file.eof())
	{
		char img_name[2000];
		file.getline(img_name, 2000);
		char img_file[2000];
		//cout << img_name << endl;
		Mat source_img = imread(img_name);
		if (source_img.empty() == true)
		{
			cout << "erro: no image was selected" << endl;
			continue;
		}		
		img_index++;
		//detect.my_inside_dust_detect(source_img);
		//�ӹ���

		//int flag = fd.my_flare_detecte_V4(source_img, edgeDZ_distance, long_DZ_length, shortDZ_nums, fat_radius, inside_dust);
		
		Mat ho_Image_Zoom;
		resize(source_img, ho_Image_Zoom, Size(), param.hv_AbnormalZoomSize, param.hv_AbnormalZoomSize);

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
		medianBlur(ho_Image_AllV, ho_Image_Median, 2 * (param.hv_AbnormalMedianT) + 1);



		float radius;
		radius = ff.fat_detect_test(0, ho_Image_Median,param);

		cout << "��" << img_index << "��ͼƬ" << endl;
		//if (flag = 0)
		{
			//cout << img_name << endl;
			OutFile << radius << endl;
			//sprintf_s(img_file, "img_%d", img_index);
			//cout << img_file << "*********" << flag << endl;
		}
			
		//waitKey(500);
		//int��stringƴ��
	}
	t=1000*((double)getTickCount() - t) / getTickFrequency();
	t = t / img_index;
	cout << "����һ��ͼƬ��ƽ��ʱ�䣺" <<t<<"ms"<< endl;
	OutFile << "����һ��ͼƬ��ƽ��ʱ��:"<<t << endl;
	OutFile.close();
	//double ok_Accuracy = (OK_nums / img_index);
	//cout <<"�����ʣ�"<< ok_Accuracy<<endl;
	//double ng_accuracy = NG_nums / img_index;
	//cout <<"©���ʣ�"<< ng_accuracy << endl;


}
