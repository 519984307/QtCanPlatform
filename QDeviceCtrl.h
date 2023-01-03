#pragma once

#include <QWidget>
#include "ui_QDeviceCtrl.h"
#include <qtcpsocket.h>
#include <qcheckbox.h>
#include "QMoudBusCtrl.h"
#include <QTimer>
#include <QModbusDataUnit>
#include "PCAN.h"
#include "QProcessBuild.h"
#include "QAutoTestIniData.h"
#include "mHttp.h"
#pragma execution_character_set("utf-8")  
class QDeviceCtrl : public QWidget
{
	Q_OBJECT

public:
	QDeviceCtrl(QWidget *parent = nullptr);
	~QDeviceCtrl();

private:
	Ui::QDeviceCtrlClass ui;
	QTcpSocket* tcp = nullptr;
	PCAN* waterCan = nullptr;
	QAutoTestIniData* autoTestWidget = nullptr;
	QMoudBusCtrl* moudBus = nullptr;
	//PLC ��ַ�Ͷ˿ں�
	QString ipAddress = "192.168.200.10";
	uint16_t port = 1001;
	/*bool bInState_One[7] = {false};
	bool bInState_Two[7] = { false };
	bool bInState_Three[7] = { false };*/
	QString bInName_One[7] = {"��ͣ","��λ","�������","����","1#����-��","1#����-��","1#��ԭλ"};
	QString bInName_Two[7] = {"2#����-��","2#����-��","2#��ԭλ","NONE","3#����-��","3#����-��","3#��ԭλ"};
	QString bInName_Three[7] = {"1#�а�ť","2#�а�ť","3#�а�ť","��դ","1#ȷ�Ϲ���","2#ȷ�Ϲ���","3#ȷ�Ϲ���"};
	QCheckBox* checkOne[7] = { nullptr };
	QCheckBox* checkTwo[7] = { nullptr };
	QCheckBox* checkThree[7] = { nullptr };

	QTimer* timeSend = nullptr;
	QTimer* timeGetPower = nullptr;
	QModbusDataUnit m_mdu;
	QModbusDataUnit m_mdu2;

	QProcessBuild* qProcessB = nullptr;
	//��ˮ����ѭ��״̬��0�رգ�1����
	uint bitInCircle = 0;
	//��ˮ����ѭ��״̬��0�رգ�1����
	uint bitOutCircle = 0;
	int inCircle = 1;
	int outCircle = 1;

	bool cbProcess1Check = false;
	bool cbProcess2Check = false;
	bool cbProcess3Check = false;

	mHttp* mp = nullptr;;

	float reamHV_1 = 0;
	float reamHV_2 = 0;

	//����״̬,7����3���豸�������ˣ������ƣ�111
	//PLCΪbit[2],��ˮ��Ϊbit[1]����ѹ��ԴΪbit[0]
	unsigned short int deviceState = 0;
	//���̺߳�������ȡPLC�е�������ź�
	void getInStateRun();
	//���̺߳�������ȡ��ѹ��Դ�ĵ�ѹ��������
	void getPowerRun();
	void setOffPower();
	void getAnSetWaterRun();
public:
	unsigned short int getDeviceState() { return deviceState; }
	bool getProcess1State();
	bool getProcess2State();
	bool getProcess3State();
	float getHV_1() { return reamHV_1; }
	float getHV_2() { return reamHV_2; }
	void setWorkButton(int n);

	void setHV_1(int hv, float current);
	void setHV_2(int hv, float current);

	bool bInState_One[7] = { false };
	bool bInState_Two[7] = { false };
	bool bInState_Three[7] = { false };
	void closeSomething();
	bool outCycleState() { return outCircle; }
	bool inCycleState() { return inCircle; }
	void setResInLabel(int ch, QString str, QColor color);
	QString getPhuCode(int ch);
signals:
	void timeToSend(QString str,int num);
	void sigArealdSend(QModbusDataUnit mdu);
	void sigWorkRun(bool isCheck);
	void sigCanChanged();
public slots:

	//1#��λ
	void on_pbGrasp_clicked(bool isClicked);
	void on_pbUpDown_clicked(bool isClicked);
	void on_pbCover_clicked(bool isClicked);
	void on_pbColdWater_clicked(bool isClicked);
	void on_pbPureWater_clicked(bool isClicked);
	void on_pbBlowWater_clicked(bool isClicked);

	//2#��λ
	void on_pbGrasp_2_clicked(bool isClicked);
	void on_pbUpDown_2_clicked(bool isClicked);
	void on_pbCover_2_clicked(bool isClicked);
	void on_pbColdWater_2_clicked(bool isClicked);
	void on_pbPureWater_2_clicked(bool isClicked);
	void on_pbBlowWater_2_clicked(bool isClicked);

	//3#��λ
	void on_pbGrasp_3_clicked(bool isClicked);
	void on_pbUpDown_3_clicked(bool isClicked);
	void on_pbCover_3_clicked(bool isClicked);
	void on_pbColdWater_3_clicked(bool isClicked);
	void on_pbPureWater_3_clicked(bool isClicked);
	void on_pbBlowWater_3_clicked(bool isClicked);
	void on_SocketStateChanged(QAbstractSocket::SocketState);
	void on_pbTcpConnect_clicked(bool isClicked);

	void _readyRead();
	void on_timeToSend(QString str,int num);

	//���õ�����ѹ
	void on_pbSetVoltAndCurr_clicked();
	void on_delaySend();
	//���Ӵ���
	void on_pbConnectRTU_clicked(bool isCheck);

	//����spinBox��������ѹ
	void on_spinBoxVolt_editingFinished();
	void on_spinBoxCurrent_editingFinished();
	//�¸�ѹ
	void on_pbOffVolt_clicked();
	//ˢ�´���
	void on_pbRefresh_clicked();

	//������ˮ������
	void onReceiveData(unsigned int fream_id, QByteArray data);
	void on_pbStartInCricle_clicked(bool isCheck);
	void on_pbStartOutCricle_clicked(bool isCheck);
	void on_pbBlowWater_4_clicked(bool isCheck);

	//
	void on_dOpenCan_clicked(bool isCheck);
	void on_dCanRefresh_clicked();

	//����������
	void on_pbProcessSet_clicked(bool isCheck);
	void on_setProcessSetState(bool b);

	//��λ��ѡ��
	void on_dCbProcess1_stateChanged(int state);
	void on_dCbProcess2_stateChanged(int state);
	void on_dCbProcess3_stateChanged(int state);

	void on_linedSetp_editingFinished();
	void on_pbTestParamSet_clicked();
private slots:
	void on_lineEdit_1ResCode_editingFinished();
	void on_lineEdit_2ResCode_editingFinished();
	void on_lineEdit_3ResCode_editingFinished();
public slots:
	void on_sendMdu();
	
};
