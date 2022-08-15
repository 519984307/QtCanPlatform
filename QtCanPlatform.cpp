#include "QtCanPlatform.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHBoxLayout>
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
    //��Ӹ���ť
    QPushButton* pbAddModel = new QPushButton("���Can����");
    connect(pbAddModel, &QPushButton::clicked, this, &QtCanPlatform::qCanSettingShow);
    //��Ӹ�ˮƽ�Ĳ���
    QHBoxLayout* hLayout = new QHBoxLayout();
    //�Ѱ�ť����ȥ
    hLayout->addWidget(pbAddModel);
    //�ѵ���Ҳ����ȥ
    hLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding));
    //����һ����ֱ����
    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->addLayout(hLayout);
    vLayout->addWidget(tableView);
    vLayout->addWidget(textBrowser);
    ui.centralWidget->setLayout(vLayout);

}

void QtCanPlatform::initData()
{
    
}
void QtCanPlatform::qCanSettingShow()
{
    if (!canSetting) {
        canSetting = new QCanSetting();
    }
    canSetting->show();
}