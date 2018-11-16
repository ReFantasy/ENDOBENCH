#ifndef __COMCORRECT_H__
#define __COMCORRECT_H__
#include <windows.h>
#include <string>
using std::string;

//У������
extern double CH_1_Point_Y[128];	 // ����ͨ��һ�Ĺյ�����
extern double CH_1_Point_X[128];	 // ����ͨ��һ��У��ϵ������
extern double CH_1_Base;    // �����׼У��ֵ
extern double CH_1_Point_N;

extern double CH_2_Point_Y[128];	 // ����ͨ��һ�Ĺյ�����
extern double CH_2_Point_X[128];	 // ����ͨ��һ��У��ϵ������
extern double CH_2_Base;    // �����׼У��ֵ
extern double CH_2_Point_N;


// ���������ļ�
void LoadIni();

// ת��ͨ��һ����ʵֵ
double  Convert_To_Real_CH1(string adc);
// ת��ͨ��2����ʵֵ
double Convert_To_Real_CH2(string adc);

// ��ȡ�Ӵ�
string GetSubA(const string str);
string GetSubB(const string str);

#endif //__COMCORRECT_H__