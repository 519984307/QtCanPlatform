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
}

QSetProperty::~QSetProperty()
{
	
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
	cp.color = QColor(240, 240, 240);
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
	QString rgba = "background-color:#"+QString::number(color.rgba(), 16);
	send->setStyleSheet(rgba);
	QModelIndex index = table->indexAt(QPoint(send->geometry().x(), send->geometry().y()));
	int row = index.row();
	if (!YB::keyInMap(*mcp, QString::number(row)))
	{
		return;
	}
	(*mcp)[QString::number(row)].color = color;
}