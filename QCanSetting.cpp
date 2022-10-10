#include "QCanSetting.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QComboBox>
#include <qmessagebox.h>
#include <QDir>
#include <QJsonDocument>	// ����Json
#include <QJsonArray>		// [ ]
#include <QJsonObject>		// { }
#include <QJsonValue>		// int float double null { } [ ]
#include <QJsonParseError>
#include <QTextStream>
#include <QCHeckBox>
#include "qGboleData.h"
#include "QsLog.h"
QCanSetting::QCanSetting(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//��widgetģ̬����
	this->setWindowModality(Qt::ApplicationModal);
	InitUi();
	InitpGboleData();
}

QCanSetting::~QCanSetting()
{
	if (modelView)
	{
		delete modelView; modelView = nullptr;
	}
	if (canIdView)
	{
		delete canIdView; canIdView = nullptr;
	}
	if (tableView)
	{
		delete tableView; tableView = nullptr;
	}
}

void QCanSetting::closeEvent(QCloseEvent* event)
{
	event->accept();
	emit settingWidowsClose();
	QLOG_INFO() << "�豸���ڹر�";
}

void QCanSetting::InitUi()
{
	QLOG_INFO() << "���ڳ�ʼ�����ý��桭��";
	QStringList listname;
	listname << "����" << "Э��";
	QPushButton* pbAddModel = new QPushButton("����ͺ�");
	pbAddModel->setFixedWidth(50);
	connect(pbAddModel, &QPushButton::clicked, this, &QCanSetting::on_pbAddModel_clicked);
	QPushButton* pbMoveUpModel = new QPushButton("����");
	pbMoveUpModel->setFixedWidth(40);
	connect(pbMoveUpModel, &QPushButton::clicked, this, &QCanSetting::on_pbMoveUpModel_clicked);
	QPushButton* pbMoveDownModel = new QPushButton("����");
	pbMoveDownModel->setFixedWidth(40);
	connect(pbMoveDownModel, &QPushButton::clicked, this, &QCanSetting::on_pbMoveDownModel_clicked);
	QPushButton* pbDelModel = new QPushButton("ɾ��");
	pbDelModel->setFixedWidth(40);
	connect(pbDelModel, &QPushButton::clicked, this, &QCanSetting::on_pbDelModel_clicked);
	QPushButton* pbSaveModel = new QPushButton("����");
	pbSaveModel->setFixedWidth(40);
	connect(pbSaveModel, &QPushButton::clicked, this, &QCanSetting::on_pbSaveModel_clicked);
	//pbSaveModel->setMinimumWidth(40);
	QHBoxLayout* hLayoutMdeol = new QHBoxLayout();
	hLayoutMdeol->addWidget(pbAddModel);
	hLayoutMdeol->addWidget(pbMoveUpModel);
	hLayoutMdeol->addWidget(pbMoveDownModel);
	hLayoutMdeol->addWidget(pbDelModel);
	hLayoutMdeol->addWidget(pbSaveModel);
	hLayoutMdeol->addSpacerItem(new QSpacerItem(60, 20, QSizePolicy::Expanding));
	hLayoutMdeol->setSpacing(0);
	modelView = new QTableWidget();
	modelView->setColumnCount(2);
	modelView->setHorizontalHeaderLabels(listname);
	modelView->setSelectionBehavior(QAbstractItemView::SelectRows);
	modelView->setMaximumWidth(270);
	connect(modelView, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(on_modelView_doubleClicked(int, int)));
	connect(modelView, SIGNAL(cellClicked(int, int)), this, SLOT(on_modelView_Clicked(int, int)));
	
	QVBoxLayout* vLayoutModel = new QVBoxLayout();
	vLayoutModel->addLayout(hLayoutMdeol);
	vLayoutModel->addWidget(modelView);
	vLayoutModel->addSpacerItem(new QSpacerItem(20, 80, QSizePolicy::Expanding));

	listname.clear();
	listname << "CanId" << "����/����";
	QPushButton* pbAddCanId = new QPushButton("���ID");
	connect(pbAddCanId, &QPushButton::clicked, this, &QCanSetting::on_pbAddCanId_clicked);
	QPushButton* pbMoveUpCanId = new QPushButton("����");
	connect(pbMoveUpCanId, &QPushButton::clicked, this, &QCanSetting::on_pbMoveUpCanId_clicked);
	QPushButton* pbMoveDownCanId = new QPushButton("����");
	connect(pbMoveDownCanId, &QPushButton::clicked, this, &QCanSetting::on_pbMoveDownCanId_clicked);
	QPushButton* pbDelCanId = new QPushButton("ɾ��");
	connect(pbDelCanId, &QPushButton::clicked, this, &QCanSetting::on_pbDelCanId_clicked);
	QPushButton* pbSaveCanId = new QPushButton("����");
	connect(pbSaveCanId, &QPushButton::clicked, this, &QCanSetting::on_pbSaveCanId_clicked);
	pbAddCanId->setFixedWidth(50);
	pbMoveUpCanId->setFixedWidth(40);
	pbMoveDownCanId->setFixedWidth(40);
	pbDelCanId->setFixedWidth(40);
	pbSaveCanId->setFixedWidth(40);
	QHBoxLayout* hLayoutCanId = new QHBoxLayout();
	hLayoutCanId->addWidget(pbAddCanId);
	hLayoutCanId->addWidget(pbMoveUpCanId);
	hLayoutCanId->addWidget(pbMoveDownCanId);
	hLayoutCanId->addWidget(pbDelCanId);
	hLayoutCanId->addWidget(pbSaveCanId);
	hLayoutCanId->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding));
	hLayoutCanId->setSpacing(0);
	canIdView = new QTableWidget();
	canIdView->setColumnCount(3);
	canIdView->setHorizontalHeaderLabels(listname);
	canIdView->setSelectionBehavior(QAbstractItemView::SelectRows);
	canIdView->setMaximumWidth(270);
	connect(canIdView, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(on_canIdView_doubleClicked(int, int)));
	connect(canIdView, SIGNAL(cellClicked(int, int)), this, SLOT(on_canIdView_Clicked(int, int)));
	QVBoxLayout* vLayoutCanId = new QVBoxLayout();
	vLayoutCanId->addLayout(hLayoutCanId);
	vLayoutCanId->addWidget(canIdView);
	
	//vLayoutCanId->addSpacerItem(new QSpacerItem(20, 80, QSizePolicy::Expanding));

	QPushButton* pbAddIteam = new QPushButton("�����");
	connect(pbAddIteam, &QPushButton::clicked, this, &QCanSetting::on_pbAddIteam_clicked);
	QPushButton* pbMoveUpIteam = new QPushButton("����");
	connect(pbMoveUpIteam, &QPushButton::clicked, this, &QCanSetting::on_pbMoveUpIteam_clicked);
	QPushButton* pbMoveDownIteam = new QPushButton("����");
	connect(pbMoveDownIteam, &QPushButton::clicked, this, &QCanSetting::on_pbMoveDownIteam_clicked);
	QPushButton* pbDelIteam = new QPushButton("ɾ��");
	connect(pbDelIteam, &QPushButton::clicked, this, &QCanSetting::on_pbDelIteam_clicked);
	QPushButton* pbSaveIteam = new QPushButton("����");
	connect(pbSaveIteam, &QPushButton::clicked, this, &QCanSetting::on_pbSaveIteam_clicked);
	QHBoxLayout* hLayout = new QHBoxLayout();
	hLayout->addWidget(pbAddIteam);
	hLayout->addWidget(pbMoveUpIteam);
	hLayout->addWidget(pbMoveDownIteam);
	hLayout->addWidget(pbDelIteam);
	hLayout->addWidget(pbSaveIteam);
	hLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding));
	hLayout->setSpacing(0);
	//hLayout->addSpacerItem(new QSpacerItem(20, 80, QSizePolicy::Expanding));
	listname.clear();
	listname << tr("�ֶ�����") << tr("��ֹ�ֽ�") << tr("��ֹλ") << tr("����") << tr("����") << tr("ƫ����")<<tr("����")<<tr("������ʾ")<<tr("������Դ");
	
	tableView = new QTableWidget();
	tableView->setColumnCount(9);
	tableView->setHorizontalHeaderLabels(listname);
	tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	connect(tableView, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(on_tableView_doubleCLicked(int, int)));
	tableView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	QVBoxLayout* vLayout = new QVBoxLayout();
	vLayout->addLayout(hLayout);
	vLayout->addWidget(tableView);
	
	//�½�һ��ˮƽlayout
	QHBoxLayout* hLayoutAll = new QHBoxLayout();
	hLayoutAll->addLayout(vLayoutModel);
	hLayoutAll->addLayout(vLayoutCanId);
	hLayoutAll->addLayout(vLayout);
	//���ñ�����1��1��3
	hLayoutAll->setStretch(0, 1);
	hLayoutAll->setStretch(1, 1);
	hLayoutAll->setStretch(2, 10);
	hLayoutAll->setSpacing(5);
	this->setLayout(hLayoutAll);
	
	
	QLOG_INFO() << "���ý����ʼ����ɣ�Good Job��";
}
void QCanSetting::on_pbAddModel_clicked()
{
	
	if (!modelView)
		return;
	disconnect(modelView, SIGNAL(itemChanged(cellChanged(int, int))), this, SLOT(on_modelView_cellChanged(int, int)));
	short int row = modelView->rowCount();
	modelView->setRowCount(row + 1);
	QComboBox* proto = new QComboBox();
	proto->addItem(tr("Intel"));
	proto->addItem(tr("Motorola"));
	connect(proto, SIGNAL(currentIndexChanged(int)), this, SLOT(on_modelView_currentIndexChanged(int)));
	modelView->setItem(row, 0, new QTableWidgetItem(tr("�ͺ�")+QString::number(row)));
	modelView->setCellWidget(row, 1, proto);
	struct protoData data;
	data.modelName = tr("�ͺ�") + QString::number(row);
	data.agreement = 0;
	qGboleData* qGb = qGboleData::getInstance();
	if (!qGb)return;
	qGb->pGboleData.push_back(data);
	
}

void QCanSetting::savepGboleData()
{
	qGboleData* qGb = qGboleData::getInstance();
	if (!qGb)return;
	qGb->save();
}

void QCanSetting::InitpGboleData()
{
	qGboleData* qGb = qGboleData::getInstance();
	if (!qGb)return;
	if (!qGb->getIsInit())
		qGb->read();
	//��ʼ�����
	SetTableData();
}

void QCanSetting::SetTableData()
{
	qGboleData* qGb = qGboleData::getInstance();
	if (!qGb)return;
	if (qGb->pGboleData.size() < 0)
		return;
	for (int i = 0; i < qGb->pGboleData.size(); ++i)
	{
		
		modelView->setRowCount(i + 1);
		QComboBox* proto = new QComboBox();
		proto->addItem(tr("Intel"));
		proto->addItem(tr("Motorola"));
		connect(proto, SIGNAL(currentIndexChanged(int)), this, SLOT(on_modelView_currentIndexChanged(int)));
		modelView->setItem(i, 0, new QTableWidgetItem(qGb->pGboleData.at(i).modelName));
		modelView->setCellWidget(i, 1, proto);
		proto->setCurrentIndex(qGb->pGboleData.at(i).agreement);
	}
}

void QCanSetting::on_pbMoveUpModel_clicked()
{
	int curRow = modelView->currentRow();
	
	//��л���ûѡ�У�����
	if (curRow < 1)
	{
		return;
	}
	//���������Ȳ���һ��
	modelView->insertRow(curRow - 1);
	//����һ�е�������ȡ������Ҳ������
	QString idtex = modelView->item(curRow + 1, 0)->text();
	QComboBox* cb = dynamic_cast<QComboBox*>(modelView->cellWidget(curRow + 1, 1));
	//�ڵ�Ԫ��Ҫ����һ��Item����Ȼֱ�Ӷ�Ԫ�ؽ�ȥ
	modelView->setItem(curRow - 1, 0, new QTableWidgetItem(idtex));
	modelView->setCellWidget(curRow - 1, 1, cb);
	//ɾ��ԭ���У���Ϊ����ǰ��������һ�У��������Ҫ��
	modelView->removeRow(curRow + 1);

	qGboleData* qGb = qGboleData::getInstance();
	if (!qGb)return;
	if (curRow > qGb->pGboleData.size() - 1)
	{
		QMessageBox::warning(NULL, "warnning", tr("������Χ�����������ƶ�"));
		return;
	}
	std::swap(qGb->pGboleData[curRow], qGb->pGboleData[curRow - 1]);
	
}

void QCanSetting::on_pbMoveDownModel_clicked()
{
	int curRow = modelView->currentRow();
	
	//�������л���ûѡ�У�����
	if (curRow >= modelView->rowCount() - 1)
	{
		return;
	}
	
	modelView->insertRow(curRow + 2);
	QString idtex = modelView->item(curRow, 0)->text();
	QComboBox* cb = dynamic_cast<QComboBox*>(modelView->cellWidget(curRow, 1));
	modelView->setItem(curRow + 2, 0, new QTableWidgetItem(idtex));
	modelView->setCellWidget(curRow + 2, 1, cb);
	modelView->removeRow(curRow);

	qGboleData* qGb = qGboleData::getInstance();
	if (!qGb)return;
	if (curRow > qGb->pGboleData.size())
	{
		QMessageBox::warning(NULL, "warnning", tr("������Χ�����������ƶ�"));
		return;
	}
	std::swap(qGb->pGboleData[curRow], qGb->pGboleData[curRow + 1]);
}

void QCanSetting::on_pbDelModel_clicked()
{
	if (!modelView)
		return;
	short int index = modelView->currentIndex().row();
	if (index < 0)return;
	modelView->removeRow(index);
	qGboleData* qGb = qGboleData::getInstance();
	if (!qGb)return;
	if (qGb->pGboleData.size() - 1 < index)
		return;
	std::vector<protoData>::iterator iterb = qGb->pGboleData.begin();
	std::vector<protoData>::iterator itere = qGb->pGboleData.end();
	int n = 0;
	while (iterb != itere)
	{
		if (n == index)
		{
			qGb->pGboleData.erase(iterb);
			break;
		}
		else
		{
			iterb++;
			n++;
		}
			
	}
}

void QCanSetting::on_pbSaveModel_clicked()
{
	savepGboleData();
}

void QCanSetting::on_pbAddCanId_clicked()
{
	qGboleData* qGb = qGboleData::getInstance();
	if (!qGb)return;
	if (!canIdView)
		return;
	if (!modelView)
		return;
	if (modelView->rowCount() < 1)
		return;
	short int curSelectRow = modelView->currentRow();
	if (curSelectRow<0 || curSelectRow>qGb->pGboleData.size() - 1)
	{
		QMessageBox::warning(this, tr("warning"), tr("δѡ���ͺţ��������CanID"));
		return;
	}
		
	short int row = canIdView->rowCount();
	canIdView->setRowCount(row + 1);
	QComboBox* proto = new QComboBox();
	proto->addItem(tr("����"));
	proto->addItem(tr("����"));
	connect(proto, SIGNAL(currentIndexChanged(int)), this, SLOT(on_canIdView_currentIndexChanged(int)));

	canIdView->setItem(row, 0, new QTableWidgetItem(tr("123456")));
	canIdView->setCellWidget(row, 1, proto);

	QCheckBox* isSend = new QCheckBox();
	isSend->setChecked(false);
	canIdView->setCellWidget(row, 2, isSend);
	connect(isSend, &QCheckBox::stateChanged, this, &QCanSetting::on_canIdView_SendChanged);
	canIdData cdata;
	cdata.strCanId = canIdView->item(row,0)->text();
	cdata.opt = 0;
	qGb->pGboleData.at(curSelectRow).cItem.push_back(cdata);
}

void QCanSetting::on_pbMoveUpCanId_clicked()
{
	int curRow = canIdView->currentRow();
	int modelCurRow = modelView->currentRow();
	//��л���ûѡ�У�����
	if (curRow < 1 )
	{
		return;
	}
	if (modelCurRow < 0)
	{
		QMessageBox::warning(NULL, "warnning", tr("δѡ���ͺţ����������ƶ�"));
		return;
	}
		
	//���������Ȳ���һ��
	canIdView->insertRow(curRow - 1);
	//����һ�е�������ȡ������Ҳ������
	QString idtex = canIdView->item(curRow + 1, 0)->text();
	QComboBox* cb = dynamic_cast<QComboBox*>(canIdView->cellWidget(curRow + 1, 1));
	//�ڵ�Ԫ��Ҫ����һ��Item����Ȼֱ�Ӷ�Ԫ�ؽ�ȥ
	canIdView->setItem(curRow - 1, 0, new QTableWidgetItem(idtex));
	canIdView->setCellWidget(curRow - 1, 1, cb);
	//ɾ��ԭ���У���Ϊ����ǰ��������һ�У��������Ҫ��
	canIdView->removeRow(curRow + 1);

	qGboleData* qGb = qGboleData::getInstance();
	if (!qGb)return;
	if (curRow > qGb->pGboleData.at(modelCurRow).cItem.size() - 1)
	{
		QMessageBox::warning(NULL, "warnning", tr("������Χ�����������ƶ�"));
		return;
	}
	std::swap(qGb->pGboleData.at(modelCurRow).cItem[curRow], qGb->pGboleData.at(modelCurRow).cItem[curRow - 1]);
}

void QCanSetting::on_pbMoveDownCanId_clicked()
{
	int curRow = canIdView->currentRow();
	int modelCurRow = modelView->currentRow();
	//�������л���ûѡ�У�����
	if (curRow >= canIdView->rowCount()-1)
	{
		return;
	}
	if (modelCurRow < 0)
	{
		QMessageBox::warning(NULL, "warnning", tr("δѡ���ͺţ����������ƶ�"));
		return;
	}
	canIdView->insertRow(curRow + 2);
	QString idtex = canIdView->item(curRow , 0)->text();
	QComboBox* cb = dynamic_cast<QComboBox*>(canIdView->cellWidget(curRow, 1));
	canIdView->setItem(curRow + 2, 0, new QTableWidgetItem(idtex));
	canIdView->setCellWidget(curRow + 2, 1, cb);
	canIdView->removeRow(curRow );

	qGboleData* qGb = qGboleData::getInstance();
	if (!qGb)return;
	if (curRow > qGb->pGboleData.at(modelCurRow).cItem.size())
	{
		QMessageBox::warning(NULL, "warnning", tr("������Χ�����������ƶ�"));
		return;
	}
	std::swap(qGb->pGboleData.at(modelCurRow).cItem[curRow], qGb->pGboleData.at(modelCurRow).cItem[curRow + 1]);
}

void QCanSetting::on_pbDelCanId_clicked()
{
	if (!canIdView)
		return;
	
	short int index = canIdView->currentIndex().row();
	if (index < 0)return;
	canIdView->removeRow(index);
	qGboleData* qGb = qGboleData::getInstance();
	if (!qGb)return;
	short int mr = modelView->currentRow();
	if (mr<0 || mr>qGb->pGboleData.size() - 1)
		return;
	if (index > qGb->pGboleData.at(mr).cItem.size() - 1)
		return;
	std::vector<canIdData>::iterator itbe = qGb->pGboleData.at(mr).cItem.begin();
	std::vector<canIdData>::iterator itend = qGb->pGboleData.at(mr).cItem.end();
	int n = 0;
	while (itbe != itend)
	{
		if (n == index)
		{
			qGb->pGboleData.at(mr).cItem.erase(itbe);
			break;
		}
		else
		{
			n++;
			itbe++;
		}
	}
}

void QCanSetting::on_pbSaveCanId_clicked()
{
	savepGboleData();
}

void QCanSetting::on_pbAddIteam_clicked()
{
	qGboleData* qGb = qGboleData::getInstance();
	if (!qGb)return;
	if (!modelView)
		return;
	if (!tableView)
		return;
	if (!canIdView)
		return;
	if (modelView->rowCount() < 1)
		return;
	if (canIdView->rowCount() < 1)
		return;
	short int curSelectRow = modelView->currentRow();
	if (curSelectRow<0 || curSelectRow>qGb->pGboleData.size() - 1)
	{
		QMessageBox::warning(this, tr("warning"), tr("δѡ���ͺţ�������ӣ�Ҫ��ѡ���ͺţ�Ȼ����ѡ��CanID�������"));
		return;
	}
	short int curSelectCanRow = canIdView->currentRow();
	if (curSelectCanRow<0 || curSelectCanRow>qGb->pGboleData.at(curSelectRow).cItem.size() - 1)
	{
		QMessageBox::warning(this, tr("warning"), tr("δѡ��CanId��������ӣ�Ҫ��ѡ���ͺţ�Ȼ����ѡ��CanID�������"));
		return;
	}
	short int row = tableView->rowCount();
	tableView->setRowCount(row + 1);
	tableView->setItem(row, 0, new QTableWidgetItem(tr("�ֶ�")+QString::number(row)));
	tableView->setItem(row, 1, new QTableWidgetItem(QString::number(0)));
	tableView->setItem(row, 2, new QTableWidgetItem(QString::number(1)));
	tableView->setItem(row, 3, new QTableWidgetItem(QString::number(1)));
	tableView->setItem(row, 4, new QTableWidgetItem(QString::number(1)));
	tableView->setItem(row, 5, new QTableWidgetItem(QString::number(0)));
	QPushButton* property = new QPushButton(tr("����"));
	connect(property, SIGNAL(clicked()), this, SLOT(on_property_clicked()));
	tableView->setCellWidget(row, 6, property);
	QCheckBox* isRoll = new QCheckBox();
	tableView->setCellWidget(row, 7, isRoll);
	connect(isRoll, &QCheckBox::stateChanged, this, &QCanSetting::on_CheckStateChanged);
	tableView->setItem(row, 8, new QTableWidgetItem("-1"));
	protoItem pItem;
	pItem.bitName	= tableView->item(row, 0)->text();
	pItem.startByte = tableView->item(row, 1)->text().toInt(NULL, 10);
	pItem.startBit	= tableView->item(row, 2)->text().toInt(NULL, 10);
	pItem.bitLeng	= tableView->item(row, 3)->text().toInt(NULL, 10);
	pItem.precision = tableView->item(row, 4)->text().toFloat();
	pItem.offset	= tableView->item(row, 5)->text().toInt(NULL, 10);
	pItem.send = 0;
	pItem.isRoll = false;
	pItem.dataFrom = "-1";
	try
	{
		qGb->pGboleData.at(curSelectRow).cItem.at(curSelectCanRow).pItem.push_back(pItem);
	}
	catch (const std::exception&e)
	{
		QMessageBox::warning(this, tr("warning"), e.what());
	}
	catch(...) {
		QMessageBox::warning(this, tr("warning"),("Look this:"+QString::number(curSelectRow) + " and " + QString::number(curSelectCanRow)));
	}
	
}

void QCanSetting::on_pbMoveUpIteam_clicked()
{
	int canidCurRow = canIdView->currentRow();
	int modelCurRow = modelView->currentRow();
	int curRow = tableView->currentRow();
	//��л���ûѡ�У�����
	if (canidCurRow < 0 || modelCurRow < 0)
	{
		QMessageBox::warning(NULL, "warnning", tr("δѡ���ͺŻ���CANID�����������ƶ�"));
		return;
	}
	if (curRow<1)
	{
		return;
	}
	//���������Ȳ���һ��
	tableView->insertRow(curRow - 1);
	//�����е�����������ȡ����
	QString str1 = tableView->item(curRow + 1, 0)->text();
	QString str2 = tableView->item(curRow + 1, 1)->text();
	QString str3 = tableView->item(curRow + 1, 2)->text();
	QString str4 = tableView->item(curRow + 1, 3)->text();
	QString str5 = tableView->item(curRow + 1, 4)->text();
	QString str6 = tableView->item(curRow + 1, 5)->text();
	QPushButton* qpb = dynamic_cast<QPushButton*>(tableView->cellWidget(curRow + 1, 6));
	QCheckBox* qcb = dynamic_cast<QCheckBox*>(tableView->cellWidget(curRow + 1, 7));
	QString str9 = tableView->item(curRow + 1, 8)->text();
	//�ŵ��²������һ��
	tableView->setItem(curRow - 1, 0, new QTableWidgetItem(str1));
	tableView->setItem(curRow - 1, 1, new QTableWidgetItem(str2));
	tableView->setItem(curRow - 1, 2, new QTableWidgetItem(str3));
	tableView->setItem(curRow - 1, 3, new QTableWidgetItem(str4));
	tableView->setItem(curRow - 1, 4, new QTableWidgetItem(str5));
	tableView->setItem(curRow - 1, 5, new QTableWidgetItem(str6));
	tableView->setCellWidget(curRow - 1, 6, qpb);
	tableView->setCellWidget(curRow - 1, 7, qcb);
	tableView->setItem(curRow - 1, 8, new QTableWidgetItem(str9));
	tableView->setCellWidget(curRow - 1, 6, qpb);
	tableView->removeRow(curRow + 1);
	
	qGboleData* qGb = qGboleData::getInstance();
	if (!qGb)return;
	if (modelCurRow > qGb->pGboleData.size() - 1)
	{
		QMessageBox::warning(NULL, "warnning", tr("������Χ�����������ƶ�"));
		return;
	}
	if (canidCurRow > qGb->pGboleData.at(modelCurRow).cItem.size() - 1)
	{
		QMessageBox::warning(NULL, "warnning", tr("������Χ�����������ƶ�"));
		return;
	}
	if (curRow > qGb->pGboleData.at(modelCurRow).cItem.at(canidCurRow).pItem.size() - 1)
	{
		QMessageBox::warning(NULL, "warnning", tr("������Χ�����������ƶ�"));
		return;
	}
	std::swap(qGb->pGboleData.at(modelCurRow).cItem.at(canidCurRow).pItem[curRow], qGb->pGboleData.at(modelCurRow).cItem.at(canidCurRow).pItem[curRow - 1]);
}

void QCanSetting::on_pbMoveDownIteam_clicked()
{
	int canidCurRow = canIdView->currentRow();
	int modelCurRow = modelView->currentRow();
	int curRow = tableView->currentRow();
	//�������л���ûѡ�У�����
	if (canidCurRow < 0 || modelCurRow < 0)
	{
		QMessageBox::warning(NULL, "warnning", tr("δѡ���ͺŻ���CANID�����������ƶ�"));
		return;
	}
	if (curRow >= tableView->rowCount()-1)
	{
		return;
	}
	//���������Ȳ���һ��
	tableView->insertRow(curRow + 2);
	//�����е�����������ȡ����
	QString str1 = tableView->item(curRow , 0)->text();
	QString str2 = tableView->item(curRow , 1)->text();
	QString str3 = tableView->item(curRow , 2)->text();
	QString str4 = tableView->item(curRow , 3)->text();
	QString str5 = tableView->item(curRow , 4)->text();
	QString str6 = tableView->item(curRow , 5)->text();
	QPushButton* qpb = dynamic_cast<QPushButton*>(tableView->cellWidget(curRow , 6));
	QCheckBox* qcb = dynamic_cast<QCheckBox*>(tableView->cellWidget(curRow , 7));
	QString str9 = tableView->item(curRow , 8)->text();
	//�ŵ��²������һ��
	tableView->setItem(curRow + 2, 0, new QTableWidgetItem(str1));
	tableView->setItem(curRow + 2, 1, new QTableWidgetItem(str2));
	tableView->setItem(curRow + 2, 2, new QTableWidgetItem(str3));
	tableView->setItem(curRow + 2, 3, new QTableWidgetItem(str4));
	tableView->setItem(curRow + 2, 4, new QTableWidgetItem(str5));
	tableView->setItem(curRow + 2, 5, new QTableWidgetItem(str6));
	tableView->setCellWidget(curRow + 2, 6, qpb);
	tableView->setCellWidget(curRow + 2, 7, qcb);
	tableView->setItem(curRow + 2, 8, new QTableWidgetItem(str9));
	tableView->removeRow(curRow);
	qGboleData* qGb = qGboleData::getInstance();
	if (!qGb)return;
	if (modelCurRow > qGb->pGboleData.size() - 1)
	{
		QMessageBox::warning(NULL, "warnning", tr("������Χ�����������ƶ�"));
		return;
	}
	if (canidCurRow > qGb->pGboleData.at(modelCurRow).cItem.size() - 1)
	{
		QMessageBox::warning(NULL, "warnning", tr("������Χ�����������ƶ�"));
		return;
	}
	if (curRow > qGb->pGboleData.at(modelCurRow).cItem.at(canidCurRow).pItem.size())
	{
		QMessageBox::warning(NULL, "warnning", tr("������Χ�����������ƶ�"));
		return;
	}
	std::swap(qGb->pGboleData.at(modelCurRow).cItem.at(canidCurRow).pItem[curRow], qGb->pGboleData.at(modelCurRow).cItem.at(canidCurRow).pItem[curRow + 1]);
}
//��������ɾ��
void QCanSetting::on_pbDelIteam_clicked()
{
	if (!tableView)
		return;
	short int index = tableView->currentIndex().row();
	if (index < 0)return;
	tableView->removeRow(index);

	qGboleData* qGb = qGboleData::getInstance();
	if (!qGb)return;
	if (!modelView)
		return;
	
	if (!canIdView)
		return;
	//����һ����û�ж�����ֱ��return
	if (modelView->rowCount() < 1)
		return;
	//�ϵڶ�����û�ж�����ͬ��return
	if (canIdView->rowCount() < 1)
		return;
	//������Χ
	short int curSelectRow = modelView->currentRow();
	if (curSelectRow<0 || curSelectRow>qGb->pGboleData.size() - 1)
	{
		QMessageBox::warning(this, tr("warning"), tr("δѡ���ͺţ�����ɾ����Ҫ��ѡ���ͺţ�Ȼ����ѡ��CanID����ɾ��"));
		return;
	}
	//������Χ
	short int curSelectCanRow = canIdView->currentRow();
	try
	{
		if (curSelectCanRow<0 || curSelectCanRow>qGb->pGboleData.at(curSelectRow).cItem.size() - 1)
		{
			QMessageBox::warning(this, tr("warning"), tr("δѡ��CanId������ɾ����Ҫ��ѡ���ͺţ�Ȼ����ѡ��CanID����ɾ��"));
			return;
		}
		//������Χ
		if (index<0 || index > qGb->pGboleData.at(curSelectRow).cItem.at(curSelectCanRow).pItem.size()-1)
		{
			QMessageBox::warning(this, tr("warning"), tr("����ɾ����������Vector����"));
			return;
		}
		//ʹ�õ�����
		std::vector<protoItem>::iterator itbegin= qGb->pGboleData.at(curSelectRow).cItem.at(curSelectCanRow).pItem.begin();
		std::vector<protoItem>::iterator itend= qGb->pGboleData.at(curSelectRow).cItem.at(curSelectCanRow).pItem.end();
		int n = 0;
		while (itbegin != itend)
		{
			//�ҵ�����λ��,��Ϊvector������ģ�����е�ͬʱҲpush��vector�ˣ��е�λ�þ���vector��λ��
			if (n == index)
			{
				//ʹ��erase�Ƴ�
				qGb->pGboleData.at(curSelectRow).cItem.at(curSelectCanRow).pItem.erase(itbegin);
				break;
			}
			else
			{
				itbegin++;
				n++;
			}
		}
	}
	catch (const std::exception&e)
	{
		QLOG_WARN() << "ɾ����ʱVector������Χ��" << e.what();
		QMessageBox::warning(NULL, tr("����"), tr("ɾ����ʱVector������Χ\non_pbDelIteam_clicked"));
	}
}

void QCanSetting::on_pbSaveIteam_clicked()
{
	savepGboleData();
}

void QCanSetting::on_modelView_currentIndexChanged(int index)
{
	qGboleData* qGb = qGboleData::getInstance();
	if (!qGb)return;
	//ȡ�������ߵ�ָ��
	QComboBox* cbx = qobject_cast<QComboBox*>(sender());
	if (cbx == nullptr)
		return;
	//ȡ��ť��������
	QModelIndex idx = modelView->indexAt(QPoint(cbx->frameGeometry().x(), cbx->frameGeometry().y()));
	int row = idx.row();
	if (row<0 || row>qGb->pGboleData.size() - 1)
	{
		QMessageBox::warning(this, tr("����"), tr("���ݳ�����Χ"));
		return;
	}
	qGb->pGboleData.at(row).agreement = cbx->currentIndex();
}

void QCanSetting::on_modelView_cellChanged(int row, int col)
{
	qGboleData* qGb = qGboleData::getInstance();
	if (!qGb)return;
	if (row<0 || row>qGb->pGboleData.size() - 1||col!=0)
	{
		return;
	}
	qGb->pGboleData.at(row).modelName = modelView->item(row,col)->text();
	//�ص��������ֹ�����setData��ʱ�򣬴�������ź�
	disconnect(modelView, SIGNAL(itemChanged(cellChanged(int, int))), this, SLOT(on_modelView_cellChanged(int, int)));
}

void QCanSetting::on_modelView_doubleClicked(int, int)
{
	//ע��QT�����cellChanged�źţ����ᵱsetData��ʱ�򴥷�����ź�
	//�������߾ȹ�����˫����ʱ�����������ۣ�
	connect(modelView, SIGNAL(cellChanged(int, int)), this, SLOT(on_modelView_cellChanged(int, int)));
}
/***********************************
*@brief:�б��ڳ�ʼ��ʱ��ֻ���ʼ���ͺ��б��
*���ͺ������CanId vector���ݣ�ֻ������ڵ��ĳ���ͺ�ʱ
* �Ż��ʼ�������
* @return: void
************************************/
void QCanSetting::on_modelView_Clicked(int row, int col)
{
	qGboleData* qGb = qGboleData::getInstance();
	if (!qGb)return;
	if (!canIdView)
		return;
	if (!modelView)
		return;
	if (row < 0)
		return;
	int rowCount = canIdView->rowCount();
	for (int m = 0; m < rowCount; m++)
		canIdView->removeRow(rowCount - m - 1);

	if (row > qGb->pGboleData.size() - 1)
		return;
	try
	{
		for (int i = 0; i < qGb->pGboleData.at(row).cItem.size(); ++i)
		{
			canIdView->setRowCount(i + 1);
			QComboBox* proto = new QComboBox();
			proto->addItem(tr("����"));
			proto->addItem(tr("����"));
			proto->setCurrentIndex(qGb->pGboleData.at(row).cItem.at(i).opt);
			//QString mt = "0x" + QString("%1").arg(qGb->pGboleData.at(row).cItem.at(i).CanId, QString::number(qGb->pGboleData.at(row).cItem.at(i).CanId, 16).length(), 16).toUpper().trimmed();
			QString mt = qGb->pGboleData.at(row).cItem.at(i).strCanId;
			canIdView->setItem(i, 0, new QTableWidgetItem(mt));
			canIdView->setCellWidget(i, 1, proto);
			connect(proto, SIGNAL(currentIndexChanged(int)), this, SLOT(on_canIdView_currentIndexChanged(int)));

			QCheckBox* isSend = new QCheckBox();
			isSend->setChecked(qGb->pGboleData.at(row).cItem.at(i).isSend);
			canIdView->setCellWidget(i, 2, isSend);
			connect(isSend, &QCheckBox::stateChanged, this, &QCanSetting::on_canIdView_SendChanged);
		}
	}
	catch (const std::exception&e)
	{
		QMessageBox::warning(this, tr("warning"), QString(tr("Vector����:") + e.what()));
	}
	canIdView->setCurrentCell(0, 0);
	
	if (!tableView)
		return;
	on_canIdView_Clicked(0, 0);
}

void QCanSetting::on_canIdView_currentIndexChanged(int index)
{
	qGboleData* qGb = qGboleData::getInstance();
	if (!qGb)return;
	QComboBox* cb = dynamic_cast<QComboBox*>(sender());
	if (!cb)
		return;
	if (!modelView) return;
	int mCurRow = modelView->currentRow();
	if (mCurRow < 0 || mCurRow>qGb->pGboleData.size() - 1) {
		QMessageBox::warning(this, tr("warning"), tr("δѡ���ͺţ� �����޸�CanID"));
		return;
	}
	if (!canIdView) return;
	QModelIndex pIndex = canIdView->indexAt(QPoint(cb->frameGeometry().x(), cb->frameGeometry().y()));
	int row = pIndex.row();

	if (row > qGb->pGboleData.at(mCurRow).cItem.size() - 1 || row < 0 )
	{
		QMessageBox::warning(this, tr("warning"), tr("�޸ĵ�λ�ó�����Χ"));
		return;
	}
	try
	{
		qGb->pGboleData.at(mCurRow).cItem.at(row).opt = cb->currentIndex();
	}
	catch (const std::exception&e)
	{
		QMessageBox::warning(this, tr("warning"), QString(tr("Vector����:") + e.what() + "Infunction:on_canIdView_currentIndexChanged"));
	}

}
void QCanSetting::on_canIdView_SendChanged(int check)
{
	qGboleData* qGb = qGboleData::getInstance();
	if (!qGb)return;
	QCheckBox* cb = dynamic_cast<QCheckBox*>(sender());
	if (!cb)
		return;
	if (!modelView) return;
	int mCurRow = modelView->currentRow();
	if (mCurRow < 0 || mCurRow>qGb->pGboleData.size() - 1) {
		QMessageBox::warning(this, tr("warning"), tr("δѡ���ͺţ� �����޸�CanID"));
		return;
	}
	if (!canIdView) return;
	QModelIndex pIndex = canIdView->indexAt(QPoint(cb->frameGeometry().x(), cb->frameGeometry().y()));
	int row = pIndex.row();

	if (row > qGb->pGboleData.at(mCurRow).cItem.size() - 1 || row < 0)
	{
		QMessageBox::warning(this, tr("warning"), tr("�޸ĵ�λ�ó�����Χ"));
		return;
	}
	try
	{
		bool b = check > 0 ? true : false;
		qGb->pGboleData.at(mCurRow).cItem.at(row).isSend = b;
	}
	catch (const std::exception& e)
	{
		QMessageBox::warning(this, tr("warning"), QString(tr("Vector����:") + e.what() + "Infunction:on_canIdView_currentIndexChanged"));
	}
}

/*
* @brief���ڶ������޸ĺ�Ĳۺ��������ڱ����޸ĺ��ֵ
* @param row�����޸ĵĵ�Ԫ���������
* @param col�����޸ĵĵ�Ԫ���������
* @return����
*/
void QCanSetting::on_canIdView_cellChanged(int row, int col)
{
	qGboleData* qGb = qGboleData::getInstance();
	if (!qGb)return;
	if (!modelView) return;
	int mCurRow = modelView->currentRow();
	if (mCurRow < 0 ||mCurRow>qGb->pGboleData.size()-1) {
		QMessageBox::warning(this, tr("warning"), tr("δѡ���ͺţ� �����޸�CanID"));
		return;
	}
	if (!canIdView) return;
	try
	{
		//���ڷ�Χ
		if (row > qGb->pGboleData.at(mCurRow).cItem.size() - 1 || row <0||col!=0)
		{
			QMessageBox::warning(this, tr("warning"), tr("�޸ĵ�λ�ó�����Χ"));
			return;
		}
		//����ֻ��ID���ǿ����޸ĵ�
		qGb->pGboleData.at(mCurRow).cItem.at(row).strCanId = canIdView->item(row,col)->text().trimmed();
	}
	catch (const std::exception&e)
	{
		QMessageBox::warning(this, tr("warning"), QString(tr("Vector����:") + e.what() + "Infunction:on_canIdView_cellChanged"));
	}
	//�ص��������ֹ�����setData��ʱ�򣬴�������ź�
	disconnect(canIdView, SIGNAL(itemChanged(cellChanged(int, int))), this, SLOT(on_canIdView_cellChanged(int, int)));
}
/*
* @brief���ڶ����˫���¼���Ӧ���������༭��Ĳ�
*/
void QCanSetting::on_canIdView_doubleClicked(int, int)
{
	//ע��QT�����cellChanged�źţ����ᵱsetData��ʱ�򴥷�����ź�
	//�������߾ȹ�����˫����ʱ�����������ۣ�
	connect(canIdView, SIGNAL(cellChanged(int, int)), this, SLOT(on_canIdView_cellChanged(int, int)));
}
/*
* @brief: ��tableView����������ʾcanId��Ӧ����
* @return: void
*/
void QCanSetting::on_canIdView_Clicked(int row, int col)
{
	qGboleData* qGb = qGboleData::getInstance();
	if (!qGb)return;
	if (!tableView)
		return;
	if (!canIdView)
		return;
	if (!modelView)
		return;
	if (row < 0)
		return;
	//��ȡ��ǰѡ�е��ͺţ���ûѡ�У��򲻹�
	short mRow = modelView->currentRow();
	if (mRow < 0 || mRow > qGb->pGboleData.size() - 1)
		return;
	//���֮ǰ��������
	int rowCount = tableView->rowCount();
	for (int m = 0; m < rowCount; m++)
		tableView->removeRow(rowCount - m - 1);
	int count = qGb->pGboleData.at(mRow).cItem.size()-1;
	if (row > count)
		return;
	try
	{
		//��ʾ��ӦCanID����
		for (int i = 0; i < qGb->pGboleData.at(mRow).cItem.at(row).pItem.size(); i++)
		{
			//short int crow = tableView->rowCount();
			tableView->setRowCount(i + 1);
			tableView->setItem(i, 0, new QTableWidgetItem(qGb->pGboleData.at(mRow).cItem.at(row).pItem.at(i).bitName));
			tableView->setItem(i, 1, new QTableWidgetItem(QString::number(qGb->pGboleData.at(mRow).cItem.at(row).pItem.at(i).startByte)));
			tableView->setItem(i, 2, new QTableWidgetItem(QString::number(qGb->pGboleData.at(mRow).cItem.at(row).pItem.at(i).startBit)));
			tableView->setItem(i, 3, new QTableWidgetItem(QString::number(qGb->pGboleData.at(mRow).cItem.at(row).pItem.at(i).bitLeng)));
			tableView->setItem(i, 4, new QTableWidgetItem(QString::number(qGb->pGboleData.at(mRow).cItem.at(row).pItem.at(i).precision)));
			tableView->setItem(i, 5, new QTableWidgetItem(QString::number(qGb->pGboleData.at(mRow).cItem.at(row).pItem.at(i).offset)));
			QPushButton* property = new QPushButton(tr("����"));
			connect(property, SIGNAL(clicked()), this, SLOT(on_property_clicked()));
			tableView->setCellWidget(i, 6, property);
			QCheckBox* isRoll = new QCheckBox();
			if (qGb->pGboleData.at(mRow).cItem.at(row).pItem.at(i).isRoll)
				isRoll->setCheckState(Qt::Checked);
			tableView->setCellWidget(i, 7, isRoll);
			//���connectһ������setCheckState(Qt::Checked);����������棬��Ȼһ��������������ͻᴥ���ź�
			connect(isRoll, &QCheckBox::stateChanged, this, &QCanSetting::on_CheckStateChanged);
			tableView->setItem(i, 8, new QTableWidgetItem(qGb->pGboleData.at(mRow).cItem.at(row).pItem.at(i).dataFrom));
		}
	}
	catch (const std::exception&e)
	{
		QMessageBox::warning(this, tr("warning"), QString(tr("Vector����:")+e.what()+ "Infunction:on_canIdView_Clicked"));
		return;
	}
}

void QCanSetting::on_tableView_doubleCLicked(int row , int col)
{
	//�����༭����źŲ�
	connect(tableView, SIGNAL(cellChanged(int, int)), this, SLOT(on_tableView_cellChanged(int, int)));
}

void QCanSetting::on_tableView_cellChanged(int row, int col)
{
	qGboleData* qGb = qGboleData::getInstance();
	if (!qGb)return;
	if (!tableView)
		return;
	if (!canIdView)
		return;
	if (!modelView)
		return;
	if (row < 0 || col < 0)
		return;
	try
	{
		int mCurRow = modelView->currentRow();
		if (mCurRow<0 || mCurRow>qGb->pGboleData.size() - 1)
		{
			QMessageBox::warning(this, tr("warning"), QString(tr("δѡ���ͺţ������޸�")));
			return;
		}
		int cCurRow = canIdView->currentRow();
		if (cCurRow<0 || cCurRow>qGb->pGboleData.at(mCurRow).cItem.size() - 1)
		{
			QMessageBox::warning(this, tr("warning"), QString(tr("δѡ���ͺţ������޸�")));
			return;
		}
		if (row > qGb->pGboleData.at(mCurRow).cItem.at(cCurRow).pItem.size() - 1)
		{
			QMessageBox::warning(this, tr("warning"), QString(tr("�ֶ�vector������Χ��\nrow > pGboleData.at(mCurRow).cItem.at(cCurRow).pItem.size() - 1")));
			return;
		}
		//��ȡ�޸ĵĵ�ֵ
		switch (col)
		{
		case 0:
			qGb->pGboleData.at(mCurRow).cItem.at(cCurRow).pItem.at(row).bitName = tableView->item(row, col)->text();
			break;
		case 1:
			qGb->pGboleData.at(mCurRow).cItem.at(cCurRow).pItem.at(row).startByte = tableView->item(row, col)->text().toInt();
			break;
		case 2:
			qGb->pGboleData.at(mCurRow).cItem.at(cCurRow).pItem.at(row).startBit = tableView->item(row, col)->text().toInt();
			break;
		case 3:
			qGb->pGboleData.at(mCurRow).cItem.at(cCurRow).pItem.at(row).bitLeng = tableView->item(row, col)->text().toInt();
			break;
		case 4:
			qGb->pGboleData.at(mCurRow).cItem.at(cCurRow).pItem.at(row).precision = tableView->item(row, col)->text().toFloat();
			break;
		case 5:
			qGb->pGboleData.at(mCurRow).cItem.at(cCurRow).pItem.at(row).offset = tableView->item(row, col)->text().toInt();
			break;
		case 8:
			qGb->pGboleData.at(mCurRow).cItem.at(cCurRow).pItem.at(row).dataFrom = tableView->item(row, col)->text().trimmed();
			break;
		default:
			break;
		}
	}
	catch (const std::exception&e)
	{
		QMessageBox::warning(this, tr("warning"), QString(tr("Vector����:") + e.what()+"Infunction:on_tableView_cellChanged"));
		//return;
	}
	//����Ҫ������ص�����Ȼ�û�����ӵ���ʱ�򣬻ᴥ������ۺ���
	disconnect(tableView, SIGNAL(cellChanged(int, int)), this, SLOT(on_tableView_cellChanged(int, int)));
}

void QCanSetting::on_property_clicked()
{
	QPushButton* pb = dynamic_cast<QPushButton*>(sender());
	if (!pb)
		return;
	if (!tableView)
		return;
	if (!canIdView)
		return;
	if (!modelView)
		return;
	QModelIndex inedx = tableView->indexAt(QPoint(pb->geometry().x(), pb->geometry().y()));
	int row = inedx.row();
	qGboleData* qGb = qGboleData::getInstance();
	if (!qGb)return;
	
	try
	{
		//�жϵ�һ�����Ƿ�ѡ��
		int mCurRow = modelView->currentRow();
		if (mCurRow<0 || mCurRow>qGb->pGboleData.size() - 1)
		{
			QMessageBox::warning(this, tr("warning"), QString(tr("δѡ���ͺţ������޸�")));
			return;
		}
		//�жϵڶ������Ƿ�ѡ��
		int cCurRow = canIdView->currentRow();
		if (cCurRow<0 || cCurRow>qGb->pGboleData.at(mCurRow).cItem.size() - 1)
		{
			QMessageBox::warning(this, tr("warning"), QString(tr("δѡ���ͺţ������޸�")));
			return;
		}
		//�ж�Button�Ƿ��ڵ�����������ݷ�Χ��
		if (row > qGb->pGboleData.at(mCurRow).cItem.at(cCurRow).pItem.size() - 1 || row <0)
		{
			QMessageBox::warning(this, tr("warning"), QString(tr("�ֶ�vector������Χ��\nrow > pGboleData.at(mCurRow).cItem.at(cCurRow).pItem.size() - 1")));
			return;
		}
		if (!pp)
		{
			pp = new QSetProperty();
		}
		//ȡ�������map
		//std::map<QString, cellProperty>&mapMM = qGb->pGboleData.at(mCurRow).cItem.at(cCurRow).pItem.at(row).itemProperty;
		//ȡ�������vector
		std::vector<cellProperty>& stlMM = qGb->pGboleData.at(mCurRow).cItem.at(cCurRow).pItem.at(row).stl_itemProperty;
		//����ָ���ȥ��C/C++ָ����TMD�ĺã�
		pp->setIntoMap(&stlMM);
		pp->setWindowFlag(Qt::Window);
		pp->show();
	}
	catch (const std::exception& e)
	{
		QMessageBox::warning(this, tr("warning"), QString(tr("Vector����:") + e.what() + "Infunction:on_property_clicked"));
		//return;
	}

	
}

void QCanSetting::on_CheckStateChanged(int isCheck)
{
	QCheckBox* pb = dynamic_cast<QCheckBox*>(sender());
	if (!pb)
		return;
	if (!tableView)
		return;
	if (!canIdView)
		return;
	if (!modelView)
		return;
	QModelIndex inedx = tableView->indexAt(QPoint(pb->geometry().x(), pb->geometry().y()));
	int row = inedx.row();
	qGboleData* qGb = qGboleData::getInstance();
	if (!qGb)return;

	try
	{
		//�жϵ�һ�����Ƿ�ѡ��
		int mCurRow = modelView->currentRow();
		if (mCurRow<0 || mCurRow>qGb->pGboleData.size() - 1)
		{
			QMessageBox::warning(this, tr("warning"), QString(tr("δѡ���ͺţ������޸�")));
			return;
		}
		//�жϵڶ������Ƿ�ѡ��
		int cCurRow = canIdView->currentRow();
		if (cCurRow<0 || cCurRow>qGb->pGboleData.at(mCurRow).cItem.size() - 1)
		{
			QMessageBox::warning(this, tr("warning"), QString(tr("δѡ���ͺţ������޸�")));
			return;
		}
		//�ж�Button�Ƿ��ڵ�����������ݷ�Χ��
		if (row > qGb->pGboleData.at(mCurRow).cItem.at(cCurRow).pItem.size() - 1 || row < 0)
		{
			QMessageBox::warning(this, tr("warning"), QString(tr("�ֶ�vector������Χ��\nrow > pGboleData.at(mCurRow).cItem.at(cCurRow).pItem.size() - 1")));
			return;
		}
		qGb->pGboleData.at(mCurRow).cItem.at(cCurRow).pItem.at(row).isRoll = isCheck==2?true:false;
		
	}
	catch (const std::exception& e)
	{
		QMessageBox::warning(this, tr("warning"), QString(tr("Vector����:") + e.what() + "Infunction:on_CheckStateChanged(int isCheck)"));
		//return;
	}
}
