#include "stdafx.h"
#include "evaluate.h"


evaluate::evaluate()
{
}


evaluate::~evaluate()
{
}

void evaluate::my_evaluate(String txt_path)
{
	ifstream file(txt_path);
	int img_index = 0;
	inside_dust_detect detect;
	double t;
	t = (double)getTickCount();
	while (!file.eof())
	{
		char img_name[2000];
		file.getline(img_name, 2000);
		char img_file[2000];
		cout << img_name << endl;
		Mat source_img = imread(img_name);
		if (source_img.empty() == true)
		{
			cout << "erro: no image was selected" << endl;
			continue;
		}
		detect.my_inside_dust_detect(source_img);
		//waitKey(500);
		//int和string拼接
		sprintf_s(img_file, "img_%d", img_index);
		cout << img_file << endl;
		img_index++;
	}
	t=1000*((double)getTickCount() - t) / getTickFrequency();
	t = t / img_index;
	cout << "处理一张图片的平均时间：" <<t<<"ms"<< endl;
	double ok_Accuracy = (OK_nums / img_index);
	cout <<"良品率："<< ok_Accuracy<<endl;
	double ng_accuracy = NG_nums / img_index;
	cout <<"不良品率："<< ng_accuracy << endl;
}
