#include "stdafx.h"
#include "evaluate.h"


//for reading file
void getFiles(string path, vector<string>& files);
void getFilesName(string path, vector<string>& files);
char* str2char(string src);

void evaluate::my_evaluate(string txt_path, string outname)
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
	//�жϽ��
	string jugement = "û��ͼƬ";
	OutFile << "ÿһ�ŵļ����:" << endl;
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
		jugement = "OK";
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
		int flag = 0;
		//float radius;
		//radius = ff.fat_detect_test(0, ho_Image_Median,param);
		flag = ff.regionfOffLine(flag, ho_Image_Median, ho_Image_AllV, param);
		if (flag != 0)
		{
			jugement = "NG";
			break;
		}
			
		cout << "��" << img_index << "��ͼƬ" << endl;
		//if (flag = 0)
		{
			//OutFile << radius << endl;
			
			OutFile << "��" << img_index << "��ͼƬ�ļ������" << flag << endl;
			//cout << img_file << "*********" << flag << endl;
		}
		//waitKey(500);
		//int��stringƴ��
	}
	t=1000*((double)getTickCount() - t) / getTickFrequency();
	t = t / img_index;
	cout << "����һ��ͼƬ��ƽ��ʱ�䣺" <<t<<"ms"<< endl;
	cout << "�жϽ����" << jugement << endl;
	OutFile << "����һ��ͼƬ��ƽ��ʱ��:"<<t << endl;
	OutFile << "�жϽ����" << jugement << endl;
	OutFile.close();
	//double ok_Accuracy = (OK_nums / img_index);
	//cout <<"�����ʣ�"<< ok_Accuracy<<endl;
	//double ng_accuracy = NG_nums / img_index;
	//cout <<"©���ʣ�"<< ng_accuracy << endl;


}

string evaluate::my_evaluate_test(string txt_path, string outname)
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
	//�жϽ��
	string jugement = "û��ͼƬ";
	OutFile << "ÿһ�ŵļ����:" << endl;
	while (!file.eof())
	{

		char img_name[2000];
		file.getline(img_name, 2000);
		char img_file[2000];
		//cout << img_name << endl;
		Mat source_img = imread(img_name);
		if (source_img.empty() == true)
		{
			//cout << "erro: no image was selected" << endl;
			break;
		}
		img_index++;
		//detect.my_inside_dust_detect(source_img);
		//�ӹ���
		jugement = "OK";
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
		int flag = 0;
		//float radius;
		//radius = ff.fat_detect_test(0, ho_Image_Median,param);
		flag = ff.fat_detect(0, ho_Image_Median, param);
		flag = ff.regionfOffLine(flag, ho_Image_Median, ho_Image_AllV, param);
		if (flag != 0)
		{
			jugement = "NG";
			break;
		}

		cout << "��" << img_index << "��ͼƬ�������." << endl;
		//if (flag = 0)
		{
			//OutFile << radius << endl;

			OutFile << "��" << img_index << "��ͼƬ�ļ������" << flag << endl;
			//cout << img_file << "*********" << flag << endl;
		}
	}
	t = 1000 * ((double)getTickCount() - t) / getTickFrequency();
	t = t / img_index;
	cout << "����һ��ͼƬ��ƽ��ʱ�䣺" << t << "ms" << endl;
	cout << "�жϽ����" << jugement << endl;
	OutFile << "����һ��ͼƬ��ƽ��ʱ��:" << t << endl;
	OutFile << "�жϽ����" << jugement << endl;
	OutFile.close();
	return jugement;
}

void evaluate::folder_evaluate(string foldname, string outname)
{
	ofstream out_file(outname);
	string path = foldname;
	
	//��ȡ����·��
	vector<string> filenames1;
	getFiles(path, filenames1);
	//����ȡ�ļ���
	vector<string> filenames2;
	getFilesName(path, filenames2);

	for (int file_i = 0; file_i < filenames2.size(); file_i++)
	{
		string b1 = filenames1[file_i].c_str();
		string b2 = filenames2[file_i].c_str();


		char * test_b1 = str2char(b1);
		char * testChar = str2char(b2);

		char noSuffixName[100];
		//����ȡ�ļ������������ļ���չ�������ļ���Ϊ0_0_272.jpg,���ȡ�ļ���noSuffixName="0_0_272"
		int	indexPoint = b2.find_first_of('.', 0);
		//int	indexPointEnd = b.find_first_of('.',0); 
		for (int j = 0; j<indexPoint; j++) {
			noSuffixName[j] = testChar[j];
		}
		noSuffixName[indexPoint] = '\0';//���������

										//��ʾ��ȡ���ļ�����
		//cout << "noSuffixName:" << noSuffixName << endl;//�ļ����� ���������ļ���չ����
		//cout << "fileName:" << testChar << endl;//�ļ�����+�ļ���չ��
		cout << "pathFileName:" << test_b1 << endl;//·��+�ļ�����+�ļ���չ��

		out_file << test_b1 << endl;

	}
	out_file.close();
}
void getFilesName(string path, vector<string>& files)
{
	//�ļ����
	long long   hFile = 0;
	//�ļ���Ϣ
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//�����Ŀ¼,����֮
			//�������,�����б�
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					//getFiles( p.assign(path).append("\\").append(fileinfo.name), files );
					getFiles(fileinfo.name, files);
			}
			else
			{
				//files.push_back(p.assign(path).append("\\").append(fileinfo.name) );
				files.push_back(fileinfo.name);
			}
		} while (!_findnext(hFile, &fileinfo));
		_findclose(hFile);
	}
}
void getFiles(string path, vector<string>& files)
{
	//�ļ����
	long long  hFile = 0;
	//�ļ���Ϣ
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*.bmp").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//�����Ŀ¼,����֮
			//�������,�����б�
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (!_findnext(hFile, &fileinfo));
		_findclose(hFile);
	}
}
char* str2char(string src)
{
	char *dst = new char[255];
	int i;
	for (i = 0; i <= src.length(); i++)
		dst[i] = src[i];
	dst[i] = '\0';
	return dst;
}

