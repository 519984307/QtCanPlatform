#pragma once
#include <iostream>
#include <stdlib.h>
#include <map>
#include "dataStruct.h"
namespace YB {
	template <typename T_Key,typename T_Value>
	bool keyInMap(std::map<T_Key,T_Value>&mp,T_Key key)
	{
		std::map<T_Key, T_Value>::iterator it = mp.find(key);
		return it != mp.end();
	}
	template<typename T>
	T InRang(T min, T max, T value)
	{
		if (value < min)
			return min;
		else if (value > max)
			return max;
		else
			return value;
	}
	
	int idNameInVector(std::vector<showTableData>& Data, const QString& name);
	
	/************************************
	* @brief������name�Ƿ��Ѿ�����vec����
	* @param1 vec�������ҵ�vector
	* @param2 name��Ҫ���ҵ�name
	* @param3 index������λ��
	************************************/
	bool nameInVector(std::vector<RollStruct>& vec, QString name, int& index);

	bool isExistKey(std::map<unsigned int, QStringList>& m, unsigned int key);
}