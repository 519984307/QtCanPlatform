#include "QAutoTestIniData.h"
#include "qGboleData.h"
#pragma execution_character_set("utf-8")
QAutoTestIniData::QAutoTestIniData(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	InitUi();
	this->setWindowModality(Qt::ApplicationModal);
}

QAutoTestIniData::~QAutoTestIniData()
{

}
void QAutoTestIniData::InitUi()
{
	
	ui.tableWidget->setColumnCount(2);
	ui.tableWidget->setRowCount(7);
	QStringList aa{ "������", "����" };
	ui.tableWidget->setHorizontalHeaderLabels(aa);

	qGboleData* gb = qGboleData::getInstance();
	if (!gb)
		return;
	gb->read_ini();
	const struct autoTestData  at = gb->getATData();
	ui.tableWidget->setItem(0, 0, new QTableWidgetItem(tr("Ƿѹ�𲽵�ѹ(V)")));
	ui.tableWidget->setItem(0, 1, new QTableWidgetItem(QString::number(at.m_iLowVolt)));

	ui.tableWidget->setItem(1, 0, new QTableWidgetItem(tr("��ѹ�𲽵�ѹ(V)")));
	ui.tableWidget->setItem(1, 1, new QTableWidgetItem(QString::number(at.m_iHeightVolt)));

	ui.tableWidget->setItem(2, 0, new QTableWidgetItem(tr("��ѹ����(V)")));
	ui.tableWidget->setItem(2, 1, new QTableWidgetItem(QString::number(at.m_iVoltStep)));

	ui.tableWidget->setItem(3, 0, new QTableWidgetItem(tr("��ѹ���(V)")));
	ui.tableWidget->setItem(3, 1, new QTableWidgetItem(QString::number(at.m_iVoltError)));

	ui.tableWidget->setItem(4, 0, new QTableWidgetItem(tr("�����¶�(��C)")));
	ui.tableWidget->setItem(4, 1, new QTableWidgetItem(QString::number(at.m_iHeatTempture)));

	ui.tableWidget->setItem(5, 0, new QTableWidgetItem(tr("��¼������¶�(��C)")));
	ui.tableWidget->setItem(5, 1, new QTableWidgetItem(QString::number(at.m_iPowerTempture)));

	ui.tableWidget->setItem(6, 0, new QTableWidgetItem(tr("���Գ�ʱʱ��)(��)")));
	ui.tableWidget->setItem(6, 1, new QTableWidgetItem(QString::number(at.m_iOverTime)));
}
void QAutoTestIniData::on_pbSaveAData_clicked()
{
	struct autoTestData  at;

	at.m_iLowVolt = ui.tableWidget->item(0, 1)->text().toInt();
	at.m_iHeightVolt = ui.tableWidget->item(1, 1)->text().toInt();
	at.m_iHeatTempture = ui.tableWidget->item(2, 1)->text().toInt();
	at.m_iPowerTempture = ui.tableWidget->item(3, 1)->text().toInt();
	at.m_iOverTime = ui.tableWidget->item(4, 1)->text().toInt();

	qGboleData* gb = qGboleData::getInstance();
	if (!gb)
		return;
	gb->setATData(at);
	gb->save_ini();


}