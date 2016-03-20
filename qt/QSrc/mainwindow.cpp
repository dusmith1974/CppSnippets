#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QStandardItemModel>
#include <QTreeView>

#include "filesystem.h"

enum Roles
{
    kFileSystemInfoRole = Qt::UserRole + 1
};

struct FileSystemInfo
{
    int mode;
    QString name;
};

Q_DECLARE_METATYPE(FileSystemInfo*)

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    m_Ui(new Ui::MainWindow)
{
    m_Ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete m_Ui;
    delete m_Model;
}

void MainWindow::init()
{
    m_Model = new QStandardItemModel;
    m_Model->setColumnCount(3);
    QStandardItem* parentItem = m_Model->invisibleRootItem();
    PtrPath pPath = OpenDirectory(".");

    if (pPath)
    {
        PtrVecEntry pEntries = FindAllEntries(pPath);

        if (pEntries)
        {
            for (const auto& entry : *pEntries)
            {
                QList<QStandardItem*> items;
                items.append(new QStandardItem(QString::fromStdString(entry.path().string())));
                parentItem->appendRow(items);
            }

//            for (int i = 0; i < 4; ++i)
//            {
//                QStandardItem* item = new QStandardItem(QString("item %0").arg(i));
//                QStandardItem* item2 = new QStandardItem(QString("ReadWrite"));
//                QStandardItem* item3 = new QStandardItem(QString("Filename"));
//                QList<QStandardItem*> items;
//                items.append(item);
//                items.append(item2);
//                items.append(item3);
//                FileSystemInfo* p = new FileSystemInfo;
//                p->mode = i;
//                p->name = "A File";
//                item->setData(QVariant::fromValue(p), kFileSystemInfoRole);
//                parentItem->appendRow(items);
//                parentItem = item;
//            }
        }
    }

    m_Ui->treeView->setModel(m_Model);
}

void MainWindow::on_treeView_clicked(const QModelIndex& index)
{
    QStandardItem* item = m_Model->itemFromIndex(index);
    FileSystemInfo* p = index.data(kFileSystemInfoRole).value<FileSystemInfo*>();
}

void MainWindow::on_treeView_activated(const QModelIndex& index)
{
}
