// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"
#include<opencv2/opencv.hpp>
#include <fstream>
#include <stdio.h>
#include <tchar.h>
#include<iostream>

using namespace std;
using namespace cv;

#include"evaluate.h";
#include"inside_dust_detect.h"
#include "image_subtract.h"
#include"subtract_center_area.h"
#include"little_dust_detect.h"
#include"fat_light_detect.h"


extern int OK_nums;
extern int NG_nums;

// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�