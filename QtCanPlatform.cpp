#include "QtCanPlatform.h"
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include "qGboleData.h"
#include <QLabel>
QtCanPlatform::QtCanPlatform(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    initUi();
    initData();
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
    qGb->read();
}
void QtCanPlatform::on_CurrentModelChanged(int index)
{
}
void QtCanPlatform::qCanSettingShow()
{
    if (!canSetting) {
        canSetting = new QCanSetting();
    }
    canSetting->show();
}