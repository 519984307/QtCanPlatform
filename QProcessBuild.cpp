#include "QProcessBuild.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
QProcessBuild::QProcessBuild(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	InitUI();
}

QProcessBuild::~QProcessBuild()
{
	
}
/*
* @brief:	��ʼ��������̵Ľ���,��Ҫ�ɰ�ť��table����
*			
* @param:	void
* 
* @return:	void
*/
void QProcessBuild::InitUI()
{
	addButton = new QPushButton(tr("�������"));
	removeButton = new QPushButton(tr("�Ƴ�����"));
	upButton = new QPushButton(tr("����"));
	downButton = new QPushButton(tr("����"));

	connect(addButton, &QPushButton::clicked, this, &QProcessBuild::on_addButton_clicked);
	connect(removeButton, &QPushButton::clicked, this, &QProcessBuild::on_removeButton_clicked);
	connect(upButton, &QPushButton::clicked, this, &QProcessBuild::on_upButton_clicked);
	connect(downButton, &QPushButton::clicked, this, &QProcessBuild::on_downButton_clicked);
	QHBoxLayout* vBoxLButton = new QHBoxLayout();
	vBoxLButton->addWidget(addButton);
	vBoxLButton->addWidget(removeButton);
	vBoxLButton->addWidget(upButton);
	vBoxLButton->addWidget(downButton);
	vBoxLButton->addSpacerItem(new QSpacerItem(20, 20,QSizePolicy::Expanding));

	processTable = new QTableWidget();
	QStringList title;
	title << tr("��Դ") << tr("ѡ��") << tr("�ֶ�/����") << tr("�ȶԷ�ʽ") << tr("�Ա�ֵ") << tr("������Դ") << tr("����ѡ��") << tr("�ֶ�/����") << tr("�ȶԷ�ʽ") << tr("�Ա�ֵ");
	processTable->setColumnCount(10);
	processTable->setHorizontalHeaderLabels(title);
	QHBoxLayout* vBoxLTable = new QHBoxLayout();
	vBoxLTable->addWidget(processTable);
	QVBoxLayout* hBoxLWid = new QVBoxLayout();
	hBoxLWid->addLayout(vBoxLButton);
	hBoxLWid->addLayout(vBoxLTable);
	hBoxLWid->setStretch(0, 1);
	hBoxLWid->setStretch(1, 10);
	this->setLayout(hBoxLWid);
}
void QProcessBuild::on_addButton_clicked()
{

}
void QProcessBuild::on_removeButton_clicked()
{
}

void QProcessBuild::on_upButton_clicked()
{
}

void QProcessBuild::on_downButton_clicked()
{
}
