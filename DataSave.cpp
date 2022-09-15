#include "DataSave.h"
#include <windows.h>
#include <QFileDialog>
DataSave::DataSave(QObject *parent)
	: QThread(parent)
{}

DataSave::~DataSave()
{
    isRun = false;
    msleep(1000);
}

void DataSave::SaveData(QStringList list, int rows, QString path)
{
    dataList = list;
    this->rows = rows;
    this->filepath = path;
    this->start();
}

bool DataSave::setTitle(QString title)
{
    datalist_name = title;
    return true;
}

void DataSave::run()
{
    if (dataList.count() > 0)
    {
        if (!filepath.isEmpty()) {
            HRESULT re = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
            QAxObject* excel = new QAxObject(this);
            //����Excel�ؼ�
            excel->setControl("Excel.Application");
            //����ʾ����
            excel->dynamicCall("SetVisible (bool Visible)", "false");
            //����ʾ�κξ�����Ϣ�����Ϊtrue��ô�ڹر��ǻ�������ơ��ļ����޸ģ��Ƿ񱣴桱����ʾ
            excel->setProperty("DisplayAlerts", false);
            //��ȡ����������
            QAxObject* workbooks = excel->querySubObject("WorkBooks");
            //�½�һ��������
            workbooks->dynamicCall("Add");
            //��ȡ��ǰ������
            QAxObject* workbook = excel->querySubObject("ActiveWorkBook");
            //��ȡ��������
            QAxObject* worksheets = workbook->querySubObject("Sheets");
            //��ȡ�������ϵĹ�����1����sheet1
            QAxObject* worksheet = worksheets->querySubObject("Item(int)", 1);




            //��ͷ
            QString str = datalist_name;
            QStringList strlist = str.split(",");


            strlist.removeAll("");
            int listCount = strlist.count();
            QString cellCol;
            for (int j = 0; j < listCount; j++)
            {
                if (j > 25)
                    cellCol = "A" + QString('A' + j - 26) + QString::number(1);          //excel��񣬵�27����AA��
                else if (j <= 25)
                    cellCol = QString('A' + j) + QString::number(1);
                else
                    return;


                QAxObject* cellRow = worksheet->querySubObject("Range(QVariant, QVariant)", cellCol);
                cellRow->dynamicCall("SetValue(const QVariant&)", QVariant(strlist[j]));


                QAxObject* font = cellRow->querySubObject("Font");  //��ȡ��Ԫ������
                font->setProperty("Bold", true);  //���õ�Ԫ������Ӵ�
                if (!isRun)
                    return;
            }




            //���ñ������
            if (dataList.count() > 0)
                for (int i = 0; i < rows; i++) {
                    QString str = dataList.at(i);
                    QStringList strlist = str.split(",");
                    //            QString str = model->data(model->index(i),Qt::DisplayRole).toString();
                    //            QStringList strlist = str.split(" ");


                    strlist.removeAll("");
                    for (int j = 0; j < strlist.count(); j++)
                    {
                        //QString cellCol = QString('A'+j)+QString::number(i+2);
                        if (j > 25)
                            cellCol = "A" + QString('A' + j - 26) + QString::number(i + 2);    //excel��񣬵�27����AA��
                        else if (j <= 25)
                            cellCol = QString('A' + j) + QString::number(i + 2);
                        else
                            return;
                        QAxObject* cellRow = worksheet->querySubObject("Range(QVariant, QVariant)", cellCol);
                        cellRow->dynamicCall("SetValue(const QVariant&)", QVariant(strlist[j]));
                        if (!isRun)
                        {
                            workbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(filepath));//������filepath��ע��һ��Ҫ��QDir::toNativeSeparators��·���е�"/"ת��Ϊ"\"����Ȼһ�����治�ˡ�
                            workbook->dynamicCall("Close()");//�رչ�����
                            excel->dynamicCall("Quit()");//�ر�excel
                            delete excel;
                            excel = NULL;
                            return;
                        }
                            
                    }
                }


            workbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(filepath));//������filepath��ע��һ��Ҫ��QDir::toNativeSeparators��·���е�"/"ת��Ϊ"\"����Ȼһ�����治�ˡ�
            workbook->dynamicCall("Close()");//�رչ�����
            excel->dynamicCall("Quit()");//�ر�excel
            delete excel;
            excel = NULL;
        }
    }
    dataList.clear();
    rows = 0;
}
