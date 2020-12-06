#include "stdafx.h"
#include "fatf.h"

RNG rng(12345);
//带有上下限的threshold
void threshold2(Mat gray, Mat& thresh, int minvalue, int maxvalue)
{
	Mat thresh1;
	Mat thresh2;
	threshold(gray, thresh1, minvalue, 255, THRESH_BINARY);
	threshold(gray, thresh2, maxvalue, 255, THRESH_BINARY_INV);
	thresh = thresh1 & thresh2;
}
//寻找连通域
vector<vector<Point>> connection3(Mat src)
{
	vector<vector<Point>>contours;
	findContours(src, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
	return contours;
}

vector<vector<Point>> connection2(Mat src, Mat& draw)
{
	draw = Mat::zeros(src.rows, src.cols, CV_8UC3);
	vector<vector<Point>>contours;
	findContours(src, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
	for (int i = 0; i<contours.size(); i++)
	{
		Scalar  color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(draw, contours, i, color, -1);
	}
	return contours;
}
//select_shape_area
vector<vector<Point>>  selectShapeArea2(Mat src, Mat& draw, vector<vector<Point>> contours, int minvalue, int maxvalue)
{
	vector<vector<Point>> result_contours;
	draw = Mat::zeros(src.rows, src.cols, CV_8UC3);
	for (int i = 0; i<contours.size(); i++)
	{
		int countour_area = contourArea(contours[i]);
		if (countour_area >minvalue && countour_area<maxvalue)
		{
			result_contours.push_back(contours[i]);
		}
	}
	//for (int i = 0; i<result_contours.size(); i++)
	//{
	//	Scalar  color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
	//	drawContours(draw, result_contours, i, color, -1);
	//}
	return result_contours;
}

vector<vector<Point>>  selectShapeArea3(vector<vector<Point>> contours, int minvalue, int maxvalue)
{
	vector<vector<Point>> result_contours;
	for (int i = 0; i<contours.size(); i++)
	{
		int countour_area = contourArea(contours[i]);
		if (countour_area >minvalue && countour_area<maxvalue)
		{
			result_contours.push_back(contours[i]);
		}
	}
	return result_contours;
}

Mat closing_circle2(Mat src,int radius ) 
{
	Mat closing_circle;
	Mat se1 = getStructuringElement(MORPH_ELLIPSE, Size(2*radius+1, 2*radius+1));
	morphologyEx(src, closing_circle, MORPH_CLOSE, se1);
	return closing_circle;
}

Mat opening_circle2(Mat src, int radius)
{
	Mat opening_circle;
	Mat se1 = getStructuringElement(MORPH_ELLIPSE, Size(radius,radius));
	morphologyEx(src,opening_circle, MORPH_OPEN, se1);
	return opening_circle;
}
//计算轮廓的圆的特性
float calculateCircularity2(vector<Point> contour)
{
	Point2f center;
	float radius = 0;
	minEnclosingCircle((Mat)contour, center, radius);
	//以最小外接圆半径作为数学期望，计算轮廓上各点到圆心距离的标准差
	float fsum = 0;
	float fcompare = 0;
	for (int i = 0; i<contour.size(); i++)
	{
		Point2f ptmp = contour[i];
		float fdistenct = sqrt((float)((ptmp.x - center.x)*(ptmp.x - center.x) + (ptmp.y - center.y)*(ptmp.y - center.y)));
		float fdiff = abs(fdistenct - radius);
		fsum = fsum + fdiff;
	}
	fcompare = fsum / (float)contour.size();
	return fcompare;
}
//select_shape
vector<vector<Point>> selectShapeCircularity(Mat src, Mat& draw, vector<vector<Point>> contours, float minvalue, float maxvalue)
{
	vector<vector<Point>> result_contours;
	draw = Mat::zeros(src.rows, src.cols, CV_8UC3);
	for (int i = 0; i<contours.size(); i++)
	{
		float fcompare = calculateCircularity2(contours[i]);
		if (fcompare >= minvalue && fcompare <= maxvalue)
		{
			result_contours.push_back(contours[i]);
		}
	}
	for (int i = 0; i<result_contours.size(); i++)
	{
		Scalar  color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(draw, result_contours, i, color, -1);
	}
	return result_contours;
}

int fatf::fat_detect(int jugement,Mat &img,fv_SParam &param)
{
	int hv_jugement = jugement;
	int hv_fatTmin = param.hv_fatTmin;
	int hv_fatTmax = param.hv_fatTmax;
	int hv_fatOpenT=param.hv_fatOpenT;
	int hv_diameter = param.hv_diameter;
	Mat ho_fat_Region;
	threshold(img, ho_fat_Region, hv_fatTmin, hv_fatTmax,0);
	hv_fatOpenT = hv_fatOpenT+ hv_fatOpenT;
	//圆形结构元素形态学处理
	Mat ho_fat_Open;
	Mat se1 = getStructuringElement(MORPH_ELLIPSE, Size(2*hv_fatOpenT,2*hv_fatOpenT), Point(-1, -1));
	morphologyEx(ho_fat_Region, ho_fat_Open, MORPH_OPEN, se1);
	//轮廓查找
	vector<vector<Point>>contours;
	findContours(ho_fat_Open,contours,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE,Point());
	for (size_t t = 0; t < contours.size(); t++)
	{
		if (contours[t].size() > 280)
		{
			Point2f center; float radius;
			minEnclosingCircle(contours[t], center, radius);
			// 最小外接轮廓
			//RotatedRect rrt = fitEllipse(contours[t]);
			//float w = rrt.size.height;
			//float hv_fat_smallRadius = w / 2;
			//float h = rrt.size.height;
			if (radius > hv_diameter) hv_jugement=1;
			//cout << "外接轮廓的宽:" << w << endl;
			//cout << "外接轮廓的高:" << h << endl;
			//Point centers = rrt.center;
			//cout << "椭圆中心" << centers << endl;
			//ellipse(gray_img, rrt, Scalar(0, 0, 255), 2, 8);
		}
	}

	return hv_jugement;
}

float fatf::fat_detect_test(int jugement, Mat & img, fv_SParam & param)
{
	int hv_jugement = jugement;
	int hv_fatTmin = param.hv_fatTmin;
	int hv_fatTmax = param.hv_fatTmax;
	int hv_fatOpenT = param.hv_fatOpenT;
	int hv_diameter = param.hv_diameter;
	Mat ho_fat_Region;
	threshold(img, ho_fat_Region, hv_fatTmin, hv_fatTmax, 0);
	hv_fatOpenT = hv_fatOpenT + hv_fatOpenT;
	//圆形结构元素形态学处理
	Mat ho_fat_Open;
	Mat se1 = getStructuringElement(MORPH_ELLIPSE, Size( hv_fatOpenT-1, hv_fatOpenT-1), Point(-1, -1));
	morphologyEx(ho_fat_Region, ho_fat_Open, MORPH_OPEN, se1);
	//轮廓查找
	vector<vector<Point>>contours;
	findContours(ho_fat_Open, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());
	//选取最大轮廓
	int temp=0;
	int sizes = 0;
	int index = 0;
	for (size_t t = 0; t < contours.size(); t++)
	{
		sizes = contours[t].size();
		if (sizes > temp)
		{
			temp = sizes;
			index = t;
		}
	}
		
		Point2f center; float radius;
		minEnclosingCircle(contours[index], center, radius);
			// 最小外接轮廓
			//RotatedRect rrt = fitEllipse(contours[t]);
			//float w = rrt.size.height;
			//float hv_fat_smallRadius = w / 2;
			//float h = rrt.size.height;
			//if (radius > hv_diameter) hv_jugement = 1;
	return radius;
}

int fatf::abonormalTestf(int jugement, Mat& img, fv_SParam & param)
{
	int hv_jugement=jugement;

	Mat ho_Region_Abnormal;
	int hv_AbnormalMinT = param.hv_AbnormalMinT;
	int hv_AbnormalMaxT = param.hv_AbnormalMaxT;
	int hv_AbnormalNumberJuge1 = param.hv_AbnormalNumberJuge1;
	int hv_AbnormalNumberJuge2 = param.hv_AbnormalNumberJuge2;
	int hv_AbnormalSelectLmin = param.hv_AbnormalSelectLmin;
	int hv_AbnormalSelectLmax = param.hv_AbnormalSelectLmax;
	int hv_AbnormalMAXArea = param.hv_AbnormalMAXArea;
	int hv_AbnormalMINArea = param.hv_AbnormalMINArea;

	threshold(img, ho_Region_Abnormal, hv_AbnormalMinT, hv_AbnormalMaxT,0);
	int hv_Height = ho_Region_Abnormal.rows;
	int hv_Width = ho_Region_Abnormal.cols;
	int img_area = hv_Height*hv_Width;
	//轮廓查找
	vector<vector<Point>>contours;
	vector<Vec4i>hierarchy;
	findContours(ho_Region_Abnormal, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());
	int hv_Abnormal_Number = 0;
	for (size_t t = 0; t < contours.size(); t++)
	{
		double hv_Abnormal_Area = contourArea(contours[t]);
		if ((hv_Abnormal_Area> hv_AbnormalSelectLmin)&&(hv_Abnormal_Area<hv_AbnormalSelectLmax))
		{
			if ((hv_Abnormal_Area > (img_area - hv_AbnormalMAXArea)) || hv_Abnormal_Area < hv_AbnormalMINArea)
				hv_jugement = 10;
			hv_Abnormal_Number++;
		}
	}
	if((hv_Abnormal_Number>hv_AbnormalNumberJuge1)||(hv_Abnormal_Number= hv_AbnormalNumberJuge2))
		hv_jugement = 10;//10为异常
	
	return hv_jugement;
}

int fatf::regionfOffLine(int jugement, Mat& imgM,Mat &imgV, fv_SParam & param)
{	
	int hv_jugement = jugement;
	int hv_bigDustFlagDTP = param.hv_bigDustFlagDTP;
	int hv_bigDustMeanTmax = param.hv_bigDustMeanTmax;
	int hv_bigDustMeanTmin = param.hv_bigDustMeanTmin;
	int hv_bigDustFlagClose = param.hv_bigDustFlagClose;
	int hv_bigDustSelectMin = param.hv_bigDustSelectMin;
	int hv_bigDustSelectMax = param.hv_bigDustSelectMax;
	int hv_eliminateDZT = param.hv_eliminateDZT;
	int hv_eliminateDZCenterTmax = param.hv_eliminateDZCenterTmax;
	int hv_eliminateDZCenterTmin = param.hv_eliminateDZCenterTmin;
	int hv_eliminateDZSlectTmax = param.hv_eliminateDZSlectTmax;
	int hv_eliminateDZSlectTmin = param.hv_eliminateDZSlectTmin;
	int hv_eliminate_aimCenterOpen = param.hv_eliminate_aimCenterOpen;
	int hv_bigDustDilate = param.hv_bigDustDilate;
	int hv_bigDustOpen = param.hv_bigDustOpen;
	int hv_DZWidth = param.hv_DZWidth;
	int hv_eliminate_DZLenth = param.hv_eliminate_DZLenth;
	int gausizesize = param.gausizesize;
	int gausizesigma = param.gausizesigma;
	//剔除道子区域
	Mat ho_Image_Median = imgM;
	//Mat ho_bigDust_FlagScaled;
	//equalizeHist(ho_Image_median, ho_bigDust_FlagScaled);
	Mat ho_bigDust_FlagMean;
	//blur(ho_Image_median, ho_bigDust_FlagMean, Size(hv_bigDustMeanTmin, hv_bigDustMeanTmax));
	medianBlur(ho_Image_Median, ho_bigDust_FlagMean, hv_bigDustMeanTmin);
	GaussianBlur(ho_bigDust_FlagMean, ho_bigDust_FlagMean, Size(gausizesize, gausizesize), gausizesigma, gausizesigma, 4);
	Mat ho_bigDust_FlagDyn;
	adaptiveThreshold(ho_bigDust_FlagMean, ho_bigDust_FlagDyn, 255, 0, THRESH_BINARY, hv_bigDustFlagDTP, 0);
	Mat ho_bigDust_Close;
	Mat se1 = getStructuringElement(MORPH_ELLIPSE, Size(hv_bigDustFlagClose, hv_bigDustFlagClose));
	morphologyEx(ho_bigDust_FlagDyn, ho_bigDust_Close, MORPH_OPEN, se1);
	//连通域分析
	vector<vector<Point>> ho_bigDust_Connect;
	Mat draw_big_dust;
	ho_bigDust_Connect = connection2(ho_bigDust_Close, draw_big_dust);
	//按照面积筛选
	Mat draw_area;
	vector<vector<Point>> ho_bigDust_Select;
	ho_bigDust_Select = selectShapeArea2(ho_bigDust_Close, draw_area,ho_bigDust_Connect, hv_bigDustSelectMin, hv_bigDustSelectMax);
	
	//提取中心
	Mat ho_eliminate_CenterMean;
	Mat ho_Image_AllV = imgV;
	blur(ho_Image_AllV, ho_eliminate_CenterMean, Size(hv_eliminateDZT, hv_eliminateDZT));
	Mat ho_eliminate_CenterRegion;
	threshold(ho_eliminate_CenterMean, ho_eliminate_CenterRegion, hv_eliminateDZCenterTmin, hv_eliminateDZCenterTmax,0);
	vector<vector<Point>> ho_eliminate_CenterConnect;
	Mat draw_big_dust1;
	ho_eliminate_CenterConnect = connection2(ho_eliminate_CenterRegion, draw_big_dust1);
	Mat draw_area1;
	vector<vector<Point>> ho_eliminate_aimCenterSelect;
	ho_eliminate_aimCenterSelect = selectShapeArea2(ho_bigDust_Close, draw_area1, ho_eliminate_CenterConnect, hv_eliminateDZSlectTmin, hv_eliminateDZSlectTmax);
	int hv_eliminate_CenterNumber = ho_eliminate_aimCenterSelect.size();
	if (hv_eliminate_CenterNumber == 1)
	{
		//求灯珠中心
		Point centers;
		Rect rect = boundingRect(ho_eliminate_aimCenterSelect[0]);
		float hv_eliminate_CenterRow = rect.y;
		float hv_eliminate_CenterColumn = rect.x;
		Mat eliminate_aimCenterSelect;
		eliminate_aimCenterSelect = Mat::zeros(ho_Image_AllV.size(), CV_8UC1);
		drawContours(eliminate_aimCenterSelect, ho_eliminate_aimCenterSelect, 0, 255, -1, 8);

		Mat se_aimcenter = getStructuringElement(MORPH_ELLIPSE, Size(hv_eliminate_aimCenterOpen, hv_eliminate_aimCenterOpen));
		morphologyEx(eliminate_aimCenterSelect, eliminate_aimCenterSelect, MORPH_OPEN, se_aimcenter);

		Mat se_aimcenter1 = getStructuringElement(MORPH_ELLIPSE, Size(2 * hv_bigDustDilate, 2 * hv_bigDustDilate));
		Mat ho_bigDust_Dilation;
		dilate(eliminate_aimCenterSelect, ho_bigDust_Dilation, se_aimcenter1,Point(-1,-1), 1,0);
		Mat ho_bigDust_Dif;

		Mat bigDust_Select;
		bigDust_Select = Mat::zeros(ho_Image_AllV.size(), CV_8UC1);
		for (int i = 0; i < ho_bigDust_Select.size(); i++) 
		{
			drawContours(bigDust_Select, ho_bigDust_Select, i, 255, -1, 8);
		}
		
		addWeighted(bigDust_Select, 1, ho_bigDust_Dilation, -1, 0, ho_bigDust_Dif);

		Mat se_aimcenter3 = getStructuringElement(MORPH_ELLIPSE, Size(2 * hv_bigDustOpen, 2 * hv_bigDustOpen));
		Mat ho_bigDust_CicleOpen;
		morphologyEx(ho_bigDust_Dif, ho_bigDust_CicleOpen, MORPH_OPEN, se_aimcenter3);

		Mat ho_bigDust_Intersect = ho_bigDust_CicleOpen;
		//道子区域掩膜
		Mat ho_eliminate_RegionUnion;
		//***********特殊区域检测*********************
		//hv_jugement=specialROIfOffLine()
		//***************************************
		if(hv_jugement==0)
		{
			//************去除屏蔽区域*************
			vector<vector<Point>> ho_eliminateDZ_Connect;
			ho_eliminateDZ_Connect = connection3(ho_bigDust_Intersect);
			//筛选出道子区域
			RotatedRect rrt;
			float hv_eliminateDZ_Row;
			float hv_eliminateDZ_Column;
			float hv_eliminateDZ_Phi;
			float hv_eliminateDZ_Length1;
			float hv_eliminateDZ_Length2;
			float k = 0;
			float line_arctan;
			double rate = 0;
			//利用淹没获取道子区域
			
			ho_eliminate_RegionUnion = Mat::zeros(ho_bigDust_Intersect.size(), CV_8UC1);
			for (int hv_i = 0; hv_i < ho_eliminateDZ_Connect.size(); hv_i++)
			{
				rrt = minAreaRect(ho_eliminateDZ_Connect[hv_i]);
				hv_eliminateDZ_Row = rrt.center.y;
				hv_eliminateDZ_Column = rrt.center.x;
				hv_eliminateDZ_Phi = rrt.angle;
				hv_eliminateDZ_Length1 = rrt.size.height;
				hv_eliminateDZ_Length2 = rrt.size.width;
				//纵横比
				rate = hv_eliminateDZ_Length2 / hv_eliminateDZ_Length1;
				//根据长度和宽度进行筛选，opencv中长非长，宽非宽，取俩者的最值即可
				if ((min(hv_eliminateDZ_Length2, hv_eliminateDZ_Length1)<hv_DZWidth)&&
					(max(hv_eliminateDZ_Length1, hv_eliminateDZ_Length2)>hv_eliminate_DZLenth))
				{					
					k = (hv_eliminate_CenterRow - hv_eliminateDZ_Row)/ (hv_eliminate_CenterColumn - hv_eliminateDZ_Column);
					//最小外接矩形的质心与灯珠中心连线的夹角
					line_arctan = atan(k);
					//角度计算
					float angle1 = line_arctan*57.3;
					if ((angle1 > 0 && rate < 0.66) || (angle1 <= 0 && rate > 1.5))
					{
						if (angle1 > 0) angle1 = angle1 - 90;


						//cout << "道子中心与圆心的角度:" << angle1 << endl;
						float angle_diff = fabs(angle1 - hv_eliminateDZ_Phi);
						if (angle_diff < 23)
						{
							drawContours(ho_eliminate_RegionUnion, ho_eliminateDZ_Connect, hv_i, 255, -1, 8);
						}
					}
				}
			}
			//相当于reducce_domain;
			Mat ho_eliminate_Reduce;
			ho_Image_AllV.copyTo(ho_eliminate_Reduce, ho_eliminate_RegionUnion);
			double hv_eliminate_Min, hv_eliminate_Max;
			Point minLoc, maxLoc;
			minMaxLoc(ho_eliminate_Reduce, &hv_eliminate_Min, &hv_eliminate_Max, &minLoc, &maxLoc);
			hv_jugement = DZJudgeF(hv_jugement, ho_eliminate_Reduce, hv_eliminate_Min, param);
		}
		//********************内尘检测*******************
		//内尘检测
		int hv_DZeliminateOpen = param.hv_DZeliminateOpen;
		if (hv_jugement == 0) 
		{
			//ho_eliminate_RegionUnion = ho_eliminate_RegionUnion;
			Mat ho_bigDust_eliminateDif;
			subtract(ho_bigDust_Intersect, ho_eliminate_RegionUnion, ho_bigDust_eliminateDif);
			//addWeighted(ho_bigDust_Intersect, 1, ho_eliminate_RegionUnion, -1, 0, ho_bigDust_eliminateDif);
			Mat ho_bigDust_Intersect;
			ho_bigDust_Intersect = opening_circle2(ho_bigDust_eliminateDif, hv_DZeliminateOpen);
			//用掩膜获取
			Mat ho_bigDust_Reduce;
			ho_Image_Median.copyTo(ho_bigDust_Reduce, ho_bigDust_Intersect);
			double hv_bigDust_Min, hv_bigDust_Max;
			Point minLoc, maxLoc;
			minMaxLoc(ho_bigDust_Reduce, &hv_bigDust_Min, &hv_bigDust_Max, &minLoc, &maxLoc);
			//内尘杂光判定
			hv_jugement = DustJudgeF(hv_jugement, ho_bigDust_Reduce, hv_bigDust_Min, param);
		}

	}
	else
	{
		hv_jugement = 10;
	}
	return hv_jugement;
}

int fatf::DZJudgeF(int jugement, Mat & img, int eliminate_Min, fv_SParam & param)
{

	int hv_jugement = jugement;
	int hv_eliminate_Min = eliminate_Min;
	int hv_eliminateTmin = param.hv_eliminateTmin;
	int hv_eliminateTmax = param.hv_eliminateTmax;
	int hv_eliminateClose = param.hv_eliminateClose;
	int hv_eliminateSmax = param.hv_eliminateSmax;
	int hv_eliminateSmin = param.hv_eliminateSmin;
	int hv_eliminate_judgeCountShort = param.hv_eliminate_judgeCountShort;
	int hv_eliminate_judgeCountLong = param.hv_eliminate_judgeCountLong;
	int hv_eliminate_judgeShort = param.hv_eliminate_judgeShort;
	int hv_eliminate_judgeLong = param.hv_eliminate_judgeLong;
	float hv_eliminate_L1W = param.hv_eliminate_L1W;
	float hv_eliminate_L2W = param.hv_eliminate_L2W;
	int hv_eliminate_jugeL1 = param.hv_eliminate_jugeL1;
	int hv_eliminate_jugeL2 = param.hv_eliminate_jugeL2;
	if (hv_jugement == 0)
	{
		int hv_eliminateFTmin = hv_eliminate_Min + hv_eliminateTmin;
		if (hv_eliminateTmin>(hv_eliminateTmax - hv_eliminate_Min))
			hv_eliminateFTmin = hv_eliminateTmax - hv_eliminateTmin;
		Mat ho_eliminate_FRegion;
		Mat ho_eliminate_Reduce = img;
		threshold(ho_eliminate_Reduce, ho_eliminate_FRegion, hv_eliminateFTmin, hv_eliminateTmax, 0);
		Mat ho_eliminate_RegionClose;
		ho_eliminate_RegionClose = closing_circle2(ho_eliminate_FRegion, hv_eliminateClose);
		vector<vector<Point>> ho_eliminate_FConnect;
		ho_eliminate_FConnect = connection3(ho_eliminate_RegionClose);
		vector<vector<Point>> ho_eliminate_SelectedRe;
		ho_eliminate_SelectedRe = selectShapeArea3(ho_eliminate_FConnect, hv_eliminateSmin, hv_eliminateSmax);
		RotatedRect rrt;
		float weight = 0;
		float height = 0;
		float hv_eliminate_FLength1 = 0;
		float hv_eliminate_FLength2 = 0;
		int hv_eliminate_CountShort = 0;
		int hv_eliminate_CountLong = 0;
		for (int i = 0; i < ho_eliminate_SelectedRe.size(); i++)
		{
			rrt = minAreaRect(ho_eliminate_SelectedRe[i]);
			weight = rrt.size.width;
			height = rrt.size.height;
			hv_eliminate_FLength1 = max(weight, height);
			hv_eliminate_FLength2 = min(weight, height);
			if (hv_eliminate_FLength1 > hv_eliminate_judgeCountShort)
				hv_eliminate_CountShort += 1;
			if (hv_eliminate_FLength1>hv_eliminate_judgeCountLong)
				hv_eliminate_CountLong += 1;

			hv_eliminate_FLength1 = hv_eliminate_L1W * (hv_eliminate_FLength1 / (2 * hv_eliminate_jugeL1));
			hv_eliminate_FLength2 = hv_eliminate_L2W*(hv_eliminate_FLength2 / (2 * hv_eliminate_jugeL2));
			if ((hv_eliminate_FLength1 >= 0.5) && (hv_eliminate_FLength2 <= 0.5))
				//道子
				hv_jugement = 3;
		}
		int hv_eliminateOUT_CountShort = hv_eliminate_CountShort;
		int hv_eliminateOUT_CountLong = hv_eliminate_CountLong;

		if (hv_eliminate_CountShort>hv_eliminate_judgeShort)
			//多道子
			hv_jugement = 5;
		if (hv_eliminateOUT_CountLong > hv_eliminate_judgeLong)
			//长道子
			hv_jugement = 4;
	}
	return hv_jugement;
}

int fatf::DustJudgeF(int jugement, Mat & img, int bigDust_Min, fv_SParam & param)
{
	int hv_jugement = jugement;
	int hv_bigDust_Min = bigDust_Min;
	int hv_bigDustTmin = param.hv_bigDustTmin;
	int hv_bigDustTmax = param.hv_bigDustTmax;
	int hv_bigDustW = param.hv_bigDustW;
	int hv_jugeBIGDustU = param.hv_jugeBIGDustU;
	Mat ho_bigDust_Reduce = img;

	if (hv_jugement == 0) 
	{
		int hv_bigDustFTmin=0;
		hv_bigDustFTmin = hv_bigDust_Min + hv_bigDustTmin;
		if(hv_bigDustTmin>(hv_bigDustTmax - hv_bigDust_Min))
			hv_bigDustFTmin = hv_bigDustTmax - hv_bigDustTmin;
		Mat ho_bigDust_FRegion;
		threshold(ho_bigDust_Reduce, ho_bigDust_FRegion, hv_bigDustFTmin, hv_bigDustTmax, 0);
		vector<vector<Point>> dust_contours;
		dust_contours = connection3(ho_bigDust_FRegion);
		double hv_bigDust_Area = 0;
		for (int i = 0; i < dust_contours.size(); i++) 
		{
			hv_bigDust_Area+=contourArea(dust_contours[i]);
		}
		double hv_BIGDustU = hv_bigDust_Area;
		hv_bigDust_Area = hv_bigDustW*(hv_bigDust_Area / (2 * hv_jugeBIGDustU));
		if (hv_bigDust_Area >= 0.5)
			hv_jugement = 2;
	}
	return hv_jugement;
}

int DZJudgeF(int jugement,Mat &img,int eliminate_Min , fv_SParam &param)
{
	int hv_jugement = jugement;
	int hv_eliminate_Min = eliminate_Min;
	int hv_eliminateTmin = param.hv_eliminateTmin;
	int hv_eliminateTmax = param.hv_eliminateTmax;
	int hv_eliminateClose = param.hv_eliminateClose;
	int hv_eliminateSmax = param.hv_eliminateSmax;
	int hv_eliminateSmin = param.hv_eliminateSmin;
	int hv_eliminate_judgeCountShort = param.hv_eliminate_judgeCountShort;
	int hv_eliminate_judgeCountLong = param.hv_eliminate_judgeCountLong;
	int hv_eliminate_judgeShort = param.hv_eliminate_judgeShort;
	int hv_eliminate_judgeLong = param.hv_eliminate_judgeLong;
	float hv_eliminate_L1W = param.hv_eliminate_L1W;
	float hv_eliminate_L2W = param.hv_eliminate_L2W;
	int hv_eliminate_jugeL1 = param.hv_eliminate_jugeL1;
	int hv_eliminate_jugeL2 = param.hv_eliminate_jugeL2;
	if (hv_jugement == 0) 
	{
		int hv_eliminateFTmin= hv_eliminate_Min + hv_eliminateTmin;
		if(hv_eliminateTmin>(hv_eliminateTmax - hv_eliminate_Min))
			hv_eliminateFTmin = hv_eliminateTmax - hv_eliminateTmin;
		Mat ho_eliminate_FRegion;
		Mat ho_eliminate_Reduce = img;
		threshold(ho_eliminate_Reduce, ho_eliminate_FRegion, hv_eliminateFTmin, hv_eliminateTmax,0);
		Mat ho_eliminate_RegionClose;
		ho_eliminate_RegionClose = closing_circle2(ho_eliminate_FRegion, hv_eliminateClose);
		vector<vector<Point>> ho_eliminate_FConnect;
		ho_eliminate_FConnect = connection3(ho_eliminate_RegionClose);
		vector<vector<Point>> ho_eliminate_SelectedRe;
		ho_eliminate_SelectedRe=selectShapeArea3(ho_eliminate_FConnect, hv_eliminateSmin, hv_eliminateSmax);
		RotatedRect rrt;
		float weight=0;
		float height=0;
		float hv_eliminate_FLength1=0;
		float hv_eliminate_FLength2 = 0;
		int hv_eliminate_CountShort = 0;
		int hv_eliminate_CountLong = 0;
		for (int i = 0; i < ho_eliminate_SelectedRe.size(); i++)
		{
			rrt = minAreaRect(ho_eliminate_SelectedRe[i]);
			weight = rrt.size.width;
			height = rrt.size.height;
			hv_eliminate_FLength1 = max(weight, height);
			hv_eliminate_FLength2 = min(weight, height);
			if (hv_eliminate_FLength1 > hv_eliminate_judgeCountShort)
				hv_eliminate_CountShort+=1;
			if(hv_eliminate_FLength1>hv_eliminate_judgeCountLong)
				hv_eliminate_CountLong += 1;

			hv_eliminate_FLength1 = hv_eliminate_L1W * (hv_eliminate_FLength1 / (2 * hv_eliminate_jugeL1));
			hv_eliminate_FLength2 = hv_eliminate_L2W*(hv_eliminate_FLength2 / (2 * hv_eliminate_jugeL2));
			if ((hv_eliminate_FLength1 >= 0.5) && (hv_eliminate_FLength2 <= 0.5))
				//道子
				hv_jugement = 3;
		}
		int hv_eliminateOUT_CountShort = hv_eliminate_CountShort;
		int hv_eliminateOUT_CountLong = hv_eliminate_CountLong;

		if(hv_eliminate_CountShort>hv_eliminate_judgeShort)
			//多道子
			hv_jugement = 5;
		if (hv_eliminateOUT_CountLong > hv_eliminate_judgeLong)
			//长道子
			hv_jugement = 4;	
	}
	return hv_jugement;
}