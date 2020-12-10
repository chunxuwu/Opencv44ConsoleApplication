// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"
#include<opencv2/opencv.hpp>
#include <fstream>
#include <stdio.h>
#include <tchar.h>
#include<iostream>
#include<fstream>
#include<io.h>
#include<string>

using namespace std;
using namespace cv;

#include"evaluate.h";
//#include"inside_dust_detect.h"
//#include "image_subtract.h"
//#include"subtract_center_area.h"
//#include"little_dust_detect.h"
//#include"fat_light_detect.h"
//#include "flare_detecte.h"
//#include"flare_detecte_V3.h"
#include"flare_detect_V4.h"
#include "fv_SParam.h"
#include "fatf.h"

extern int OK_nums;
extern int NG_nums;

// TODO:  在此处引用程序需要的其他头文件
