#include "QSetProperty.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <qcolordialog.h>
#include <qcolor.h>
QSetProperty::QSetProperty(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	InitUI();
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
	table->insertRow(rows);
	table->setItem(rows, 0, new QTableWidgetItem(QString::number(rows)));
	table->setItem(rows, 1, new QTableWidgetItem(tr("����")));
	QPushButton* pbColor = new QPushButton(tr("����ɫ"));
	connect(pbColor, SIGNAL(clicked()), this, SLOT(getColor()));
	table->setCellWidget(rows, 2, pbColor);
}

void QSetProperty::on_pbDelItem_clicked()
{
}

void QSetProperty::on_pbSaveItem_clicked()
{
}
void QSetProperty::getColor()
{
	QPushButton* send = dynamic_cast<QPushButton*>(sender());
	if (!send)
		return;
	QColor color = QColorDialog::getColor(Qt::white, this, tr("ѡ����ɫ"));
	color.rgba();
	QString rgba = "background-color:#"+QString::number(color.rgba(), 16);
	send->setStyleSheet(rgba);
}