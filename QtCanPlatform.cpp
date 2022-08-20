#include "QtCanPlatform.h"
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include "qGboleData.h"
#include <QLabel>
#include <QMessageBox>
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
    tableView = new QTableView();
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
    QPushButton* pbAddModel = new QPushButton("���Can����");
    connect(pbAddModel, &QPushButton::clicked, this, &QtCanPlatform::qCanSettingShow);
    //��Ӹ�ˮƽ�Ĳ���
    QHBoxLayout* hLayout = new QHBoxLayout();
    //�Ѱ�ť����ȥ
    hLayout->addWidget(pbAddModel);
   
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
    bool res = false;
    for (int i = 0; i < pTemp.cItem.size(); i++)
    {
        //ȡ������Ϊ���͵��ź�
        if (1 == pTemp.cItem.at(i).opt)
        {
            cTemp.push_back(pTemp.cItem.at(i));
            break;
        }
    }
    if (cTemp.size() <= 0)
    {
        QMessageBox::warning(this, tr("warning"), tr("���ͺ�û�з����źţ�������ٲ���"));
        return false;
    }
    return true;
}
void QtCanPlatform::on_CurrentModelChanged(int index)
{
    currentModel = index;
    
}
void QtCanPlatform::qCanSettingShow()
{
    if (!canSetting) {
        canSetting = new QCanSetting();
    }
    canSetting->show();
}