#include "QtCanPlatform.h"
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include "qGboleData.h"
#include <QLabel>
#include <QMessageBox>
#include <QCheckBox>
QtCanPlatform::QtCanPlatform(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    initUi();
}

QtCanPlatform::~QtCanPlatform()
{
    if (canSetting) { delete canSetting; canSetting = nullptr; }
    if (tableView) { delete tableView; tableView = nullptr; }
    if (textBrowser) { delete textBrowser; textBrowser = nullptr; }
}

void QtCanPlatform::initUi()
{
    //�������ʾ���ݵ�
    tableView = new QTableWidget();
    tableView->setColumnCount(6);
    QStringList header;
    header << tr("����")<<tr("��ַ") << tr("����") << tr("��ֹ�ֽ�") << tr("��ֹλ") << tr("����");
    tableView->setHorizontalHeaderLabels(header);
    textBrowser = new QTextBrowser();
    initData();
    QComboBox* cbSelectModel = new QComboBox();
    
    qGboleData* qGb = qGboleData::getInstance();
    if (!qGb)return;
    for (int i = 0; i < qGb->pGboleData.size(); i++)
    {
        cbSelectModel->addItem(qGb->pGboleData.at(i).modelName);
        
    }
    if (qGb->pGboleData.size() > 0)
    {
        currentModel = 0;
        cbSelectModel->setCurrentIndex(0);
    }
       
    connect(cbSelectModel, SIGNAL(currentIndexChanged(int)), this, SLOT(on_CurrentModelChanged(int)));
    //��Ӹ���ť
    QPushButton* pbAddModel = new QPushButton(tr("���Can����"));
    connect(pbAddModel, &QPushButton::clicked, this, &QtCanPlatform::qCanSettingShow);
    QPushButton* pbSend = new QPushButton(tr("����"));
    pbSend->setCheckable(true);
    connect(pbSend, SIGNAL(clicked(bool)),this,SLOT(on_pbSend_clicked(bool)));
    //��Ӹ�ˮƽ�Ĳ���
    QHBoxLayout* hLayout = new QHBoxLayout();
    //�Ѱ�ť����ȥ
    hLayout->addWidget(pbAddModel);
    hLayout->addWidget(pbSend);
   
    //�ѵ���Ҳ����ȥ
    hLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding));
    QLabel* mLabel = new QLabel();
    mLabel->setText(tr("��ǰ�ͺ�"));
    hLayout->addWidget(mLabel);
    hLayout->addWidget(cbSelectModel);
    //����һ����ֱ����
    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->addLayout(hLayout);
    vLayout->addWidget(tableView);
    vLayout->addWidget(textBrowser);
    ui.centralWidget->setLayout(vLayout);

}

void QtCanPlatform::initData()
{
    qGboleData* qGb = qGboleData::getInstance();
    if (!qGb)return;
    if(!qGb->getIsInit())
        qGb->read();
}
bool QtCanPlatform::sendDataIntoTab()
{
    if (!tableView)
        return false;
    int rcount = tableView->rowCount();
    for (int m = 0; m < rcount; m++)
        tableView->removeRow(rcount - m - 1);
    qGboleData* qGb = qGboleData::getInstance();
    if (!qGb)return false;
    if (currentModel > qGb->pGboleData.size() - 1 || currentModel< 0)
    {
        QMessageBox::warning(this, tr("warning"), tr("���ݳ�����ǰ�ͺŲ�����"));
        return false;
    }
    const protoData pTemp = qGb->pGboleData.at(currentModel);
    //canIdData cTemp;
    std::vector<canIdData>cTemp;
    for (int i = 0; i < pTemp.cItem.size(); i++)
    {
        //ȡ������Ϊ���͵��ź�
        if (1 == pTemp.cItem.at(i).opt)
        {
            cTemp.push_back(pTemp.cItem.at(i));
           // break;
        }
    }
    if (cTemp.size() <= 0)
    {
        QMessageBox::warning(this, tr("warning"), tr("���ͺ�û�з����źţ�������ٲ���"));
        return false;
    }
   
    for (int i = 0; i < cTemp.size(); i++)
    {
        int num = cTemp.at(i).pItem.size();
       
        for (int j = 0; j < num; j++)
        {
            QCheckBox* cb = new QCheckBox();
            int cr = tableView->rowCount();
            tableView->setRowCount(cr+1);
            tableView->setCellWidget(cr, 0, cb);
            QString mt = "0x"+QString("%1").arg(cTemp.at(i).CanId, QString::number(cTemp.at(i).CanId).length(), 16).toUpper().trimmed();
            tableView->setItem(cr, 1, new QTableWidgetItem(mt));
            tableView->setItem(cr, 2, new QTableWidgetItem(cTemp.at(i).pItem.at(j).bitName));
            tableView->setItem(cr, 3, new QTableWidgetItem(QString::number(cTemp.at(i).pItem.at(j).startByte)));
            tableView->setItem(cr, 4, new QTableWidgetItem(QString::number(cTemp.at(i).pItem.at(j).startBit)));
            tableView->setItem(cr, 5, new QTableWidgetItem(QString::number(cTemp.at(i).pItem.at(j).bitLeng)));
        }
   }
    return true;
}
void QtCanPlatform::on_CurrentModelChanged(int index)
{
    currentModel = index;
    sendDataIntoTab();
}
void QtCanPlatform::on_pbSend_clicked(bool clicked)
{
   
}
void QtCanPlatform::qCanSettingShow()
{
    if (!canSetting) {
        canSetting = new QCanSetting();
    }
    canSetting->show();
}