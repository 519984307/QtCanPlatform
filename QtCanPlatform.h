/*
 * QtCanPlatform.h - header file of QtCanPlatform class
 *
 * Copyright (C) 2021-2022 lyb <liyangbinbin@foxmail.com>
 *
 * This file is part of the QtCanPlatform.
 *
 * QtCanPlatform is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * QtCanPlatform is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libQtShadowsocks; see the file LICENSE. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtCanPlatform.h"
#include <QTableView>
#include <qtextbrowser.h>
#include <iostream>
#include <memory>
#include <QStringList>
#include <QStringListModel>
#include "QCanSetting.h"
#include "PCAN.h"
#include <QTimer>
#include <QPushButton>
#include <QCloseEvent>
#include "DataSave.h"
#include "QDeviceCtrl.h"
#include "kvaser.h"
#include <QtConcurrent>
#include <set>
#pragma execution_character_set("utf-8")  
class QtCanPlatform : public QMainWindow
{
    Q_OBJECT

public:
    QtCanPlatform(QWidget *parent = nullptr);
    ~QtCanPlatform();
    virtual void closeEvent(QCloseEvent* event) override;
private:
    Ui::QtCanPlatformClass ui;
    void initUi();
    void initData();
    QStringListModel *model=nullptr;

    //UI
    //��ʾ����UI�ı��
    QTableWidget* tableView=nullptr;
    //��ʾ��������UI�ı��
    QTableWidget* tableRecView=nullptr;
    //��ͨ������Ҫ�ĸ���ʾtablewidget
    QTableWidget* tableArray[4];
    QTableWidget* tableRollTitle=nullptr;
    QTableWidget* tableRollData=nullptr;
    QTableWidget* tableRollTitleArray[4];
    QTableWidget* tableRollDataArray[4];
    QTabWidget* tabRollBox = nullptr;
    QTextBrowser* textBrowser=nullptr;
    QCanSetting* canSetting = nullptr;
    //��ʾ���յ������ݵ�tab����
    QTabWidget* tabRecBox = nullptr;
    //��������4��QTableWidget
    QWidget* w[4];
    QWidget* w_rolling[4];
    //control ui
    QDeviceCtrl* dCtrl = nullptr;

    std::atomic_bool _bWork = false;
    std::atomic_int _iSetp = 0;

    int currentModel = -1;
    bool sendDataIntoTab();
    bool recDataIntoTab();
    void sendData();
    //���ݽ���
    bool intelProtocol(canIdData &cdata,uchar data[], unsigned int &fream_id);
    bool motoProtocol(canIdData& cdata,uchar data[], unsigned int& fream_id);

    //���յ������ݽ���(��ͨ��)
    void recAnalyseIntel(unsigned int fream_id, QByteArray data);
    void recAnalyseMoto(unsigned int fream_id, QByteArray data);
    //kvaser��ͨ��
    void recAnalyseIntel(int ch,unsigned int fream_id, QByteArray data);
    void recAnalyseMoto(int ch,unsigned int fream_id, QByteArray data);

    //δʹ��
    void getModelTitle();
    //���ݱ���
    void saveCanData();
    void saveCanDataMult();
    //CRC����
    unsigned char crc_high_first(uchar data[], unsigned char len);

    //��ȡ�����ļ� 
    void readSetFile();
    //����
    void configSetFile();
    //�Զ����Թ������̺���
    void workRun();

private:
    std::vector<canIdData>recCanData;
    std::vector<canIdData>sendCanData;
    //pcan�豸ָ��
    PCAN *pcan = nullptr;
    //kvaser�豸ָ��
    kvaser* kcan = nullptr;
    //����kcan�Ĵ�״̬
    const int* ckHandle = nullptr;
    //���Ͱ�ť
    QPushButton* pbSend = nullptr;
    //canͨ��ѡ��
    QComboBox* cbPcan = nullptr;
    //can����ѡ��
    QComboBox* cbCanType = nullptr;
    QPushButton* pbOpen = nullptr;
    QLineEdit* cycle = nullptr;
    QComboBox* cbBitRate = nullptr;
    QPushButton* reFresh = nullptr;
    QComboBox* cbSelectModel = nullptr;
    QLabel* communicaLabel = nullptr;
    QTimer* sendTimer = nullptr;
    QTimer* lostQTimer = nullptr;
    bool pcanIsOpen = false;
    int maxTextBrowser = 0;
    bool isTrace = false;
    std::map<QString, std::vector<parseData>>showTableD;
    QStringList rollTitle;
    QStringListModel* titleModel = nullptr;
    std::vector<RollStruct>RollShowData;
    DataSave *saveData=nullptr;
    DataSave* saveDataArr[4] = { nullptr };
    //����ͨ��ʱ�ı������
    QStringList strSaveList;
    //��ͨ��ʱ�����ݽṹ
    std::map<unsigned int, QStringList>multReceData;
    //����excel�ı�ͷ
    QString excelTitle;
    //�Զ�������������������ﵽ��������ͻ��Զ�����
    int saveListNum = 600;
    uint16_t lostTimeOut = 3000;
    QColor recBackgroudColor = QColor(0, 120, 215);
    QColor recFontColor = QColor(255, 250, 255);
    QDateTime lastTime;

    //�Զ��жϵ��������ж�ֵ
    int lowVolt = 350;      //��ѹ
    int highVolt = 750;     //��ѹ
    int avgPower = 7000;    //ƽ������
    int shortIndex = 7;     //��·�ڱ������ݵ��±�
    QString voltId = "0x0C004400";      //��ѹ�����ĸ�����
    QString powerId = "0x0C004400";     //���������ĸ�����
    QString shortId = "0x0C004400";     //��·�����ĸ�����
    int tempture = -15;                 //��ˮ���¶�
    int rmFirstFream = 3;               //�⹦��ʱ������ָ���������ӳ�֡��
    int agvPowerFream = 60;             //�⹦��ʱ��ƽ��֡��;
    float realPower[4] = { 0 };
    float realVolt[4] = { 0 };
    float realWTemp[4] = { 0 };
    QString realHvLv[4] = { ""};
    std::vector<float>PowerArr;
    std::set<QString>Error[4];

    bool isRecordError = false;

   
private slots:
    void qCanSettingShow();
    void on_CurrentModelChanged(int index);
    void on_pbSend_clicked(bool clicked);
    void on_pbRefreshDevice_clicked();
    void on_pbOpenPcan_clicked();
    void on_tableDoubleClicked(int,int);
    void on_tableClicked(int, int);
    void on_ReceiveData(uint frame_id, QByteArray data);
    void on_ReceiveData(int ch,uint frame_id, QByteArray data);
    void on_SettingWidowsClose();
    void on_cbSelectSendItemChanged(int);
    void on_setInToRollData();
    void on_setInToRollDataMult(int ch);
    void on_checkTraceChanged(int);
    void on_pbSaveCanData_clicked();
    void on_pbClearCanData_clicked();
    void on_checkSendChanged(int);
    void on_recTimeout();
    void on_autoWork(bool isRun);
    void on_recSigEndRunWork(int n);
signals:
    void sigNewRoll();
    void sigNewRollMult(int ch);
    void sigEndRunWork(int n);
public:
    void initLogger();
    void destroyLogger();

public slots:
    void logSlot(const QString& message, int level);
    
};
