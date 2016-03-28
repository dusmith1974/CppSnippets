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
    kDirEntryRole = Qt::UserRole + 1
};

Q_DECLARE_METATYPE(directory_entry*)
Q_DECLARE_METATYPE(const directory_entry*)

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    m_Ui(new Ui::MainWindow),
    m_Model(nullptr),
    m_pRootPath(nullptr)
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

    return QString().setNum(num, 'f', 0) + " " + unit;
}

namespace
{
bool PathContainsFile(path dir, path file)
{
    if (dir.filename() == ".")
    {
        dir.remove_filename();
    }

    file.remove_filename();

    auto dir_len = std::distance(dir.begin(), dir.end());
    auto file_len = std::distance(file.begin(), file.end());

    if (dir_len > file_len)
    {
        return false;
    }

    return std::equal(dir.begin(), dir.end(), file.begin());
}

bool PathDirectlyContainsFile(path dir, path file)
{
    if (dir.filename() == ".")
    {
        dir.remove_filename();
    }

    file.remove_filename();

    return dir == file;
}
}  // namespace

void MainWindow::init()
{
    QStringList headers;
    headers << "Name" << "Date modified" << "Type" << "Attr" << "Size" << "";

    m_Model = new QStandardItemModel;
    m_Model->setColumnCount(headers.size());
    m_Model->setHorizontalHeaderLabels(headers);

    QStandardItem* pParentDirItem = m_Model->invisibleRootItem();
    //m_pRootPath = OpenDirectory("C:\\folder1");
    m_pRootPath = OpenDirectory(".");

    if (m_pRootPath)
    {
        PtrVecEntry pEntries = FindAllEntries(m_pRootPath);

        if (pEntries)
        {
            char szLastWriteTime[20];

            QStandardItem* pPrevItem = nullptr;

            for (const directory_entry& entry : *pEntries)
            {
                bool bRegularFile = is_regular_file(entry.path());

                time_t tm = last_write_time(entry.path());
                strftime(szLastWriteTime, sizeof(szLastWriteTime), "%Y-%m-%d %H:%M", localtime(&tm));

                QString strFilename = QString::fromStdString(entry.path().filename().string());
                QString strLastWriteTime(szLastWriteTime);
                QString strType;
                QString strFileSize;

                if (bRegularFile)
                {
                    strType = QString::fromStdString(entry.path().extension().string());

                    if (strType.isEmpty())
                    {
                        strType = "File";
                    }

                    strFileSize = HumanFileSize(file_size(entry.path()));
                }

                QString strAttrib = QString("%1%2%3").arg(
                                        (is_directory(entry.path())) ? "d" : "-",
                                        (entry.status().permissions() & owner_read) ? "r" : "-",
                                        (entry.status().permissions() & owner_write) ? "w" : "-");

                QList<QStandardItem*> items;
                items.append(new QStandardItem(strFilename));
                items.append(new QStandardItem(strLastWriteTime));
                items.append(new QStandardItem(strType));
                items.append(new QStandardItem(strAttrib));

                auto item = new QStandardItem(strFileSize);
                item->setTextAlignment(Qt::AlignRight);
                items.append(item);

                if (!pPrevItem)
                {
                    pParentDirItem->appendRow(items);
                    pParentDirItem->setData(QVariant::fromValue(&entry), kDirEntryRole);
                    directory_entry* pp = pParentDirItem->data(kDirEntryRole).value<directory_entry*>();
                    (void)pp;
                }
                else
                {
                    directory_entry* pPrevDirEntry = pPrevItem->data(kDirEntryRole).value<directory_entry*>();

                    pParentDirItem->appendRow(items);
                    pParentDirItem->setData(QVariant::fromValue(&entry), kDirEntryRole);
                    if (PathDirectlyContainsFile(pPrevDirEntry->path(), entry.path()))
                    {
                        //pParentDirItem = items.first();
                    }
                }

                if (is_directory(entry.path()))
                {
                    pPrevItem = pParentDirItem;
                }
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

    m_Ui->treeView->setAlternatingRowColors(true);
    m_Ui->treeView->setAnimated(true);
    //m_Ui->treeView->setSortingEnabled(true);

    m_Ui->treeView->setContextMenuPolicy(Qt::DefaultContextMenu);
    //m_Ui->treeView->setContextMenuPolicy(Qt::ActionsContextMenu);
    //m_Ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    //m_Ui->treeView->sortByColumn(0, Qt::AscendingOrder);

    m_Ui->treeView->setModel(m_Model);
}

void MainWindow::on_treeView_clicked(const QModelIndex& index)
{
    QStandardItem* item = m_Model->itemFromIndex(index);
    //FileSystemInfo* p = index.data(kFileSystemInfoRole).value<FileSystemInfo*>();
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
