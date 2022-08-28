#include "QSetProperty.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <qcolordialog.h>
#include <qcolor.h>
#include <QMessageBox>
#include "AlgorithmSet.h"
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
	mcp = nullptr;
}

void QSetProperty::setIntoMap(std::map<QString, cellProperty>* cp)
{
	if (!cp)
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
	QStringList header;
	header << tr("����ֵ") << tr("��Ӧ����") << tr("��ʾ��ɫ");
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
	
	if (!mcp)
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
	if (!mcp)
	{
		QMessageBox::warning(NULL, tr("��ʾ"), tr("����ָ��δ��ʼ���������ݲ��ᱣ��"));
		return;
	}
	cellProperty cp;
	cp.r = 240;
	cp.g = 240;
	cp.b = 240;
	cp.toWord = "����";
	//����
	mcp->insert({ QString::number(rows), cp });
		
}

void QSetProperty::on_pbDelItem_clicked()
{
}

void QSetProperty::on_pbSaveItem_clicked()
{
}
void QSetProperty::on_itemDoubleClicked()
{
	//Ҫ�༭����˫����������˫����ʱ���cell�仯���źŽ������������Ǳ༭֮����ź���
	connect(table, SIGNAL(cellChanged(int, int)), this, SLOT(on_table_cellChanged(int, int)));
}
void QSetProperty::on_table_cellChanged(int row,int col)
{

	//�ص�������Ȼ������е�ʱ��ᴥ��cellChanged����ź�
	disconnect(table, SIGNAL(cellChanged(int, int)), this, SLOT(on_table_cellChanged(int, int)));
}
void QSetProperty::getColor()
{
	QPushButton* send = dynamic_cast<QPushButton*>(sender());
	if (!send)
		return;
	QColor color = QColorDialog::getColor(Qt::white, this, tr("ѡ����ɫ"));
	//color.rgba();
	QString rgba = "background-color:#"+QString::number(color.rgb(), 16);
	send->setStyleSheet(rgba);
	QModelIndex index = table->indexAt(QPoint(send->geometry().x(), send->geometry().y()));
	int row = index.row();
	if (!YB::keyInMap(*mcp, QString::number(row)))
	{
		return;
	}
	(*mcp)[QString::number(row)].r = color.red();
	(*mcp)[QString::number(row)].g = color.green();
	(*mcp)[QString::number(row)].b = color.blue();
}