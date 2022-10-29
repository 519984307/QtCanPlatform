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
#pragma execution_character_set("utf-8")  
#include <QWidget>
#include "ui_QSetProperty.h"
#include <QTableWidget>
#include "dataStruct.h"
#include <QCloseEvent>

class QSetProperty : public QWidget
{
	Q_OBJECT

public:
	QSetProperty(QWidget *parent = nullptr);
	~QSetProperty();
	//���洫������map��������������
	void setIntoMap(std::map<QString, cellProperty>* cp); 
	void setIntoMap(std::vector<cellProperty>* cp);
	//���ڹر��¼���������ձ������ݣ������´δ򿪴���ʱ����֮ǰ������
	virtual void closeEvent(QCloseEvent* event) override;
private:
	Ui::QSetPropertyClass ui;
	void InitUI();
	void ItitData();
	QTableWidget* table=nullptr;
	//std::map<QString, cellProperty>*mcp;
	//ָ�봫�ݣ������޸ĸ����ڵ����ԣ������ⲿ���ñ���ʱ�������ȵ���setIntoMap��ָ�봫����
	std::vector<cellProperty>* stl_mcp;
private slots:
	//�ر�QT��ɫ���֮��ķ�����������ȡ�û�ѡȡ����ɫ
	void getColor();
	//������Բۺ���
	void on_pbAddItem_clicked();
	//ɾ�����Բۺ���
	void on_pbDelItem_clicked();
	void on_pbSaveItem_clicked();
	//˫����Ԫ��Ĳۺ���
	void on_itemDoubleClicked(int,int);
	//��Ԫ���޸ĺ�Ĳۺ���
	void on_table_cellChanged(int,int);
	void on_check_clicked(int);
};
