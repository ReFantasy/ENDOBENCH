#include "stdafx.h"
#include "ComCorrect.h"
#include <string>
#include "ConfigFile.h"

//校正变量
double CH_1_Point_Y[128];	 // 定义通道一的拐点数组
double CH_1_Point_X[128];	 // 定义通道一的校正系数数组
double CH_1_Base;    // 定义基准校正值
double CH_1_Point_N;

double CH_2_Point_Y[128];	 // 定义通道一的拐点数组
double CH_2_Point_X[128];	 // 定义通道一的校正系数数组
double CH_2_Base;    // 定义基准校正值
double CH_2_Point_N;

void LoadIni()
{
	ConfigFile config_file;
	config_file.ReadConfigFile("ComConfig.ini");

	CH_1_Point_N = config_file.GetValueToDouble("CH1_Adjust_N");
	//基准值

	CH_1_Base = config_file.GetValueToDouble("CH1_Adjust_Base");


	for (int ii = 0; ii < CH_1_Point_N; ii++)
	{
		string  CH1_Adjust_X("CH1_Adjust_X");
		string  CH1_Adjust_Y("CH1_Adjust_Y");
		CH_1_Point_X[ii] = config_file.GetValueToDouble(CH1_Adjust_X + to_string(ii + 1));
		CH_1_Point_Y[ii] = config_file.GetValueToDouble(CH1_Adjust_Y + to_string(ii + 1));
	}


	//读取通道二拐点数量

	CH_2_Point_N = config_file.GetValueToDouble("CH2_Adjust_N");
	//基准值
	CH_2_Base = config_file.GetValueToDouble("CH2_Adjust_Base");

	for (int ii = 0; ii < CH_2_Point_N; ii++)
	{
		string  CH2_Adjust_X("CH2_Adjust_X");
		string  CH2_Adjust_Y("CH2_Adjust_Y");
		CH_2_Point_X[ii] = config_file.GetValueToDouble(CH2_Adjust_X + to_string(ii + 1));
		CH_2_Point_Y[ii] = config_file.GetValueToDouble(CH2_Adjust_Y + to_string(ii + 1));
	}
}

double Convert_To_Real_CH1(string adc)
{
	double  i = StringToDigit<double>(adc) + CH_1_Base;//首先加上基准值，然后，再判断在哪个区间内
	for (int j = 0; j < (int)CH_1_Point_N - 1; j++)
	{
		if ((i >= CH_1_Point_X[j]) && (i < CH_1_Point_X[j + 1]))//假如ADC值在这个区间范围内
		{
			i = i * CH_1_Point_Y[j];
			break;
		}
	}
	return i;
}

double Convert_To_Real_CH2(string adc)
{
	double i = StringToDigit<double>(adc) + CH_2_Base;//首先加上基准值，然后，再判断在哪个区间内
	for (int j = 0; j < (int)CH_2_Point_N - 1; j++)
	{
		if ((i >= CH_2_Point_X[j]) && (i < CH_2_Point_X[j + 1]))//假如ADC值在这个区间范围内
		{
			i = i * CH_2_Point_Y[j];
			break;
		}
	}
	return i;
}

string GetSubA(const string str)
{
	bool is_exit = false;
	int a_index = str.find_first_of('A', 0);
	int shap_index = str.find_first_of('#', a_index);
	return string(str.begin() + a_index + 1, str.begin() + shap_index);
}

string GetSubB(const string str)
{
	bool is_exit = false;
	int a_index = str.find_first_of('B', 0);
	int shap_index = str.find_first_of('#', a_index);
	return string(str.begin() + a_index + 1, str.begin() + shap_index);
}
