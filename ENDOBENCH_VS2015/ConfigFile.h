
/**
 * 
 *  ��ȡ�����ļ�
 *  ��¼�ĸ�ʽΪ key = value
 *  ��֧�ֶַ�[section]
 *  
 *  TDL 2018.11.15
 */
#ifndef __CONFIGFILE_H__
#define __CONFIGFILE_H__
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <algorithm>
#include <sstream>

using namespace std;

class ConfigFile
{
public:
	typedef string KeyType;
	typedef string ValueType;
public:
	ConfigFile() = default;
	bool ReadConfigFile(const string file_name);
	bool ReReadConfigFile(const string file_name); // ���¶�ȡ�����ļ�
	ValueType GetValue(const KeyType Key)const;
	double GetValueToDouble(const KeyType Key)const;
	int GetValueToInt(const KeyType Key)const;
private:
	fstream _fstream;
	map<KeyType, ValueType> _map;

	// �ָ������ļ���һ�� ��ʽ key=value
	bool Split(string line, string &key, string &value);
	// ȥ���ַ�����λ�ո�
	string EraseSpace(string &s);
};

// �ַ���ת����
template <class Type>
Type StringToDigit(const string& str)
{
	istringstream iss(str);
	Type num;
	iss >> num;
	return num;
}

#endif // __CONFIGFILE_H__
