#pragma once
#include <QString>
#include <iostream>
#include<vector>
#include <QColor>
struct RollStruct
{
	float value;
	QString name;
};
struct YBRGB
{
	uchar r;
	uchar g;
	uchar b;
};
struct parseData
{
	QString name;
	float value;
	QString toWord;
	struct YBRGB color;
};
//
struct cellProperty
{
	uint16_t r;
	uint16_t g;
	uint16_t b;
	QString toWord;
	QString value;
	bool isStand;
	//QColor  color;
	
};
//�ֶ�����
struct protoItem {
	bool octhex;								//16���Ʒ�
	bool isRoll;								//�Ƿ��������ʾ������һ�����
	int send;									//���͵�����
	int startByte;								//��ֹ�ֽ�
	int startBit;								//��ֹλ
	int bitLeng;								//����
	float precision;							//����
	int offset;									//ƫ��
	QString bitName;							//�ֶ�����
	QString dataFrom;							//��Դ���籨��ֻ�е�����ѹ��Ҫ��ʾ���ʣ����õ�����ѹ���
	std::vector<cellProperty>stl_itemProperty;	//��ʾ������
	//std::map<QString, cellProperty>itemProperty;
};
//CAN-ID���ݽṹ��
struct canIdData {
	bool isSend;							//�Ƿ��������ID
	uint8_t opt;							//���ձ��Ļ��Ƿ��ͱ���
	qint64 CanId;							//CAN-ID������
	QString strCanId;						//CAN-ID
	std::vector<struct protoItem>pItem;		//����
};
//�ͺ����ݽṹ��
struct protoData {
	
	uint8_t agreement;				//Э��
	int bundRate;					//������
	int circle;						//��������
	QString modelName;				//����
	std::vector<canIdData>cItem;	//����
	bool bStandardId;				//�Ƿ�Ϊ��׼֡
};
struct autoTestData {
	int m_iLowVolt;				//Ƿѹ�𲽵�ѹ
	int m_iHeightVolt;			//��ѹ�𲽵�ѹ
	int m_iVoltStep;			//��ѹ����
	int m_iVoltError;			//��ѹ���
	int m_iHeatTempture;		//��ʼ���ȵ��¶�
	int m_iPowerTempture;		//����ʵ��¶�
	int m_iOverTime;			//��ʱ
	QString m_sInWebAddr;		//�ӿڵ�ַ
	QString m_sOutWebAddr;		//�ӿڵ�ַ
};