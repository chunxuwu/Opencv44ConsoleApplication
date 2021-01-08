#pragma once
class fatf
{
public:
	int fat_detect(int jugement,Mat &img,fv_SParam &param);
	//·µ»Ø²âÁ¿°ë¾¶
	float fat_detect_test(int jugement, Mat &img, fv_SParam &param);

	int abonormalTestf(int jugement,Mat &img, fv_SParam &param);
	int regionfOffLine(int jugement, Mat &img, Mat &imgV, fv_SParam &param);
	int regionfOffLine_test(int jugement, Mat &img, Mat &imgV, fv_SParam &param);
	int my_regionfOffLine_test(int jugement, Mat &img, Mat &imgV, fv_SParam &param);
	int DZJudgeF(int jugement, Mat &img, double eliminate_Min, fv_SParam &param);
	int DZJudgeF_text(int jugement, Mat &src ,Mat &reduce_img, double eliminate_Min, fv_SParam &param);
	int DustJudgeF(int jugement, Mat &img, int hv_bigDust_Min, fv_SParam &param);
};

