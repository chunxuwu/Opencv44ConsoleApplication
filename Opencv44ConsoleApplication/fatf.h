#pragma once
class fatf
{
public:
	int fat_detect(int jugement,Mat &img,fv_SParam &param);
	//���ز����뾶
	float fat_detect_test(int jugement, Mat &img, fv_SParam &param);
	int abonormalTestf(int jugement,Mat &img, fv_SParam &param);
	int regionfOffLine(int jugement, Mat &img, Mat &imgV, fv_SParam &param);
	int DZJudgeF(int jugement, Mat &img, int eliminate_Min, fv_SParam &param);
	int DustJudgeF(int jugement, Mat &img, int hv_bigDust_Min, fv_SParam &param);
};

