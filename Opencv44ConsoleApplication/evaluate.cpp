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
//	flare_detect_V4 fd;
	fatf ff;

	fv_SParam param;
	double t,t1,t2,t3;
	t = (double)getTickCount();
	ofstream OutFile(outname); //利用构造函数创建txt文本，并且打开该文本
	//判断结果
	string jugement = "没有图片";
	OutFile << "每一张的检测结果:" << endl;
	while (!file.eof())
	{
		
		char img_name[2000];
		file.getline(img_name, 2000);
		char img_file[2000];
		//cout << img_name << endl;
		Mat source_img = imread(img_name);
		if (source_img.empty() == true)
		{
			cout << "no image was selected" << endl;
			continue;
		}
		img_index++;
		//detect.my_inside_dust_detect(source_img);
		//杂光检测
		jugement = "OK";
		//int flag = fd.my_flare_detecte_V4(source_img, edgeDZ_distance, long_DZ_length, shortDZ_nums, fat_radius, inside_dust);
		t1 = (double)getTickCount();
		Mat ho_Image_Zoom;
		resize(source_img, ho_Image_Zoom, Size(), param.hv_AbnormalZoomSize, param.hv_AbnormalZoomSize);

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
		medianBlur(ho_Image_AllV, ho_Image_Median, param.hv_AbnormalMedianT);
		int flag = 0;
		//float radius;
		//radius = ff.fat_detect_test(0, ho_Image_Median,param);

		flag = ff.fat_detect_test(0, ho_Image_Median, param);
		t2 = 1000 * ((double)getTickCount() - t1) / getTickFrequency();
		flag = ff.regionfOffLine(flag, ho_Image_Median, ho_Image_AllV, param);
		t3 = 1000 * ((double)getTickCount() - t2) / getTickFrequency();
		if (flag != 0)
		{
			jugement = "NG";
			break;
		}
			
		cout << "第" << img_index << "张图片" << endl;
		cout << t2 << endl;
		cout << t3 << endl;
		//if (flag = 0)
		{
			//OutFile << radius << endl;
			
			OutFile << "第" << img_index << "张图片的检测结果：" << flag << endl;
			//cout << img_file << "*********" << flag << endl;
		}
		//waitKey(500);
		//int和string拼接
	}
	t=1000*((double)getTickCount() - t) / getTickFrequency();
	t = t / img_index;
	cout << "处理一张图片的平均时间：" <<t<<"ms"<< endl;
	cout << "判断结果：" << jugement << endl;
	OutFile << "处理一张图片的平均时间:"<<t << endl;
	OutFile << "判断结果：" << jugement << endl;
	OutFile.close();
	//double ok_Accuracy = (OK_nums / img_index);
	//cout <<"过检率："<< ok_Accuracy<<endl;
	//double ng_accuracy = NG_nums / img_index;
	//cout <<"漏检率："<< ng_accuracy << endl;


}

string evaluate::my_evaluate_test(string txt_path, string outname)
{
	ifstream file(txt_path);
	int img_index = 0;
	//inside_dust_detect detect;
	/*int edgeDZ_distance = 600;
	int long_DZ_length = 100;
	int shortDZ_nums = 3;
	int fat_radius = 350;
	int inside_dust = 1000;*/
	//flare_detect_V4 fd;
	fatf ff;
	fv_SParam param;
	int mid_size = param.hv_AbnormalMedianT;
	double t,t1,t2,t3,t4,t5,t7,t8;
	t = (double)getTickCount();
	ofstream OutFile(outname); //利用构造函数创建txt文本，并且打开该文本
	//判断结果
	string jugement;// = "没有图片";
	jugement = "OK";
	int flags = 0;
	OutFile << "每一张的检测结果:" << endl;
//#	pragma omp parallel num_threads(4)
	while (!file.eof())
	{
		char img_name[2000];
		file.getline(img_name, 2000);
		char img_file[2000];
		//cout << img_name << endl;
		t1 = (double)getTickCount();
		Mat source_img = imread(img_name);
		if (source_img.empty() == true)
		{
			//cout << "erro: no image was selected" << endl;
			break;
		}
		img_index++;
		//detect.my_inside_dust_detect(source_img);
		//杂光检测
		//jugement = "OK";
		//int flag = fd.my_flare_detecte_V4(source_img, edgeDZ_distance, long_DZ_length, shortDZ_nums, fat_radius, inside_dust);
		t2 = 1000 * ((double)getTickCount() - t1) / getTickFrequency();
		Mat ho_Image_Zoom;
		resize(source_img, ho_Image_Zoom, Size(), param.hv_AbnormalZoomSize, param.hv_AbnormalZoomSize);
		t3 = 1000 * ((double)getTickCount() - t1) / getTickFrequency();
		cout << "resize图片耗时" << t3 - t2 << endl;
		//t2 = 1000 * ((double)getTickCount() - t1) / getTickFrequency();
		Mat hsv_img, gray;
		cvtColor(ho_Image_Zoom, hsv_img, COLOR_BGR2HSV);
		//hsv通道分离
		vector<Mat> hsv_channels;
		split(hsv_img, hsv_channels);
		//V通道
		Mat ho_Image_AllV;
		ho_Image_AllV = hsv_channels.at(2);
		t4 = 1000 * ((double)getTickCount() - t1) / getTickFrequency();
		cout << "V通道提取耗时" << t4 - t3 << endl;
		//中值滤波
		Mat ho_Image_Median;
		GaussianBlur(ho_Image_AllV, ho_Image_Median, Size(mid_size, mid_size), 15, 15, 4);
		//medianBlur(ho_Image_AllV, ho_Image_Median, mid_size);
		t3 = 1000 * ((double)getTickCount() - t1) / getTickFrequency();
		cout << "高斯滤波耗时" << t3 - t4 << endl;
		int flag = 0;
		//float radius;
		//radius = ff.fat_detect_test(0, ho_Image_Median,param);
		t7 = 1000 * ((double)getTickCount() - t1) / getTickFrequency();
		flag = ff.abonormalTestf(0, ho_Image_AllV, param);
		t8 = 1000 * ((double)getTickCount() - t1) / getTickFrequency();
		flag = ff.fat_detect(flag, ho_Image_Median, param);
		t4 = 1000 * ((double)getTickCount() - t1) / getTickFrequency();
		flag = ff.regionfOffLine_test(flag, ho_Image_Median, ho_Image_AllV, param);
		//t4 = 1000 * ((double)getTickCount() - t3) / getTickFrequency();
		t5 = 1000 * ((double)getTickCount() - t1) / getTickFrequency();
		if (flag != 0)
		{
			jugement = "NG";
			flags++;
			//break;
		}
		cout << "第" << img_index << "张图片处理完成." << endl;
		//cout << t2 << "\t" << t3 << "\t" << t4 << "\t" << t5 << endl;
		cout << "图片读取耗时：" << t2<< endl;
		cout << "图片预处理时间" << t3-t2 << endl;
		cout << "异常检测耗时：" << t8 - t7<<endl;
		cout << "光源发胖检测耗时：" << t4 - t8 << endl;
		cout << "道子内尘耗时：" << t5 - t4 << endl;
		cout << "处理总耗时：" << t5 << endl;
		//if (flag = 0)
		{
			//OutFile << radius << endl;
			OutFile << "第" << img_index << "张图片的检测结果：" << flag << endl;
			//cout << img_file << "*********" << flag << endl;
		}
	}
	//if (flags != 0) jugement = 1;
	t = 1000 * ((double)getTickCount() - t) / getTickFrequency();
	t = t / img_index;
	cout << "处理一张图片的平均时间：" << t << "ms" << endl;
	cout << "判断结果：" << jugement << endl;
	OutFile << "处理一张图片的平均时间:" << t << endl;
	//OutFile << "判断结果：" << jugement << endl;
	OutFile.close();
	
	return jugement;
}

void evaluate::folder_evaluate(string foldname, string outname)
{
	ofstream out_file(outname);
	string path = foldname;
	
	//获取绝对路径
	vector<string> filenames1;
	getFiles(path, filenames1);
	//仅获取文件名
	vector<string> filenames2;
	getFilesName(path, filenames2);

	for (int file_i = 0; file_i < filenames2.size(); file_i++)
	{
		string b1 = filenames1[file_i].c_str();
		string b2 = filenames2[file_i].c_str();


		char * test_b1 = str2char(b1);
		char * testChar = str2char(b2);

		char noSuffixName[100];
		//仅读取文件名，不包括文件扩展名，如文件名为0_0_272.jpg,则读取文件名noSuffixName="0_0_272"
		int	indexPoint = b2.find_first_of('.', 0);
		//int	indexPointEnd = b.find_first_of('.',0); 
		for (int j = 0; j<indexPoint; j++) {
			noSuffixName[j] = testChar[j];
		}
		noSuffixName[indexPoint] = '\0';//输入结束符

										//显示提取的文件名称
		//cout << "noSuffixName:" << noSuffixName << endl;//文件名称 （不包括文件扩展名）
		//cout << "fileName:" << testChar << endl;//文件名称+文件扩展名
		cout << "pathFileName:" << test_b1 << endl;//路径+文件名称+文件扩展名

		out_file << test_b1 << endl;

	}
	out_file.close();
}
void getFilesName(string path, vector<string>& files)
{
	//文件句柄
	long long   hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之
			//如果不是,加入列表
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
	//文件句柄
	long long  hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*.bmp").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之
			//如果不是,加入列表
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

