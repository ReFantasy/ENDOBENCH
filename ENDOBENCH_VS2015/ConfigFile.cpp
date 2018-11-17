#include "stdafx.h"
#include "ConfigFile.h"

bool ConfigFile::ReadConfigFile(const string file_name)
{
	if (!_map.empty())
		return false;
	_fstream.open(file_name);
	if (!_fstream)
		return false;  // ��ȡ�����ļ�ʧ��
	string line;
	while (getline(_fstream, line))
	{
		//cout << line << endl;
		string key, value;
		bool splite = Split(line, key, value);  
		if (splite)
		{
			key = EraseSpace(key);
			value = EraseSpace(value);
			//���뵽map
			_map.insert(make_pair(key,value));
		}
	}
	
	_fstream.close();
}

bool ConfigFile::ReReadConfigFile(const string file_name)
{
	_map.clear();
	return ReReadConfigFile(file_name);
}

ConfigFile::ValueType ConfigFile::GetValue(const KeyType Key)const
{
	auto iter = _map.find(Key);
	if (iter == _map.end())
		return KeyType();
	else
		return iter->second;
}

double ConfigFile::GetValueToDouble(const KeyType Key) const
{
	return StringToDigit<double>(GetValue(Key));
}

int ConfigFile::GetValueToInt(const KeyType Key) const
{
	return StringToDigit<int>(GetValue(Key));
}

bool ConfigFile::Split(string line, string &key, string &value)
{
	string::iterator iter_equal_sign = line.end();
	if ((iter_equal_sign = find(line.begin(),line.end(), '=')) == line.end())
		return false;  // �Ҳ���'='������Ч��

	// �ָ� key�� value
	key = string(line.begin(), iter_equal_sign);
	value = string(iter_equal_sign + 1, line.end());

	// ȥ����λ�ո�
	
}

std::string ConfigFile::EraseSpace(string &s)
{
	if (s.empty())
		return std::string("0");

	string::iterator b, e;
	for (int i = 0; i < s.size(); i++)
	{
		if (s[i] != ' ')
		{
			b = s.begin() + i;
			break;
		}
	}
	for (int i = s.size()-1; i >=0; i--)
	{
		if (s[i] != ' ')
		{
			e = s.begin() + i;
			break;
		}
	}
	return string(b, e + 1);
}
