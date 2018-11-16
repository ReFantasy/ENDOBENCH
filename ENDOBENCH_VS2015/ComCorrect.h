#ifndef __COMCORRECT_H__
#define __COMCORRECT_H__
#include <windows.h>
#include <string>
using std::string;

//校正变量
extern double CH_1_Point_Y[128];	 // 定义通道一的拐点数组
extern double CH_1_Point_X[128];	 // 定义通道一的校正系数数组
extern double CH_1_Base;    // 定义基准校正值
extern double CH_1_Point_N;

extern double CH_2_Point_Y[128];	 // 定义通道一的拐点数组
extern double CH_2_Point_X[128];	 // 定义通道一的校正系数数组
extern double CH_2_Base;    // 定义基准校正值
extern double CH_2_Point_N;


// 加载配置文件
void LoadIni();

// 转换通道一的真实值
double  Convert_To_Real_CH1(string adc);
// 转换通道2的真实值
double Convert_To_Real_CH2(string adc);

// 获取子串
string GetSubA(const string str);
string GetSubB(const string str);

#endif //__COMCORRECT_H__