#pragma once
struct fv_SParam
{
public:

	//异常检测参数
	float hv_AbnormalZoomSize = 0.5;
	int hv_AbnormalMedianT = 11;
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
	int hv_fatTmin = 235;
	int hv_fatTmax = 255;
	int hv_fatOpenT = 25;
	int hv_diameter = 110;

	//道子内尘提取参数
	//高斯
	int gausizesize = 55;//35
	int gausizesigma1 = 25;
	int gausizesigma2 = 21;
	int hv_bigDustFlagDTP = 65;// 85;
	int hv_bigDustFlagOpen = 27;
	int hv_bigDustFlagClose = 19;//19;

	int hv_bigDustMeanTmin = 125;//125
	int hv_bigDustMeanTmax = 125;//125

	int hv_bigDustSelectMax = 99999999;
	int hv_bigDustSelectMin = 900;// 290;
	int hv_eliminateDZT = 9;
	int hv_eliminateDZCenterTmin = 254;
	int hv_eliminateDZCenterTmax = 255;
	int hv_eliminateDZSlectTmin = 350;
	int hv_eliminateDZSlectTmax = 99999999;
	int hv_eliminate_aimCenterOpen = 35;
	int hv_bigDustDilate = 33;
	int hv_bigDustOpen = 5;
	int hv_eliminate_DZLenth = 100;
	int hv_DZWidth = 70;


	int judgeFlag = 0;

	//道子判断
	int hv_eliminateTmin=35;
	int hv_eliminateTmax = 255;
	int hv_eliminateClose = 15;
	int hv_eliminateSmin = 150;
	int hv_eliminateSmax = 99999999;
	int hv_eliminate_judgeCountShort = 75;//75
	int hv_eliminate_judgeCountLong = 150;//135
	int hv_eliminate_judgeLong = 0;
	int hv_eliminate_judgeShort = 2;
	float hv_eliminate_L1W = 1;
	float hv_eliminate_L2W = 1;
	int hv_eliminate_jugeL1=77;
	int hv_eliminate_jugeL2 = 8;

	//内尘检测
	int hv_DZeliminateOpen = 3;
	int hv_bigDustTmin = 0;
	int hv_bigDustTmax = 255;
	//内尘
	int hv_jugeBIGDustU = 3000;//8930;//530
	int hv_bigDustW = 1;

};