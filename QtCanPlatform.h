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
#include <QProgressDialog.h>
#include "canthread.h"
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
    QProgressDialog* progress = nullptr;
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
private:
    void initUi();
    void initData();
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

    //��ȡ���͵�����
    void getSendDataFromTable();

    void saveAutoTestRes(const QString &fileName,const QStringList& data);

private:
    std::vector<canIdData>recCanData;
    std::vector<canIdData>sendCanData;
    //pcan�豸ָ��
    PCAN *pcan = nullptr;
    PCAN *pcanArr[4] = { nullptr };
    CANThread* canayst= nullptr ;
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
    //���pcan��ϣ�
    QComboBox* cbIsMutil = nullptr;
    //�豸��
    QPushButton* pbOpen = nullptr;
    //��������
    QLineEdit* cycle = nullptr;
    //������
    QComboBox* cbBitRate = nullptr;
    //ˢ���豸
    QPushButton* reFresh = nullptr;
    //�ͺ�ѡ��
    QComboBox* cbSelectModel = nullptr;
    //ͨ��״̬
    QLabel* communicaLabel = nullptr;
    //��ʱ��������
    QTimer* sendTimer = nullptr;
    //ͨ�Ŷ�ʧ��ʱ��
    QTimer* lostQTimer = nullptr;
    //��·��ͨ�Ŷ�ʧ��ʱ��
    QTimer* lostQTimerArr[4] = { nullptr };
    //CAN��״̬����
    bool pcanIsOpen = false;
    //��־����
    int maxTextBrowser = 0;
    //�Ƿ��ӡ����
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
    int lowVolt = 330;      //V,�����ѹ�¿�ʼ���ѹ����
    int highVolt = 740;     //V,�����ѹ�¿�ʼ���ѹ����
    int avgPower = 7000;    //ƽ������,δ�õ�
    int m_iHeatTempture = -15;                 //��C����ˮ������¶��¿�ʼ�����
    int m_iPowerTempture = 0;              //��C������¶��µĶ����
    int m_iOverTime = 300;                //�룬�������ʱ�䣬������Բ��־�Ҫ������
    int m_iVoltError = 8;                  //��ѹ���
    int m_iVoltStep = 4;                   //��ѹ����
    int rmFirstFream = 3;               //�⹦��ʱ������ָ���������ӳ�֡��
    int agvPowerFream = 60;             //�⹦��ʱ��ƽ��֡��;
    float realPower[4] = { 0 };
    float realVolt[4] = { 0 };
    float realWTemp[4] = { 0 };
    QString realHVErr[4] = { ""};
    QString realHvLv[4] = { ""};
    QString realOTPro[4] = { ""};
    QString strErrorName = "��ѹ�쳣";
    QString strOkName = "��ѹ����";
    std::vector<float>PowerArr[4];
    std::set<QString>Error[4];
    int runStep = -1;
    bool isRecordError = false;

    QTimer* t_GetVer = nullptr;         //
    QPushButton* pbGetVer = nullptr;
    QLabel labelVer1;
    QLabel labelVer2;
    QLabel labelVer3;
    QLabel labelVer4;
    //������ȡ�汾������
    uchar data_ver[8] = { 0x06,0x20,0x22,0x10,0x24,0x09,0x24,0x06 };

    //�洢�Զ������Խ��
    QString phuRes_1 = "";
    QString phuRes_2 = "";
    QString phuRes_3 = "";
    QStringList m1_listTestRes;
    QStringList m2_listTestRes;
    QStringList m3_listTestRes;
    QString m1_strPhuCode;
    QString m2_strPhuCode;
    QString m3_strPhuCode;
   
private slots:
    //CANЭ������
    void qCanSettingShow();
    //ѡ��ǰ�ͺ�
    void on_CurrentModelChanged(int index);
    //����
    void on_pbSend_clicked(bool clicked);
    //ˢ���豸
    void on_pbRefreshDevice_clicked();
    //���豸
    void on_pbOpenPcan_clicked();
    //˫���༭�����ͱ�񡱵�����
    void on_tableDoubleClicked(int,int);
    //����еĵ�Ԫ�����ݷ����仯��
    void on_tableClicked(int, int);
    //CAN ���ݽ���
    void on_ReceiveData(uint frame_id, QByteArray data);
    void on_ReceiveDataMulti(uint frame_id, QByteArray data);
    void on_ReceiveData(int ch,uint frame_id, QByteArray data);
    //CANЭ�����õĴ��ڹر���Ӧ������ˢ�����õ�����
    void on_SettingWidowsClose();
    //�����ͱ���е����������
    void on_cbSelectSendItemChanged(int);
    //������ʾ
    void on_setInToRollData();
    void on_setInToRollDataMult(int ch);
    //���ݼ��
    void on_checkTraceChanged(int);
    //��������
    void on_pbSaveCanData_clicked();
    //�������
    void on_pbClearCanData_clicked();

    void on_checkSendChanged(int);
    //���ճ�ʱ����
    void on_recTimeout();
    void on_recTimeoutMutil();
    //�Զ�������
    void on_autoWork(bool isRun);
    //�Զ��������̷߳������źŵĽ��պ���
    void on_recSigEndRunWork(int n,int channel);
    //debug�õ�step
    void on_lineEdit_editingFinished();
    //��ȡ�汾�ţ�����ר�ã�
    void on_pbGetVer_clicked(bool);

    void on_pbClearLogShow_clicked();

    void on_cbCanType_currentIndexChanged(int index);
signals:
    void sigNewRoll();
    void sigNewRollMult(int ch);
    void sigEndRunWork(int n,int channel);
public:
    void initLogger();
    void destroyLogger();

public slots:
    void logSlot(const QString& message, int level);
    
};
