#pragma once
struct fv_SParam
{
public:

	//异常检测参数
	float hv_AbnormalZoomSize = 0.5;

	int hv_AbnormalMedianT = 11;//长度9，原半径5，
	int hv_AbnormalMinT = 200;
	int hv_AbnormalMaxT = 255;
	int hv_AbnormalSelectLmin = 10000;
	int hv_AbnormalSelectLmax = 99999999;
	int hv_AbnormalNumberJuge1 = 1;
	int hv_AbnormalNumberJuge2 = 0;
	int hv_clip = 15;
	int hv_AbnormalMAXArea = 10000;
	int hv_AbnormalMINArea = 15000;

	//光源发胖参数
	int hv_fatTmin = 235;// 235;
	int hv_fatTmax = 255;
	int hv_fatOpenT = 25;
	int hv_diameter = 136;//根据4-30设置(126)//1-11

	//道子内尘提取参数
	//高斯
	int gausizesize =13;// 55;//35
	int gausizesigma1 = 15;//25
	int gausizesigma2 = 10;//21
	//blocksize-mean
	int mean_size = 105;// 105//125;
	float bigDustFlagScale = 0.63;
	float hv_bigDustFlagScale = 0.63;
	int bigDustFlagDTP = 2;

	double adaptive_C = -1;

	int hv_bigDustFlagOpen = 22;// 22;
	
	int hv_bigDustFlagClose = 36;//现36 原半径19的圆，

	int hv_bigDustMeanTmin = 125;
	int hv_bigDustMeanTmax = 125;
	//内尘杂光面积选择
	int hv_bigDustSelectMax = 99999999;
	int hv_bigDustSelectMin = 230;// 290;

	int hv_eliminateDZT = 9;
	int hv_eliminateDZCenterTmin = 254;
	int hv_eliminateDZCenterTmax = 255;

	int hv_eliminateDZSlectTmin = 50;//50
	int hv_eliminateDZSlectTmax = 99999999;
	int hv_eliminate_aimCenterOpen = 35;
	//画中心光源
	int hv_bigDustDilate = 35;
	int hv_bigDustOpen = 23;

	//道子特征选取最低参数
	int hv_DZLenth = 42;
	int hv_DZWidth = 80;

	int judgeFlag = 0;

	//道子判断
	int hv_eliminateTmin=0;
	//int eliminateTmin[]={35,3,19,65,85};
	int hv_eliminateTmax = 255;
	int hv_eliminateClose = 9;//圆半径为5，等效方形8.8
	//对道子区域进行面积筛选
	int hv_eliminateSmin = 150;
	int hv_eliminateSmax = 99999999;

	int DZ_angle = 20;
	//短道子
	int hv_eliminate_judgeCountShort = 52;//75
	//int hv_eliminate_judgeCountShort={69,179,99,100,100}
	//长道子入选
	int hv_eliminate_judgeCountLong = 89;//280
	//int hv_eliminate_judgeCountLong={89,219,159,200,200}

	//长道子数量控制
	int hv_eliminate_judgeLong = 2;
	//道子数量控制
	int hv_eliminate_judgeShort = 3;
	float hv_eliminate_L1W = 1;
	float hv_eliminate_L2W = 1;
	//长道子长
	int hv_eliminate_jugeL1=250;
	//亮道子宽
	int hv_eliminate_jugeL2 = 47;

	//内尘检测
	int hv_DZeliminateOpen = 3;
	int hv_bigDustTmin = 0;
	//int hv_bigDustTmin[5] = { 0,10,30,40,50 };
	int hv_bigDustTmax = 255;
	//内尘联合面积
	int hv_jugeBIGDustU=3500;// 3700;//8930;//530
	//内尘最大面积3500
	int hv_bigDustW = 1;

};