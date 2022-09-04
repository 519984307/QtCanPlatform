#include "QSetProperty.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <qcolordialog.h>
#include <qcolor.h>
#include <QMessageBox>
#include "AlgorithmSet.h"
#include "QsLog.h"
QSetProperty::QSetProperty(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	InitUI();
	this->setWindowModality(Qt::ApplicationModal);
	//ItitData();
}

QSetProperty::~QSetProperty()
{
	if (table)
	{
		delete table; table = nullptr;
	}
	stl_mcp = nullptr;
}

void QSetProperty::setIntoMap(std::map<QString, cellProperty>* cp)
{
	/*if (!cp)
		return;
	mcp = cp;
	if (!table)
	{
		return;
	}
	int row = table->rowCount();
	for (int i = 0; i < row; i++)
	{
		table->removeRow(row - i - 1);
	}
	std::map<QString, cellProperty>::iterator iBegin = cp->begin();
	std::map<QString, cellProperty>::iterator iEnd = cp->end();
	while (iBegin!=iEnd)
	{

		int rows = table->rowCount();
		table->setRowCount(rows + 1);
		table->setItem(rows, 0, new QTableWidgetItem(iBegin->first));
		table->setItem(rows, 1, new QTableWidgetItem(iBegin->second.toWord));
		QPushButton* pbColor = new QPushButton(tr("����ɫ"));
		QColor cu = QColor(iBegin->second.r, iBegin->second.g, iBegin->second.b);
		QString rgba = "background-color:#" + QString::number(cu.rgb(), 16);
		pbColor->setStyleSheet(rgba);
		connect(pbColor, SIGNAL(clicked()), this, SLOT(getColor()));
		table->setCellWidget(rows, 2, pbColor);
		iBegin++;
	}*/
	

}

void QSetProperty::setIntoMap(std::vector<cellProperty>* cp)
{
	if (!cp)
		return;
	stl_mcp = cp;
	if (!table)
	{
		return;
	}
	int row = table->rowCount();
	for (int i = 0; i < row; i++)
	{
		table->removeRow(row - i - 1);
	}
	for (int i = 0; i < stl_mcp->size(); i++)
	{
		int rows = table->rowCount();
		table->setRowCount(rows + 1);
		table->setItem(rows, 0, new QTableWidgetItem(stl_mcp->at(i).value));
		table->setItem(rows, 1, new QTableWidgetItem(stl_mcp->at(i).toWord));
		QPushButton* pbColor = new QPushButton(tr("����ɫ"));
		QColor cu = QColor(stl_mcp->at(i).r, stl_mcp->at(i).g, stl_mcp->at(i).b);
		QString rgba = "background-color:#" + QString::number(cu.rgb(), 16);
		pbColor->setStyleSheet(rgba);
		connect(pbColor, SIGNAL(clicked()), this, SLOT(getColor()));
		table->setCellWidget(rows, 2, pbColor);
	}
}

void QSetProperty::closeEvent(QCloseEvent* event)
{
	if (!table)
	{
		return;
	}
	int row = table->rowCount();
	for (int i = 0; i < row; i++)
	{
		table->removeRow(row - i - 1);
	}
	event->accept();
}


void QSetProperty::InitUI()
{
	QPushButton* pbAddItem = new QPushButton(tr("���"));
	connect(pbAddItem, SIGNAL(clicked()), this, SLOT(on_pbAddItem_clicked()));
	QPushButton* pbDelItem = new QPushButton(tr("ɾ��"));
	connect(pbDelItem, SIGNAL(clicked()), this, SLOT(on_pbDelItem_clicked()));
	QPushButton* pbSaveItem = new QPushButton(tr("����"));
	connect(pbSaveItem, SIGNAL(clicked()), this, SLOT(on_pbSaveItem_clicked()));
	QHBoxLayout* pbLayout = new QHBoxLayout();
	pbLayout->addWidget(pbAddItem);
	pbLayout->addWidget(pbDelItem);
	pbLayout->addWidget(pbSaveItem);
	pbLayout->addSpacerItem(new QSpacerItem(60, 20, QSizePolicy::Expanding));
	table = new QTableWidget(this);
	connect(table, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(on_itemDoubleClicked(int, int)));
	QStringList header;
	header << tr("ֵ") << tr("��������") << tr("��ʾ��ɫ");
	table->setColumnCount(3);
	table->setHorizontalHeaderLabels(header);
	table->setSelectionBehavior(QAbstractItemView::SelectRows);
	QLabel* tips = new QLabel(tr("���ô��ֶη��ص�ֵ��Ӧ��ʾ������,��ǰ����ʾ����ɫ"));
	QVBoxLayout* vb = new QVBoxLayout();
	vb->addLayout(pbLayout);
	vb->addWidget(table);
	vb->addWidget(tips);
	vb->setStretch(0, 1);
	vb->setStretch(1, 5);
	vb->setStretch(2, 1);
	this->setLayout(vb);

}

void QSetProperty::ItitData()
{
	
	if (!stl_mcp)
		return;
	
}

void QSetProperty::on_pbAddItem_clicked()
{
	if (!table) return;
	int rows = table->rowCount();
	table->setRowCount(rows + 1);
	table->setItem(rows, 0, new QTableWidgetItem(QString::number(rows)));
	table->setItem(rows, 1, new QTableWidgetItem(tr("����")));
	QPushButton* pbColor = new QPushButton(tr("����ɫ"));
	connect(pbColor, SIGNAL(clicked()), this, SLOT(getColor()));
	table->setCellWidget(rows, 2, pbColor);
	if (!stl_mcp)
	{
		QMessageBox::warning(NULL, tr("��ʾ"), tr("����ָ��δ��ʼ���������ݲ��ᱣ��"));
		return;
	}
	cellProperty cp;
	cp.r = 240;
	cp.g = 240;
	cp.b = 240;
	cp.toWord = "����";
	cp.value = QString::number(rows);
	//����
	stl_mcp->push_back(cp);
	//mcp->insert({ QString::number(rows), cp });
		
}

void QSetProperty::on_pbDelItem_clicked()
{
	int curRow = table->currentRow();
	if (curRow < 0)
		return;
	if (!stl_mcp)
		return;
	if (curRow > stl_mcp->size() - 1)
		return;
	std::vector<cellProperty>::iterator iBegin = stl_mcp->begin();
	std::vector<cellProperty>::iterator iEnd = stl_mcp->end();
	int  index = 0;
	while (iBegin!=iEnd)
	{
		//��Ϊpush��ȥ��˳�������˳��һ�������Ե�ǰ�е�λ�þ���vector��λ��
		//����vector�����ԣ�Ԫ�ص�˳�����ȥ�Ĵ����й�ϵ
		if (index == curRow) {
			stl_mcp->erase(iBegin);			//ɾ����
			table->removeRow(curRow);		//ͬʱҲ�Ƴ�����ϵ���һ��
			break;
		}
		else
		{
			iBegin++;
			index++;
		}
	}
}

void QSetProperty::on_pbSaveItem_clicked()
{
}
void QSetProperty::on_itemDoubleClicked(int,int)
{
	//Ҫ�༭����˫����������˫����ʱ���cell�仯���źŽ������������Ǳ༭֮����ź���
	connect(table, SIGNAL(cellChanged(int, int)), this, SLOT(on_table_cellChanged(int, int)));
}
void QSetProperty::on_table_cellChanged(int row,int col)
{
	if (!stl_mcp)
		return;
	if (row > stl_mcp->size() - 1 || col > 2)
	{
		QMessageBox::warning(NULL, tr("����"), tr("�޸�λ�ó�����Χ\non_table_cellChanged(int row,int col)"));
		return;
	}
	try
	{
		if (0 == col)
		{
			stl_mcp->at(row).value = table->item(row, col)->text();
		}
		else
		{
			stl_mcp->at(row).toWord = table->item(row, col)->text();
			//(*mcp)[QString::number(row)].toWord = table->item(row, col)->text();
		}
	}
	catch (const std::exception&e)
	{
		QLOG_WARN() << "�޸ĵ�Ԫ��ʱVector������Χ��" << e.what();
		QMessageBox::warning(NULL, tr("����"), tr("�޸ĵ�Ԫ��ʱVector������Χ\non_table_cellChanged(int row,int col)"));
	}
	
	//�ص�������Ȼ������е�ʱ��ᴥ��cellChanged����ź�
	disconnect(table, SIGNAL(cellChanged(int, int)), this, SLOT(on_table_cellChanged(int, int)));
}
void QSetProperty::getColor()
{
	if (!stl_mcp)
		return;
	QPushButton* send = dynamic_cast<QPushButton*>(sender());
	if (!send)
		return;
	QColor color = QColorDialog::getColor(Qt::white, this, tr("ѡ����ɫ"));
	//color.rgba();
	QString rgba = "background-color:#"+QString::number(color.rgb(), 16);
	send->setStyleSheet(rgba);
	QModelIndex index = table->indexAt(QPoint(send->geometry().x(), send->geometry().y()));
	int row = index.row();
	if (row > stl_mcp->size() - 1)
		return;
	stl_mcp->at(row).r = color.red();
	stl_mcp->at(row).g = color.green();
	stl_mcp->at(row).b = color.blue();
	
	/*(*mcp)[QString::number(row)].r = color.red();
	(*mcp)[QString::number(row)].g = color.green();
	(*mcp)[QString::number(row)].b = color.blue();*/
}