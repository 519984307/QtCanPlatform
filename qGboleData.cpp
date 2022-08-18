#include "qGboleData.h"
#include <qapplication.h>
#include <qmessagebox.h>
#include <QDir>
#include <QJsonDocument>	// ����Json
#include <QJsonArray>		// [ ]
#include <QJsonObject>		// { }
#include <QJsonValue>		// int float double null { } [ ]
#include <QJsonParseError>
#include <QTextStream>
#include <QString>
#include <qobject.h>
qGboleData* qGboleData::pGboleInstance = nullptr;
qGboleData::qGboleData()
{

}

void qGboleData::save()
{
	std::lock_guard<std::mutex>lock(dMut);
	QString appPath = QApplication::applicationDirPath() + "/Data";
	QDir dir(appPath);
	if (!dir.exists())
	{
		if (!dir.mkpath(appPath))
		{
			QMessageBox::warning(NULL, QObject::tr("warning"), QObject::tr("��������Ŀ¼ʧ��"));
			return;
		}
	}

	QJsonObject mItemObjRoot;
	for (int i = 0; i < pGboleData.size(); i++)
	{

		QJsonObject mItem;
		mItem.insert("modelName", pGboleData.at(i).modelName);
		mItem.insert("agreement", pGboleData.at(i).agreement);
		QJsonObject cItemObj;
		for (int j = 0; j < pGboleData.at(i).cItem.size(); j++)
		{
			QJsonObject pItemObj;
			pItemObj.insert("CanId", pGboleData.at(i).cItem.at(j).CanId);
			pItemObj.insert("opt", pGboleData.at(i).cItem.at(j).opt);
			QJsonObject pDItem;
			//pItem��һ������������
			for (int k = 0; k < pGboleData.at(i).cItem.at(j).pItem.size(); k++)
			{
				QJsonArray pItem;
				pItem.append(pGboleData.at(i).cItem.at(j).pItem.at(k).bitName);
				pItem.append(pGboleData.at(i).cItem.at(j).pItem.at(k).startByte);
				pItem.append(pGboleData.at(i).cItem.at(j).pItem.at(k).startBit);
				pItem.append(pGboleData.at(i).cItem.at(j).pItem.at(k).bitLeng);
				pItem.append(pGboleData.at(i).cItem.at(j).pItem.at(k).precision);
				pItem.append(pGboleData.at(i).cItem.at(j).pItem.at(k).offset);
				pDItem.insert(QString::number(k), pItem);
			}
			pItemObj.insert("pDItem", pDItem);
			cItemObj.insert(QString::number(j), pItemObj);
		}
		mItem.insert("mDItem", cItemObj);
		mItemObjRoot.insert(QString::number(i), mItem);
	}
	// ��json�����������ת��Ϊ�ַ���
	QJsonDocument doc;
	// ��object����λ���ĵ�����Ҫ����
	doc.setObject(mItemObjRoot);
	appPath += "/appData.json";
	QFile file(appPath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
	{
		return;
	}
	QTextStream stream(&file);
	stream.setCodec("UTF-8");
	stream << doc.toJson();
	file.close();
}

void qGboleData::read()
{
	std::lock_guard<std::mutex>lock(dMut);
	QString appPath = QApplication::applicationDirPath() + "/Data/appData.json";
	//QFile ���Ի�ȡ�ļ��Ǵ���
	QFile file(appPath);
	if (!file.exists(appPath))
		return;
	//���ļ���ֻ��ģʽ�����ʧ�ܣ���return
	if (!file.open(QFile::ReadOnly | QFile::Truncate))
	{
		QMessageBox::warning(NULL, QObject::tr("warning"), QObject::tr("���ݲ����ڣ���ʼ������ʧ��"));
		return;
	}
	//��text�������ڴ���
	QTextStream readStrem(&file);
	//���ø�ʽΪutf-8
	readStrem.setCodec("UTF-8");
	//��ȡ����
	QString str = readStrem.readAll();
	//����֮��ǵ�Ҫclose����ֹ�����쳣�˳���������
	file.close();
	QJsonParseError jsonError;
	//����Ϊjson�ĵ�
	QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8(), &jsonError);
	if (jsonError.error != QJsonParseError::NoError)
	{
		QMessageBox::warning(NULL, QObject::tr("warning"), QString(QObject::tr("����jsonʧ�ܣ�") + jsonError.errorString()));
		return;
	}
	//ת����json object
	QJsonObject rootObj = doc.object();
	//��ȡ����key
	QStringList keyList = rootObj.keys();
	//����key���б���
	for (int i = 0; i < keyList.size(); i++)
	{
		protoData ptem;
		//���������{}��������
		if (rootObj[keyList.at(i)].type() != QJsonValue::Object)
			continue;
		//תΪ����
		QJsonObject rootSecond = rootObj[keyList.at(i)].toObject();
		//�Ѷ�������ļ���ֵȡ����
		ptem.agreement = rootSecond["agreement"].toInt(0);
		ptem.modelName = rootSecond["modelName"].toString();
		//�������Ƕ�׶���
		if (rootSecond["mDItem"].type() == QJsonValue::Object)
		{
			//��mDItem��Value��Ϊ���󣬼�����������{}��������
			QJsonObject rootthird = rootSecond["mDItem"].toObject();
			QStringList keyListThird = rootthird.keys();
			for (int j = 0; j < keyListThird.size(); j++)
			{
				if (rootthird[keyListThird.at(j)].type() != QJsonValue::Object)
					continue;
				//mDitem����Ҳ�Ƕ��󣬶�Ӧ�����ݽṹ��struct canIdData,std::vector<canIdData>cItem;
				QJsonObject rootfour = rootthird[keyListThird.at(j)].toObject();
				canIdData ctemp;
				ctemp.CanId = rootfour["CanId"].toInt(0);
				ctemp.opt = rootfour["opt"].toInt(0);
				if (rootfour["pDItem"].isObject())
				{
					//pDitem��Ӧ����std::vector<struct protoItem>pItem;
					QJsonObject rootfift = rootfour["pDItem"].toObject();
					QStringList keyListFour = rootfift.keys();
					for (int k = 0; k < keyListFour.size(); k++)
					{
						if (!rootfift[keyListFour.at(k)].isArray())
							continue;
						//��Ӧ struct protoItem
						QJsonArray jarr = rootfift[keyListFour.at(k)].toArray();
						if (jarr.size() < 5)
							continue;
						protoItem dtemp;
						dtemp.bitName = jarr[0].toString();
						dtemp.startByte = jarr[1].toInt();
						dtemp.startBit = jarr[2].toInt();
						dtemp.bitLeng = jarr[3].toInt();
						dtemp.precision = jarr[4].toInt();
						dtemp.offset = jarr[5].toInt();
						ctemp.pItem.push_back(dtemp);
					}
				}

				ptem.cItem.push_back(ctemp);

			}
		}
		pGboleData.push_back(ptem);
	}
	isInit = true;
}

qGboleData::~qGboleData()
{
	if (pGboleInstance)
	{
		delete pGboleInstance;
		pGboleInstance = nullptr;
	}
}
