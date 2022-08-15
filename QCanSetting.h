#pragma once

#include <QWidget>
#include "ui_QCanSetting.h"
#include <QTableView>
#include <qtablewidget.h>
#include <QStandardItemModel>
#pragma execution_character_set("utf-8")  
struct protoItem {
	int startByte;
	int startBit;
	int bitLeng;
	int precision;
	int offset;
	QString bitName;
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
class QCanSetting : public QWidget
{
	Q_OBJECT

public:
	QCanSetting(QWidget *parent = nullptr);
	~QCanSetting();

private:
	Ui::QCanSettingClass ui;
	void InitUi();
	QTableWidget* tableView = nullptr;
	//�ͺ�table
	QTableWidget* modelView = nullptr;
	//can id table
	QTableWidget* canIdView = nullptr;
	std::vector<struct protoData>pGboleData;
	void savepGboleData();
	void InitpGboleData();
	void SetTableData();
private slots:
	//����ͺ�
	void on_pbAddModel_clicked();
	//����
	void on_pbMoveUpModel_clicked();
	//����
	void on_pbMoveDownModel_clicked();
	//ɾ��
	void on_pbDelModel_clicked();
	//����
	void on_pbSaveModel_clicked();
	
	//���CanId
	void on_pbAddCanId_clicked();
	void on_pbMoveUpCanId_clicked();
	void on_pbMoveDownCanId_clicked();
	void on_pbDelCanId_clicked();
	void on_pbSaveCanId_clicked();

	//��ӽ�������
	void on_pbAddIteam_clicked();
	void on_pbMoveUpIteam_clicked();
	void on_pbMoveDownIteam_clicked();
	void on_pbDelIteam_clicked();
	void on_pbSaveIteam_clicked();

	//�����ͺű��ť��Ӧ�ۺ���
	void on_modelView_currentIndexChanged(int index);
	void on_modelView_cellChanged(int row, int col);
	void on_modelView_doubleClicked(int, int);
	void on_modelView_Clicked(int, int);

	//������CanID�����Ӧ�ۺ���
	void on_canIdView_currentIndexChanged(int index);
	void on_canIdView_cellChanged(int row, int col);
	void on_canIdView_doubleClicked(int, int);
	void on_canIdView_Clicked(int, int);
};
