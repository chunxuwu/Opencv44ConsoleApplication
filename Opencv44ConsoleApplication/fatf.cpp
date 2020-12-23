#include "stdafx.h"
#include "fatf.h"

RNG rng(12345);
//带有上下限的threshold
void threshold2(Mat & gray, Mat& thresh, int minvalue, int maxvalue)
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
vector<vector<Point>>  selectShapeArea2(Mat &src, Mat& draw, vector<vector<Point>> contours, int minvalue, int maxvalue)
{
	vector<vector<Point>> result_contours;
	draw = Mat::zeros(src.rows, src.cols, CV_8UC3);
	for (int i = 0; i<contours.size(); i++)
	{
		double countour_area = contourArea(contours[i]);
		if (countour_area >minvalue && countour_area<maxvalue)
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

vector<vector<Point>>  selectShapeArea3(vector<vector<Point>> contours, int minvalue, int maxvalue)
{
	vector<vector<Point>> result_contours;
	for (int i = 0; i<contours.size(); i++)
	{
		double countour_area = contourArea(contours[i]);
		if (countour_area >minvalue && countour_area<maxvalue)
		{
			result_contours.push_back(contours[i]);
		}
	}
	return result_contours;
}

Mat closing_circle2(Mat & src,int radius ) 
{
	Mat closing_circle;
	Mat se1 = getStructuringElement(MORPH_ELLIPSE, Size(radius, radius));
	morphologyEx(src, closing_circle, MORPH_CLOSE, se1);
	return closing_circle;
}

Mat opening_circle2(Mat & src, int radius)
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

//写入数据
void write_date(double a)
{
	ofstream write;
	ifstream read;
	write.open("C:\\Users\\czwucx\\Desktop/detail_data.txt",ios::app);
	write << a << "\t";
	write.close();
	read.close();
}
void next_line() 
{
	ofstream write;
	ifstream read;
	write.open("C:\\Users\\czwucx\\Desktop/detail_data.txt", ios::app);
	write << endl;
	write.close();
	read.close();
}
int fatf::fat_detect(int jugement,Mat &img,fv_SParam &param)
{
	int hv_jugement = jugement;
	int hv_fatTmin = param.hv_fatTmin;
	int hv_fatTmax = param.hv_fatTmax;
	//int hv_fatOpenT=param.hv_fatOpenT;
	int hv_diameter = param.hv_diameter;
	Mat ho_fat_Region;
	threshold(img, ho_fat_Region, hv_fatTmin, hv_fatTmax,0);
	//hv_fatOpenT = hv_fatOpenT+ hv_fatOpenT;
	//double t, t1, t2, t3, t4, t5;
	//t = getTickCount();
	//圆形结构元素形态学处理	
	//Mat ho_fat_Open;
	//Mat se1 = getStructuringElement(MORPH_ELLIPSE, Size(2*hv_fatOpenT-1,2*hv_fatOpenT-1), Point(-1, -1));
	//morphologyEx(ho_fat_Region, ho_fat_Open, MORPH_OPEN, se1);
	//轮廓查找
	//t1 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
	vector<vector<Point>>contours;
	findContours(ho_fat_Region,contours,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE,Point());
	//选取最大轮廓
	int temp = 0;
	size_t sizes = 0;
	size_t index = 0;
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
	//t2 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
	//cout << "光源发胖形态学耗时：" << t1 - t << endl;
	//cout << "计算灯珠半径耗时：" << t2 - t << endl;
	RotatedRect fe =  fitEllipse(contours[index]);
	//float height = 
	float width = fe.size.width;
	//write_date(radius);
	//next_line();
	if (width/2 > hv_diameter)
	{
		hv_jugement = 1;
		//next_line();
	}
	return hv_jugement;
}

float fatf::fat_detect_test(int jugement, Mat & img, fv_SParam & param)
{
	int hv_jugement = jugement;
	int hv_fatTmin = param.hv_fatTmin;
	int hv_fatTmax = param.hv_fatTmax;
	//int hv_fatOpenT = param.hv_fatOpenT;
	int hv_diameter = param.hv_diameter;
	double t, t1,t2,t3,t4,t5;
	t = getTickCount();
	Mat ho_fat_Region;
	threshold(img, ho_fat_Region, hv_fatTmin, hv_fatTmax, 0);
	t1 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
	cout << "阈值分割时间：" << t1 - t << endl;
	//hv_fatOpenT = hv_fatOpenT + hv_fatOpenT;
	//圆形结构元素形态学处理
	//Mat ho_fat_Open;
	//Mat se1 = getStructuringElement(MORPH_ELLIPSE, Size( hv_fatOpenT-1, hv_fatOpenT-1), Point(-1, -1));
	//morphologyEx(ho_fat_Region, ho_fat_Open, MORPH_OPEN, se1);
	//轮廓查找

	vector<vector<Point>>contours;
	t2 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
	findContours(ho_fat_Region, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());
	t3 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
	cout << "连通域分析耗时：" << t3 - t2 << endl;
	//选取最大轮廓
	int temp=0;
	size_t sizes = 0;
	size_t index = 0;
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
	t4 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
	minEnclosingCircle(contours[index], center, radius);
	t5 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
	cout << "最小外接圆耗时：" << t5 - t4 << endl;
	return radius;
	//return flag;
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
	
	if (jugement == 0)
	{
		int hv_jugement = jugement;
		int hv_bigDustFlagDTP = param.hv_bigDustFlagDTP;
		int hv_bigDustMeanTmax = param.hv_bigDustMeanTmax;
		int hv_bigDustMeanTmin = param.hv_bigDustMeanTmin;
		int hv_bigDustFlagClose = param.hv_bigDustFlagClose;
		int hv_bigDustFlagOpen = param.hv_bigDustFlagOpen;
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
		int gausizesigma1 = param.gausizesigma1;
		int DZ_angle = param.DZ_angle;
		float adaptive_C = param.adaptive_C;
		//剔除道子区域
		Mat ho_Image_Median = imgM;
		//Mat ho_bigDust_FlagScaled;
		//equalizeHist(ho_Image_median, ho_bigDust_FlagScaled);
		Mat ho_bigDust_FlagMean;
		//blur(ho_Image_median, ho_bigDust_FlagMean, Size(hv_bigDustMeanTmin, hv_bigDustMeanTmax));
		//medianBlur(ho_Image_Median, ho_bigDust_FlagMean, hv_bigDustMeanTmin);
		GaussianBlur(ho_Image_Median, ho_bigDust_FlagMean, Size(gausizesize, gausizesize), gausizesigma1, gausizesigma1, 4);
		Mat ho_bigDust_FlagDyn;
		adaptiveThreshold(ho_bigDust_FlagMean, ho_bigDust_FlagDyn, 255, 0, THRESH_BINARY, hv_bigDustFlagDTP,adaptive_C);
		//circle(ho_bigDust_FlagDyn, Point(1506, 510), 60.5, 0, -1, 8);
		Mat ho_bigDust_Close;
		Mat se1 = getStructuringElement(MORPH_ELLIPSE, Size(hv_bigDustFlagOpen, hv_bigDustFlagOpen));
		Mat se2 = getStructuringElement(MORPH_ELLIPSE, Size(hv_bigDustFlagClose, hv_bigDustFlagClose));
		Mat ho_bigDust_open;
		morphologyEx(ho_bigDust_FlagDyn, ho_bigDust_open, MORPH_CLOSE, se1);
		morphologyEx(ho_bigDust_open, ho_bigDust_Close, MORPH_OPEN, se2);
		//连通域分析
		vector<vector<Point>> ho_bigDust_Connect;
		Mat draw_big_dust;
		ho_bigDust_Connect = connection2(ho_bigDust_Close, draw_big_dust);
		//按照面积筛选
		Mat draw_area;
		vector<vector<Point>> ho_bigDust_Select;
		ho_bigDust_Select = selectShapeArea2(ho_bigDust_Close, draw_area, ho_bigDust_Connect, hv_bigDustSelectMin, hv_bigDustSelectMax);

		//提取中心光源
		Mat ho_eliminate_CenterMean;
		Mat ho_Image_AllV = imgV;
		blur(ho_Image_AllV, ho_eliminate_CenterMean, Size(hv_eliminateDZT, hv_eliminateDZT));
		Mat ho_eliminate_CenterRegion;
		threshold(ho_eliminate_CenterMean, ho_eliminate_CenterRegion, hv_eliminateDZCenterTmin, hv_eliminateDZCenterTmax, 0);
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
			Point2f center; float radius;
			minEnclosingCircle(ho_eliminate_aimCenterSelect[0], center, radius);
			//Rect rect = boundingRect(ho_eliminate_aimCenterSelect[0]);

			float hv_eliminate_CenterRow = center.y;
			float hv_eliminate_CenterColumn = center.x;
			Mat eliminate_aimCenterSelect;
			eliminate_aimCenterSelect = Mat::zeros(ho_Image_AllV.size(), CV_8UC1);
			drawContours(eliminate_aimCenterSelect, ho_eliminate_aimCenterSelect, 0, 255, -1, 8);

			Mat se_aimcenter = getStructuringElement(MORPH_ELLIPSE, Size(hv_eliminate_aimCenterOpen, hv_eliminate_aimCenterOpen));
			morphologyEx(eliminate_aimCenterSelect, eliminate_aimCenterSelect, MORPH_OPEN, se_aimcenter);

			Mat se_aimcenter1 = getStructuringElement(MORPH_ELLIPSE, Size( 2*hv_bigDustDilate, 2*hv_bigDustDilate));
			Mat ho_bigDust_Dilation;
			dilate(eliminate_aimCenterSelect, ho_bigDust_Dilation, se_aimcenter1, Point(-1, -1), 1, 0);
			Mat ho_bigDust_Dif;

			Mat bigDust_Select;
			bigDust_Select = Mat::zeros(ho_Image_AllV.size(), CV_8UC1);
			for (int i = 0; i < ho_bigDust_Select.size(); i++)
			{
				drawContours(bigDust_Select, ho_bigDust_Select, i, 255, -1, 8);
			}

			addWeighted(bigDust_Select, 1, ho_bigDust_Dilation, -1, 0, ho_bigDust_Dif);

			Mat se_aimcenter3 = getStructuringElement(MORPH_ELLIPSE, Size( hv_bigDustOpen, hv_bigDustOpen));
			Mat ho_bigDust_CicleOpen;
			morphologyEx(ho_bigDust_Dif, ho_bigDust_CicleOpen, MORPH_CLOSE, se_aimcenter3);

			Mat ho_bigDust_Intersect = ho_bigDust_CicleOpen;
			//道子区域掩膜
			Mat ho_eliminate_RegionUnion;
			//***********特殊区域检测*********************
			//hv_jugement=specialROIfOffLine()
			//***************************************
			if (hv_jugement == 0)
			{
				//************去除屏蔽区域*************
				vector<vector<Point>> ho_eliminateDZ_Connect;
				ho_eliminateDZ_Connect = connection3(ho_bigDust_Intersect);
				//筛选出道子区域
				RotatedRect rrt;
				float hv_eliminateDZ_Row;
				float hv_eliminateDZ_Column;
				float hv_eliminateDZ_Phi;
				double hv_eliminateDZ_Length1;
				double hv_eliminateDZ_Length2;
				float k = 0;
				float line_arctan;
				double rate = 0;
				//利用掩膜获取道子区域
				double length1 = 0;
				double length2 = 0;
				ho_eliminate_RegionUnion = Mat::zeros(ho_bigDust_Intersect.size(), CV_8UC1);
				//#pragma omp parallel for num_threads(10)
				for (int hv_i = 0; hv_i < ho_eliminateDZ_Connect.size(); hv_i++)
				{
					rrt = minAreaRect(ho_eliminateDZ_Connect[hv_i]);
					hv_eliminateDZ_Row = rrt.center.y;
					hv_eliminateDZ_Column = rrt.center.x;
					hv_eliminateDZ_Phi = rrt.angle;
					
					//cout << hv_eliminateDZ_Phi << endl;
					length1 = rrt.size.height;
					length2 = rrt.size.width;
					hv_eliminateDZ_Length1 = min(length1, length2);
					hv_eliminateDZ_Length2 = max(length1, length2);
					//纵横比
					rate = length2 / length1;
					float eliminateDZ_Phi;
					eliminateDZ_Phi = hv_eliminateDZ_Phi;
					//根据长度和宽度进行筛选，opencv中长非长，宽非宽，取俩者的最值即可
					if ((hv_eliminateDZ_Length1<hv_DZWidth) && (hv_eliminateDZ_Length2>hv_eliminate_DZLenth))
					{
						k = (hv_eliminate_CenterRow - hv_eliminateDZ_Row) / (hv_eliminate_CenterColumn - hv_eliminateDZ_Column);
						//最小外接矩形的质心与灯珠中心连线的夹角
						line_arctan = atan(k);
						//角度计算
						double angle1 = line_arctan*57.3;
						//检查垂直的道子
						//检测外接矩形在第二象限，而中心直线在第一象限的特殊临界道子
						//if ((90 + angle1) < 5 && rate < 0.66 && fabs(hv_eliminateDZ_Phi) <10) 
						//{
						//	eliminateDZ_Phi = -(90 + hv_eliminateDZ_Phi);
						//	rate = 1 / rate;
						//}
						//if ((hv_eliminateDZ_Phi == 0) && rate < 0.66)
						//{
						//	rate = 1 / rate;
						//	eliminateDZ_Phi = -90;
						//}
						//if ((hv_eliminateDZ_Phi == -90) && rate > 2)
						//{
						//	rate = 1 / rate;
						//	eliminateDZ_Phi = 0;
						//}
						////检查水平的道子
						//if (fabs(angle1)<20&& eliminateDZ_Phi == -90) eliminateDZ_Phi = 0;
						//if ( (angle1<0)&&(rate >1.5)||(angle1>0&&(rate<0.66)))
						//{
						//	
						//	float angle_diff = 0;
						//	if (angle1<0)
						//	{								
						//		angle_diff = angle1 - eliminateDZ_Phi;
						//	}
						//	else 
						//	{
						//		angle_diff = 90-(angle1 -eliminateDZ_Phi);
						//	}
							//cout << "道子中心与圆心的角度:" << angle1 << endl;
							//cout << angle_diff << endl;
							//cout << angle_diff << endl;
						double angle_diff=30;
						double abs_angle = abs(angle1);
						if(abs_angle<10&& (hv_eliminateDZ_Phi==0&&rate>1.618)||(hv_eliminateDZ_Phi==-90)&&rate<0.618)
						{
							angle_diff = abs(angle1);
						}else if(abs_angle>84&&((hv_eliminateDZ_Phi==0&&rate<0.618)||(hv_eliminateDZ_Phi==-90&&rate>1.618)))
						{
							angle_diff = 90 - abs_angle;
						}
						else if (angle1<0 && rate>1.618) 
						{
							angle_diff = abs(angle1 - hv_eliminateDZ_Phi);
						}else if(angle1>0&&rate<0.618)
						{
							angle_diff = abs(90 - angle1 + hv_eliminateDZ_Phi);
						}
						else if (abs_angle > 85 && rate < 0.66&&abs(hv_eliminateDZ_Phi) < 6) {
							angle_diff = abs(90 - abs_angle + hv_eliminateDZ_Phi);
						}
						else if (abs_angle > 85 && rate > 1.5&&hv_eliminateDZ_Phi < -85) {
							angle_diff = abs(angle1 + hv_eliminateDZ_Phi);
						}
						else if (hv_eliminateDZ_Phi > -6 && rate > 1.5&&angle1 > 0 && angle1 < 6) {
							angle_diff = abs(angle1 + hv_eliminateDZ_Phi);
						}
						else if ((angle1<0) && (angle1>-6) && (hv_eliminateDZ_Phi < -84) && (rate < 0.618)) {
							angle_diff = abs(90 - angle1 + hv_eliminateDZ_Phi);
						}

						if (angle_diff < DZ_angle)
						{
							drawContours(ho_eliminate_RegionUnion, ho_eliminateDZ_Connect, hv_i, 255, -1, 8);
						}
						
					}
				}
				//相当于reducce_domain;
				Mat ho_eliminate_Reduce;
				ho_Image_AllV.copyTo(ho_eliminate_Reduce, ho_eliminate_RegionUnion);
				double hv_eliminate_Min=33, hv_eliminate_Max;
				Point minLoc, maxLoc;
				//minMaxLoc(ho_eliminate_Reduce, &hv_eliminate_Min, &hv_eliminate_Max, &minLoc, &maxLoc);	
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
	}else { return jugement; }	
}

int fatf::regionfOffLine_test(int jugement, Mat & img, Mat & imgV, fv_SParam & param)
{
	if (jugement == 0)
	{
		int hv_jugement = jugement;
		int hv_bigDustFlagDTP = param.hv_bigDustFlagDTP;
		int hv_bigDustMeanTmax = param.hv_bigDustMeanTmax;
		int hv_bigDustMeanTmin = param.hv_bigDustMeanTmin;
		int hv_bigDustFlagClose = param.hv_bigDustFlagClose;
		int hv_bigDustFlagOpen = param.hv_bigDustFlagOpen;
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
		int gausizesigma1 = param.gausizesigma1;
		int DZ_angle = param.DZ_angle;
		float adaptive_C = param.adaptive_C;
		//剔除道子区域
		double t, t1, t2, t3, t4, t5, t6, t7;
		t = (double)getTickCount();
		Mat ho_Image_Median = img;
		//Mat ho_bigDust_FlagScaled;
		//equalizeHist(ho_Image_median, ho_bigDust_FlagScaled);
		Mat ho_bigDust_FlagMean;
		//blur(ho_Image_median, ho_bigDust_FlagMean, Size(hv_bigDustMeanTmin, hv_bigDustMeanTmax));
		//medianBlur(ho_Image_Median, ho_bigDust_FlagMean, hv_bigDustMeanTmin);
		t6 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
		GaussianBlur(ho_Image_Median, ho_bigDust_FlagMean, Size(gausizesize, gausizesize), gausizesigma1, gausizesigma1, 4);
		t7 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
		cout << "高斯滤波耗时：" << t7 - t6 << endl;
		Mat ho_bigDust_FlagDyn;
		t6 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
		adaptiveThreshold(ho_bigDust_FlagMean, ho_bigDust_FlagDyn, 255, 0, THRESH_BINARY, hv_bigDustFlagDTP, adaptive_C);
		t7 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
		cout << "动态阈值分割" << t7 - t6 << endl;
		circle(ho_bigDust_FlagDyn, Point(1506, 510), 60.5, 0, -1, 8);
		//Mat ho_bigDust_close;
		//Mat se1 = getStructuringElement(MORPH_ELLIPSE, Size(hv_bigDustFlagOpen, hv_bigDustFlagOpen));
		Mat se2 = getStructuringElement(MORPH_ELLIPSE, Size(hv_bigDustFlagClose, hv_bigDustFlagClose));
		Mat ho_bigDust_open;
		t6 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
		morphologyEx(ho_bigDust_FlagDyn, ho_bigDust_open, MORPH_CLOSE, se2);
		t7 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
		cout << "形态学闭操作耗时：" << t7 - t6 << endl;
		//morphologyEx(ho_bigDust_open, ho_bigDust_close, MORPH_OPEN, se2);
		t6 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
		//连通域分析
		vector<vector<Point>> ho_bigDust_Connect;
		Mat draw_big_dust;
		ho_bigDust_Connect = connection2(ho_bigDust_open, draw_big_dust);
		//按照面积筛选
		Mat draw_area;
		vector<vector<Point>> ho_bigDust_Select;
		ho_bigDust_Select = selectShapeArea2(ho_bigDust_open, draw_area, ho_bigDust_Connect, hv_bigDustSelectMin, hv_bigDustSelectMax);
		t7 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
		cout << "杂光连通域分析与面积筛选耗时：" << t7 - t6 << endl;

		//**********************提取中心光源***********************************
		//Mat ho_eliminate_CenterMean;
		//Mat ho_Image_AllV = imgV;
		//blur(ho_Image_AllV, ho_eliminate_CenterMean, Size(hv_eliminateDZT, hv_eliminateDZT));
		t6 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
		Mat ho_eliminate_CenterRegion;
		threshold(ho_bigDust_FlagMean, ho_eliminate_CenterRegion, hv_eliminateDZCenterTmin, hv_eliminateDZCenterTmax, 0);
		vector<vector<Point>> ho_eliminate_CenterConnect;
		Mat draw_big_dust1;
		ho_eliminate_CenterConnect = connection2(ho_eliminate_CenterRegion, draw_big_dust1);
		Mat draw_area1;
		vector<vector<Point>> ho_eliminate_aimCenterSelect;
		ho_eliminate_aimCenterSelect = selectShapeArea2(ho_bigDust_open, draw_area1, ho_eliminate_CenterConnect, hv_eliminateDZSlectTmin, hv_eliminateDZSlectTmax);
		t7 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
		cout << "杂灯珠区域提取耗时：" << t7 - t6 << endl;
		int hv_eliminate_CenterNumber = ho_eliminate_aimCenterSelect.size();
		if (hv_eliminate_CenterNumber == 1)
		{
			//求灯珠中心
			//Point centers;
			Point2f center; float radius;
			minEnclosingCircle(ho_eliminate_aimCenterSelect[0], center, radius);
			//Rect rect = boundingRect(ho_eliminate_aimCenterSelect[0]);
			//灯珠掩膜
			Mat center_mask=Mat::zeros(ho_eliminate_CenterRegion.size(),CV_8UC1);
			circle(center_mask, center, radius + hv_bigDustDilate, 255, -1);

			float hv_eliminate_CenterRow = center.y;
			float hv_eliminate_CenterColumn = center.x;
			//Mat eliminate_aimCenterSelect;
			//eliminate_aimCenterSelect = Mat::zeros(imgV.size(), CV_8UC1);
			//drawContours(eliminate_aimCenterSelect, ho_eliminate_aimCenterSelect, 0, 255, -1, 8);

			//Mat se_aimcenter = getStructuringElement(MORPH_ELLIPSE, Size(hv_eliminate_aimCenterOpen, hv_eliminate_aimCenterOpen));
			//morphologyEx(eliminate_aimCenterSelect, eliminate_aimCenterSelect, MORPH_OPEN, se_aimcenter);
			//t6 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
			//Mat se_aimcenter1 = getStructuringElement(MORPH_ELLIPSE, Size(2 * hv_bigDustDilate, 2 * hv_bigDustDilate));
			//Mat ho_bigDust_Dilation;
			//dilate(eliminate_aimCenterSelect, ho_bigDust_Dilation, se_aimcenter1);
			//t7 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
			//cout << "灯珠膨胀耗时：" << t7 - t6 << endl;
			
			Mat bigDust_Select;
			bigDust_Select = Mat::zeros(imgV.size(), CV_8UC1);
			for (int i = 0; i < ho_bigDust_Select.size(); i++)
			{
				drawContours(bigDust_Select, ho_bigDust_Select, i, 255, -1, 8);
			}
			//addWeighted(bigDust_Select, 1, ho_bigDust_Dilation, -1, 0, ho_bigDust_Dif);
			//去除灯珠的图片
			Mat ho_bigDust_Dif;
			addWeighted(bigDust_Select, 1, center_mask, -1, 0, ho_bigDust_Dif);
			//bitwise_or(eliminate_aimCenterSelect, bigDust_Select, ho_bigDust_Dif);
			//Mat se_aimcenter3 = getStructuringElement(MORPH_ELLIPSE, Size(hv_bigDustOpen, hv_bigDustOpen));
			//Mat ho_bigDust_CicleOpen;
			//morphologyEx(ho_bigDust_Dif, ho_bigDust_CicleOpen, MORPH_CLOSE, se_aimcenter3);

			t5 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
			Mat ho_bigDust_Intersect = ho_bigDust_Dif;
			//道子区域掩膜
			Mat ho_eliminate_RegionUnion;
			//***********特殊区域检测*********************
			//hv_jugement=specialROIfOffLine()
			//***************************************
			if (hv_jugement == 0)
			{
				//************去除屏蔽区域*************
				vector<vector<Point>> ho_eliminateDZ_Connect;
				ho_eliminateDZ_Connect = connection3(ho_bigDust_Intersect);
				//筛选出道子区域
				RotatedRect rrt;
				float hv_eliminateDZ_Row;
				float hv_eliminateDZ_Column;
				float hv_eliminateDZ_Phi;
				double hv_eliminateDZ_Length1;
				double hv_eliminateDZ_Length2;
				float k = 0;
				float line_arctan;
				double rate = 0;
				//利用掩膜获取道子区域
				double length1 = 0;
				double length2 = 0;
				ho_eliminate_RegionUnion = Mat::zeros(ho_bigDust_Intersect.size(), CV_8UC1);
				//#pragma omp parallel for num_threads(10)
				for (int hv_i = 0; hv_i < ho_eliminateDZ_Connect.size(); hv_i++)
				{
					rrt = minAreaRect(ho_eliminateDZ_Connect[hv_i]);
					hv_eliminateDZ_Row = rrt.center.y;
					hv_eliminateDZ_Column = rrt.center.x;
					hv_eliminateDZ_Phi = rrt.angle;

					//cout << hv_eliminateDZ_Phi << endl;
					length1 = rrt.size.height;
					length2 = rrt.size.width;
					hv_eliminateDZ_Length1 = min(length1, length2);
					hv_eliminateDZ_Length2 = max(length1, length2);
					//纵横比
					rate = length2 / length1;
					float eliminateDZ_Phi;
					eliminateDZ_Phi = hv_eliminateDZ_Phi;
					//根据长度和宽度进行筛选，opencv中长非长，宽非宽，取俩者的最值即可
					if ((hv_eliminateDZ_Length1 < hv_DZWidth) && (hv_eliminateDZ_Length2 > hv_eliminate_DZLenth))
					{
						k = (hv_eliminate_CenterRow - hv_eliminateDZ_Row) / (hv_eliminate_CenterColumn - hv_eliminateDZ_Column);
						//最小外接矩形的质心与灯珠中心连线的夹角
						line_arctan = atan(k);
						//角度计算
						double angle1 = line_arctan*57.3;
						double angle_diff = 30;
						double abs_angle = abs(angle1);
						if (abs_angle < 10 && (hv_eliminateDZ_Phi == 0 && rate > 1.618) || (hv_eliminateDZ_Phi == -90) && rate < 0.618)
						{
							angle_diff = abs(angle1);
						}
						else if (abs_angle > 84 && ((hv_eliminateDZ_Phi == 0 && rate < 0.618) || (hv_eliminateDZ_Phi == -90 && rate > 1.618)))
						{
							angle_diff = 90 - abs_angle;
						}
						else if (angle1<0 && rate>1.618)
						{
							angle_diff = abs(angle1 - hv_eliminateDZ_Phi);
						}
						else if (angle1 > 0 && rate < 0.618)
						{
							angle_diff = abs(90 - angle1 + hv_eliminateDZ_Phi);
						}
						else if (abs_angle > 85 && rate < 0.66&&abs(hv_eliminateDZ_Phi) < 6) {
							angle_diff = abs(90 - abs_angle + hv_eliminateDZ_Phi);
						}
						else if (abs_angle > 85 && rate > 1.5&&hv_eliminateDZ_Phi < -85) {
							angle_diff = abs(angle1 + hv_eliminateDZ_Phi);
						}
						else if (hv_eliminateDZ_Phi > -6 && rate > 1.5&&angle1 > 0 && angle1 < 6) {
							angle_diff = abs(angle1 + hv_eliminateDZ_Phi);
						}
						else if ((angle1 < 0) && (angle1 > -6) && (hv_eliminateDZ_Phi < -84) && (rate < 0.618)) {
							angle_diff = abs(90 - angle1 + hv_eliminateDZ_Phi);
						}
						if (angle_diff < DZ_angle)
						{
							drawContours(ho_eliminate_RegionUnion, ho_eliminateDZ_Connect, hv_i, 255, -1, 8);
						}
					}
				}
				//相当于reducce_domain;
				/*Mat ho_eliminate_Reduce;
				imgV.copyTo(ho_eliminate_Reduce, ho_eliminate_RegionUnion);
				double hv_eliminate_Min , hv_eliminate_Max;
				Point minLoc, maxLoc;
				minMaxLoc(ho_eliminate_Reduce, &hv_eliminate_Min, &hv_eliminate_Max, &minLoc, &maxLoc, ho_eliminate_RegionUnion);*/
				//t1 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
				////*******************道子判断***************************
				//hv_jugement = DZJudgeF(hv_jugement, ho_eliminate_Reduce, hv_eliminate_Min, param);		
				//cout << "提取道子特征的耗时：" << t1 - t5 << endl;
				//cout << "道子判断耗时：" << t2 - t1 << endl;
			}
			cout << "杂光特征预处理耗时：" << t5 << endl;
			//********************内尘检测*******************
			//内尘检测
			t2 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
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
				minMaxLoc(ho_bigDust_Reduce, &hv_bigDust_Min, &hv_bigDust_Max, &minLoc, &maxLoc, ho_bigDust_Intersect);
				t3 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
				//内尘杂光判定
				//hv_jugement = DustJudgeF(hv_jugement, ho_bigDust_Reduce, hv_bigDust_Min, param);

				t4 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
				cout << "内尘特征提取耗时：" << t3 - t2 << endl;
				cout << "内尘杂光检测耗时：" << t4 - t3 << endl;
			}
			if (hv_jugement == 0) 
			{
				//***************道子区域提取*****************************
				t4 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
				Mat ho_eliminate_Reduce;
				imgV.copyTo(ho_eliminate_Reduce, ho_eliminate_RegionUnion);
				double hv_eliminate_Min, hv_eliminate_Max;
				Point minLoc, maxLoc;
				minMaxLoc(ho_eliminate_Reduce, &hv_eliminate_Min, &hv_eliminate_Max, &minLoc, &maxLoc, ho_eliminate_RegionUnion);
				t1 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
				//*******************道子判断***************************
				hv_jugement = DZJudgeF(hv_jugement, ho_eliminate_Reduce, hv_eliminate_Min, param);
				t2 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
				cout << "道子区域模板获取耗时：" << t1 - t4 << endl;
				cout << "道子杂光判定耗时：" << t2 - t1 << endl;
			}			
		}
		else { hv_jugement = 10; }
		return hv_jugement;
	}	
}

int fatf::my_regionfOffLine_test(int jugement, Mat & img, Mat & imgV, fv_SParam & param)
{
	if (jugement == 0)
	{
		int hv_jugement = jugement;
		int hv_bigDustFlagDTP = param.hv_bigDustFlagDTP;
		int hv_bigDustMeanTmax = param.hv_bigDustMeanTmax;
		int hv_bigDustMeanTmin = param.hv_bigDustMeanTmin;
		int hv_bigDustFlagClose = param.hv_bigDustFlagClose;
		int hv_bigDustFlagOpen = param.hv_bigDustFlagOpen;
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
		int gausizesigma1 = param.gausizesigma1;
		int DZ_angle = param.DZ_angle;
		float adaptive_C = param.adaptive_C;
		//剔除道子区域
		double t, t1, t2, t3, t4, t5, t6, t7;
		t = (double)getTickCount();
		Mat ho_Image_Median = img;
		//Mat ho_bigDust_FlagScaled;
		//equalizeHist(ho_Image_median, ho_bigDust_FlagScaled);
		Mat ho_bigDust_FlagMean;
		//blur(ho_Image_median, ho_bigDust_FlagMean, Size(hv_bigDustMeanTmin, hv_bigDustMeanTmax));
		//medianBlur(ho_Image_Median, ho_bigDust_FlagMean, hv_bigDustMeanTmin);
		t6 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
		GaussianBlur(ho_Image_Median, ho_bigDust_FlagMean, Size(gausizesize, gausizesize), gausizesigma1, gausizesigma1, 4);
		
		t6 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
		Mat ho_eliminate_CenterRegion;
		threshold(ho_bigDust_FlagMean, ho_eliminate_CenterRegion, 225, hv_eliminateDZCenterTmax, 0);
		vector<vector<Point>> ho_eliminate_CenterConnect;
		Mat draw_big_dust1;
		ho_eliminate_CenterConnect = connection2(ho_eliminate_CenterRegion, draw_big_dust1);
		vector<vector<Point>>contours;
		//findContours(ho_eliminate_CenterConnect, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
		//选取最大轮廓
		int temp = 0;
		size_t sizes = 0;
		size_t index = 0;
		for (size_t t = 0; t < ho_eliminate_CenterConnect.size(); t++)
		{
			sizes = ho_eliminate_CenterConnect[t].size();
			if (sizes > temp)
			{
				temp = sizes;
				index = t;
			}
		}
		//求灯珠中心
		Point2f center; float radius;
		minEnclosingCircle(ho_eliminate_CenterConnect[index], center, radius);
		
		Mat center_mask=Mat::zeros(ho_bigDust_FlagMean.size(),CV_8UC1);
		drawContours(center_mask, ho_eliminate_CenterConnect, index, 255, -1);
		Mat  delete_center;
		//bitwise_xor(ho_bigDust_FlagMean, center_mask, delete_center);
		addWeighted(ho_bigDust_FlagMean, 1, center_mask, -1, 0, delete_center);

		/*Mat draw_area1;
		vector<vector<Point>> ho_eliminate_aimCenterSelect;
		ho_eliminate_aimCenterSelect = selectShapeArea2(ho_bigDust_open, draw_area1, ho_eliminate_CenterConnect, hv_eliminateDZSlectTmin, hv_eliminateDZSlectTmax);
		t7 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
		cout << "杂灯珠区域提取耗时：" << t7 - t6 << endl;*/
		//vector<vector<Point>>contours;
		//findContours(ho_fat_Region,contours,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE,Point());
		//选取最大轮廓
		t7 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
		cout << "高斯滤波耗时：" << t7 - t6 << endl;
		Mat ho_bigDust_FlagDyn;
		t6 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
		adaptiveThreshold(delete_center, ho_bigDust_FlagDyn, 255, 0, THRESH_BINARY, hv_bigDustFlagDTP, adaptive_C);
		t7 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
		cout << "动态阈值分割" << t7 - t6 << endl;
		//circle(ho_bigDust_FlagDyn, Point(1506, 510), 60.5, 0, -1, 8);
		//Mat ho_bigDust_Close;
		Mat se1 = getStructuringElement(MORPH_ELLIPSE, Size(hv_bigDustFlagOpen, hv_bigDustFlagOpen));
		//Mat se2 = getStructuringElement(MORPH_ELLIPSE, Size(hv_bigDustFlagClose, hv_bigDustFlagClose));
		Mat ho_bigDust_open;
		t6 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
		morphologyEx(ho_bigDust_FlagDyn, ho_bigDust_open, MORPH_CLOSE, se1);
		t7 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
		cout << "形态学闭操作耗时：" << t7 - t6 << endl;
		//morphologyEx(ho_bigDust_open, ho_bigDust_Close, MORPH_OPEN, se2);
		t6 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
		//连通域分析
		vector<vector<Point>> ho_bigDust_Connect;
		Mat draw_big_dust;
		ho_bigDust_Connect = connection2(ho_bigDust_open, draw_big_dust);
		//按照面积筛选
		Mat draw_area;
		vector<vector<Point>> ho_bigDust_Select;
		ho_bigDust_Select = selectShapeArea2(ho_bigDust_open, draw_area, ho_bigDust_Connect, hv_bigDustSelectMin, hv_bigDustSelectMax);
		t7 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
		cout << "杂光连通域分析与面积筛选耗时：" << t7 - t6 << endl;
		//提取中心光源
		//Mat ho_eliminate_CenterMean;
		//Mat ho_Image_AllV = imgV;
		//blur(ho_Image_AllV, ho_eliminate_CenterMean, Size(hv_eliminateDZT, hv_eliminateDZT));
		
		//int hv_eliminate_CenterNumber = ho_bigDust_FlagMean.size();
		//if (hv_eliminate_CenterNumber == 1)			
			//minEnclosingCircle(ho_eliminate_aimCenterSelect[0], center, radius);
			//Rect rect = boundingRect(ho_eliminate_aimCenterSelect[0]);

			float hv_eliminate_CenterRow = center.y;
			float hv_eliminate_CenterColumn = center.x;
			//Mat eliminate_aimCenterSelect;
			//eliminate_aimCenterSelect = Mat::zeros(imgV.size(), CV_8UC1);
			//drawContours(eliminate_aimCenterSelect, ho_eliminate_aimCenterSelect, 0, 255, -1, 8);

			//Mat se_aimcenter = getStructuringElement(MORPH_ELLIPSE, Size(hv_eliminate_aimCenterOpen, hv_eliminate_aimCenterOpen));
			//morphologyEx(eliminate_aimCenterSelect, eliminate_aimCenterSelect, MORPH_OPEN, se_aimcenter);
			//Mat se_aimcenter1 = getStructuringElement(MORPH_ELLIPSE, Size(hv_bigDustDilate, hv_bigDustDilate));
			//Mat ho_bigDust_Dilation;
			//dilate(eliminate_aimCenterSelect, ho_bigDust_Dilation, se_aimcenter1, Point(-1, -1), 1, 0);
			
			/*Mat ho_bigDust_Dif;
			Mat bigDust_Select;
			bigDust_Select = Mat::zeros(imgV.size(), CV_8UC1);
			for (int i = 0; i < ho_bigDust_Select.size(); i++)
			{
				drawContours(bigDust_Select, ho_bigDust_Select, i, 255, -1, 8);
			}

			addWeighted(bigDust_Select, 1, eliminate_aimCenterSelect, -1, 0, ho_bigDust_Dif);*/
			//bitwise_or(eliminate_aimCenterSelect, bigDust_Select, ho_bigDust_Dif);
			//Mat se_aimcenter3 = getStructuringElement(MORPH_ELLIPSE, Size(hv_bigDustOpen, hv_bigDustOpen));
			//Mat ho_bigDust_CicleOpen;
			//morphologyEx(ho_bigDust_Dif, ho_bigDust_CicleOpen, MORPH_CLOSE, se_aimcenter3);

			t5 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
			//Mat ho_bigDust_Intersect = ho_bigDust_Dif;
			//道子区域掩膜
			Mat ho_eliminate_RegionUnion;
			//***********特殊区域检测*********************
			//hv_jugement=specialROIfOffLine()
			//***************************************
			if (hv_jugement == 0)
			{
				//************去除屏蔽区域*************
				//vector<vector<Point>> ho_eliminateDZ_Connect;
				//ho_eliminateDZ_Connect = connection3(ho_bigDust_Intersect);
				//筛选出道子区域
				RotatedRect rrt;
				float hv_eliminateDZ_Row;
				float hv_eliminateDZ_Column;
				float hv_eliminateDZ_Phi;
				double hv_eliminateDZ_Length1;
				double hv_eliminateDZ_Length2;
				float k = 0;
				float line_arctan;
				double rate = 0;
				//利用掩膜获取道子区域
				double length1 = 0;
				double length2 = 0;
				ho_eliminate_RegionUnion = Mat::zeros(ho_bigDust_open.size(), CV_8UC1);
				//#pragma omp parallel for num_threads(10)
				for (int hv_i = 0; hv_i < ho_bigDust_Select.size(); hv_i++)
				{
					rrt = minAreaRect(ho_bigDust_Select[hv_i]);
					hv_eliminateDZ_Row = rrt.center.y;
					hv_eliminateDZ_Column = rrt.center.x;
					hv_eliminateDZ_Phi = rrt.angle;

					//cout << hv_eliminateDZ_Phi << endl;
					length1 = rrt.size.height;
					length2 = rrt.size.width;
					hv_eliminateDZ_Length1 = min(length1, length2);
					hv_eliminateDZ_Length2 = max(length1, length2);
					//纵横比
					rate = length2 / length1;
					float eliminateDZ_Phi;
					eliminateDZ_Phi = hv_eliminateDZ_Phi;
					//根据长度和宽度进行筛选，opencv中长非长，宽非宽，取俩者的最值即可
					if ((hv_eliminateDZ_Length1<hv_DZWidth) && (hv_eliminateDZ_Length2>hv_eliminate_DZLenth))
					{
						k = (hv_eliminate_CenterRow - hv_eliminateDZ_Row) / (hv_eliminate_CenterColumn - hv_eliminateDZ_Column);
						//最小外接矩形的质心与灯珠中心连线的夹角
						line_arctan = atan(k);
						//角度计算
						double angle1 = line_arctan*57.3;
						double angle_diff = 30;
						double abs_angle = abs(angle1);
						if (abs_angle<10 && (hv_eliminateDZ_Phi == 0 && rate>1.618) || (hv_eliminateDZ_Phi == -90) && rate<0.618)
						{
							angle_diff = abs(angle1);
						}
						else if (abs_angle>84 && ((hv_eliminateDZ_Phi == 0 && rate<0.618) || (hv_eliminateDZ_Phi == -90 && rate>1.618)))
						{
							angle_diff = 90 - abs_angle;
						}
						else if (angle1<0 && rate>1.618)
						{
							angle_diff = abs(angle1 - hv_eliminateDZ_Phi);
						}
						else if (angle1>0 && rate<0.618)
						{
							angle_diff = abs(90 - angle1 + hv_eliminateDZ_Phi);
						}
						else if (abs_angle > 85 && rate < 0.66&&abs(hv_eliminateDZ_Phi) < 6) {
							angle_diff = abs(90 - abs_angle + hv_eliminateDZ_Phi);
						}
						else if (abs_angle > 85 && rate > 1.5&&hv_eliminateDZ_Phi < -85) {
							angle_diff = abs(angle1 + hv_eliminateDZ_Phi);
						}
						else if (hv_eliminateDZ_Phi > -6 && rate > 1.5&&angle1 > 0 && angle1 < 6) {
							angle_diff = abs(angle1 + hv_eliminateDZ_Phi);
						}
						else if ((angle1<0) && (angle1>-6) && (hv_eliminateDZ_Phi < -84) && (rate < 0.618)) {
							angle_diff = abs(90 - angle1 + hv_eliminateDZ_Phi);
						}
						if (angle_diff < DZ_angle)
						{
							drawContours(ho_eliminate_RegionUnion, ho_bigDust_Select, hv_i, 255, -1, 8);
						}
					}
				}
				//相当于reducce_domain;
				Mat ho_eliminate_Reduce;
				imgV.copyTo(ho_eliminate_Reduce, ho_eliminate_RegionUnion);
				double hv_eliminate_Min = 0, hv_eliminate_Max;
				Point minLoc, maxLoc;
				//minMaxLoc(ho_eliminate_Reduce, &hv_eliminate_Min, &hv_eliminate_Max, &minLoc, &maxLoc);
				t1 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
				hv_jugement = DZJudgeF(hv_jugement, ho_eliminate_Reduce, hv_eliminate_Min, param);
				t2 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
				cout << "杂光特征预处理耗时：" << t5 << endl;
				cout << "提取道子特征的耗时：" << t1 - t5 << endl;
				cout << "道子判断耗时：" << t2 - t1 << endl;
			}
			//********************内尘检测*******************
			//内尘检测
			int hv_DZeliminateOpen = param.hv_DZeliminateOpen;
			if (hv_jugement == 0)
			{
				//ho_eliminate_RegionUnion = ho_eliminate_RegionUnion;
				Mat ho_bigDust_eliminateDif;
				subtract(ho_bigDust_open, ho_eliminate_RegionUnion, ho_bigDust_eliminateDif);
				//addWeighted(ho_bigDust_Intersect, 1, ho_eliminate_RegionUnion, -1, 0, ho_bigDust_eliminateDif);
				Mat ho_bigDust_Intersect;
				ho_bigDust_Intersect = opening_circle2(ho_bigDust_eliminateDif, hv_DZeliminateOpen);
				//用掩膜获取
				Mat ho_bigDust_Reduce;
				ho_Image_Median.copyTo(ho_bigDust_Reduce, ho_bigDust_Intersect);
				double hv_bigDust_Min, hv_bigDust_Max;
				Point minLoc, maxLoc;
				minMaxLoc(ho_bigDust_Reduce, &hv_bigDust_Min, &hv_bigDust_Max, &minLoc, &maxLoc);
				t3 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
				//内尘杂光判定
				hv_jugement = DustJudgeF(hv_jugement, ho_bigDust_Reduce, hv_bigDust_Min, param);
				t4 = 1000 * ((double)getTickCount() - t) / getTickFrequency();
				cout << "内尘特征提取耗时：" << t3 - t2 << endl;
				cout << "内尘杂光检测耗时：" << t4 - t3 << endl;
			}
		return hv_jugement;
	}
	else { return jugement; }
}

int fatf::DZJudgeF(int jugement, Mat & img, double eliminate_Min, fv_SParam & param)
{
	int hv_jugement = jugement;
	double hv_eliminate_Min = eliminate_Min;
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
		double hv_eliminateFTmin = hv_eliminate_Min + hv_eliminateTmin;
		if (hv_eliminateTmin>(hv_eliminateTmax - hv_eliminate_Min))
			hv_eliminateFTmin = hv_eliminateTmax - hv_eliminateTmin;
		Mat ho_eliminate_FRegion;
		Mat ho_eliminate_Reduce = img;
		threshold(ho_eliminate_Reduce, ho_eliminate_FRegion, hv_eliminateFTmin, hv_eliminateTmax, 0);
		//Mat ho_eliminate_RegionClose;
		//ho_eliminate_RegionClose = closing_circle2(ho_eliminate_FRegion, hv_eliminateClose);
		vector<vector<Point>> ho_eliminate_FConnect;
		ho_eliminate_FConnect = connection3(ho_eliminate_FRegion);
		vector<vector<Point>> ho_eliminate_SelectedRe;
		ho_eliminate_SelectedRe = selectShapeArea3(ho_eliminate_FConnect, hv_eliminateSmin, hv_eliminateSmax);
		RotatedRect rrt;
		float weight = 0;
		float height = 0;
		float hv_eliminate_FLength1 = 0;
		float hv_eliminate_FLength2 = 0;
		int hv_eliminate_CountShort = 0;
		int hv_eliminate_CountLong = 0;
		//#pragma omp parallel for num_threads(4)
		for (int i = 0; i < ho_eliminate_SelectedRe.size(); i++)
		{
			rrt = minAreaRect(ho_eliminate_SelectedRe[i]);
			weight = rrt.size.width;
			height = rrt.size.height;
			hv_eliminate_FLength1 = max(weight, height);
			
			hv_eliminate_FLength2 = min(weight, height);
			
			//道子数
			if (hv_eliminate_FLength1 > hv_eliminate_judgeCountShort)
			{
				//记录数据
				write_date(hv_eliminate_FLength1);
				write_date(hv_eliminate_FLength2);
				hv_eliminate_CountShort += 1;
			}
				
			//长道子数
			if (hv_eliminate_FLength1>hv_eliminate_judgeCountLong)
				hv_eliminate_CountLong += 1;
			//长道子或亮道子（宽）
			hv_eliminate_FLength1 = hv_eliminate_L1W * (hv_eliminate_FLength1 / (2 * hv_eliminate_jugeL1));
			hv_eliminate_FLength2 = hv_eliminate_L2W*(hv_eliminate_FLength2 / (2 * hv_eliminate_jugeL2));
			if ((hv_eliminate_FLength1 >= 0.5) || (hv_eliminate_FLength2 >= 0.5))
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
		next_line();
		//if (hv_jugement != 0) next_line();
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
	//Mat ho_bigDust_Reduce = img;
	if (hv_jugement == 0) 
	{
		int hv_bigDustFTmin=0;
		hv_bigDustFTmin = hv_bigDust_Min + hv_bigDustTmin;
		if(hv_bigDustTmin>(hv_bigDustTmax - hv_bigDust_Min))
			hv_bigDustFTmin = hv_bigDustTmax - hv_bigDustTmin;
		Mat ho_bigDust_FRegion;
		threshold(img, ho_bigDust_FRegion, hv_bigDustFTmin, hv_bigDustTmax, 0);
		vector<vector<Point>> dust_contours;
		dust_contours = connection3(ho_bigDust_FRegion);
		double hv_bigDust_Area = 0;
		double dust_area = -1;
		for (int i = 0; i < dust_contours.size(); i++) 
		{
			dust_area = contourArea(dust_contours[i]);
			//if(dust_area>600)
			hv_bigDust_Area+=dust_area;
		}
		double hv_BIGDustU = 0;
		hv_BIGDustU = hv_bigDust_Area;
		write_date(hv_BIGDustU);
		
		double hv_bigDust_Area_score = hv_bigDustW*(hv_bigDust_Area / (2 * hv_jugeBIGDustU));
		if (hv_bigDust_Area_score >= 0.5)
			hv_jugement = 2;
	}
	if(hv_jugement!=0) next_line();
	return hv_jugement;
}
