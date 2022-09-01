#pragma once
#include <QString>
#include <iostream>
#include<vector>
#include <QColor>
struct YBRGB
{
	uchar r;
	uchar g;
	uchar b;
};
struct parseData
{
	QString name;
	int value;
	QString toWord;
	struct YBRGB color;
};
struct cellProperty
{
	uint16_t r;
	uint16_t g;
	uint16_t b;
	QString toWord;
	QString value;
	//QColor  color;
	
};
struct protoItem {
	int send;
	int startByte;
	int startBit;
	int bitLeng;
	int precision;
	int offset;
	QString bitName;
	std::vector<cellProperty>stl_itemProperty;
	//std::map<QString, cellProperty>itemProperty;
};
struct canIdData {
	uint8_t opt;
	qint64 CanId;
	std::vector<struct protoItem>pItem;
};
struct protoData {
	uint8_t agreement;
	QString modelName;
	std::vector<canIdData>cItem;
};
