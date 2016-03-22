#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <boost/date_time/posix_time/posix_time.hpp>

#include <QDebug>
#include <QStandardItemModel>
#include <QTreeView>
#include <QLayout>

#include "filesystem.h"

using namespace std;

using namespace boost::filesystem;
using namespace boost::posix_time;

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
    //QWidget* window = new QWidget;
    //QHBoxLayout* layout = new QHBoxLayout;
    //layout->addWidget(m_Ui->treeView);
    //window->setLayout(layout);
    //window->show();
    //m_Ui->MainWindow
}

MainWindow::~MainWindow()
{
    delete m_Ui;
    delete m_Model;
}

QString HumanFileSize(uintmax_t iFileSize)
{
    float num = iFileSize;
    QStringList list;
    list << "KB" << "MB" << "GB" << "TB";

    QStringListIterator iter(list);
    QString unit("bytes");

    while (num >= 1024.0 && iter.hasNext())
    {
        unit = iter.next();
        num /= 1024.0;
    }

    return QString().setNum(num, 'f', 2) + " " + unit;
}

void MainWindow::init()
{
    QStringList headers;
    headers << "Name" << "Date modified" << "Type" << "Size" << "";

    m_Model = new QStandardItemModel;
    m_Model->setColumnCount(headers.size());
    m_Model->setHorizontalHeaderLabels(headers);

    QStandardItem* parentItem = m_Model->invisibleRootItem();
    PtrPath pPath = OpenDirectory(".");

    if (pPath)
    {
        PtrVecEntry pEntries = FindAllEntries(pPath);

        if (pEntries)
        {
            char szLastWriteTime[20];

            for (const directory_entry& entry : *pEntries)
            {
                bool bRegular = is_regular_file(entry.path());

                time_t tm = last_write_time(entry.path());
                strftime(szLastWriteTime, sizeof(szLastWriteTime), "%Y-%m-%d %H:%M", localtime(&tm));

                QString strFilename = QString::fromStdString(entry.path().filename().string());
                QString strLastWriteTime(szLastWriteTime);
                QString strType;

                if (bRegular)
                {
                    strType = QString::fromStdString(entry.path().extension().string());

                    if (strType.isEmpty())
                    {
                        strType = "File";
                    }
                }
                else
                {
                    if (is_directory(entry.path()))
                    {
                        strType = "File folder";
                    }
                }

                QString strFileSize;

                if (bRegular)
                {
                    strFileSize = HumanFileSize(file_size(entry.path()));
                }

                QList<QStandardItem*> items;
                items.append(new QStandardItem(strFilename));
                items.append(new QStandardItem(strLastWriteTime));
                items.append(new QStandardItem(strType));

                auto item = new QStandardItem(strFileSize);
                item->setTextAlignment(Qt::AlignRight);
                items.append(item);

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

void MainWindow::on_actionExit_triggered()
{
    qApp->exit();
}

void MainWindow::on_checkBox_clicked(bool checked)
{

}
