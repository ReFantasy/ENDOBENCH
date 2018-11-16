#include "stdafx.h"
#include "ComCorrect.h"
#include <string>
#include "ConfigFile.h"

//У������
double CH_1_Point_Y[128];	 // ����ͨ��һ�Ĺյ�����
double CH_1_Point_X[128];	 // ����ͨ��һ��У��ϵ������
double CH_1_Base;    // �����׼У��ֵ
double CH_1_Point_N;

double CH_2_Point_Y[128];	 // ����ͨ��һ�Ĺյ�����
double CH_2_Point_X[128];	 // ����ͨ��һ��У��ϵ������
double CH_2_Base;    // �����׼У��ֵ
double CH_2_Point_N;

void LoadIni()
{
	ConfigFile config_file;
	config_file.ReadConfigFile("C:\\Users\\MICCAI\\Desktop\\Config.ini");

	CH_1_Point_N = config_file.GetValueToDouble("CH1_Adjust_N");
	//��׼ֵ

	CH_1_Base = config_file.GetValueToDouble("CH1_Adjust_Base");


	for (int ii = 0; ii < CH_1_Point_N; ii++)
	{
		string  CH1_Adjust_X("CH1_Adjust_X");
		string  CH1_Adjust_Y("CH1_Adjust_Y");
		CH_1_Point_X[ii] = config_file.GetValueToDouble(CH1_Adjust_X + to_string(ii + 1));
		CH_1_Point_Y[ii] = config_file.GetValueToDouble(CH1_Adjust_Y + to_string(ii + 1));
	}


	//��ȡͨ�����յ�����

	CH_2_Point_N = config_file.GetValueToDouble("CH2_Adjust_N");
	//��׼ֵ
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
	double  i = StringToDigit<double>(adc) + CH_1_Base;//���ȼ��ϻ�׼ֵ��Ȼ�����ж����ĸ�������
	for (int j = 0; j < (int)CH_1_Point_N - 1; j++)
	{
		if ((i >= CH_1_Point_X[j]) && (i < CH_1_Point_X[j + 1]))//����ADCֵ��������䷶Χ��
		{
			i = i * CH_1_Point_Y[j];
			break;
		}
	}
	return i;
}

double Convert_To_Real_CH2(string adc)
{
	double i = StringToDigit<double>(adc) + CH_2_Base;//���ȼ��ϻ�׼ֵ��Ȼ�����ж����ĸ�������
	for (int j = 0; j < (int)CH_2_Point_N - 1; j++)
	{
		if ((i >= CH_2_Point_X[j]) && (i < CH_2_Point_X[j + 1]))//����ADCֵ��������䷶Χ��
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
