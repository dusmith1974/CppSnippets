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

Q_DECLARE_METATYPE(path*)

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

    //m_pRootPath = OpenDirectory("C:\\folder1");
    m_pRootPath = OpenDirectory("/Users/smithdu/sw/boost");

    if (m_pRootPath)
    {
        PtrVecPath pPaths = FindAllPaths(m_pRootPath);

        if (pPaths)
        {
            char szLastWriteTime[20];

            m_Model->invisibleRootItem()->setData(QVariant::fromValue(m_pRootPath.get()), kDirEntryRole);
            QStandardItem* pLastDirItem = m_Model->invisibleRootItem();

            for (path& iterPath : *pPaths)
            {
                bool bRegularFile = is_regular_file(iterPath);

                time_t tm = last_write_time(iterPath);
                strftime(szLastWriteTime, sizeof(szLastWriteTime), "%Y-%m-%d %H:%M", localtime(&tm));

                QString strFilename = QString::fromStdString(iterPath.filename().string());
                QString strLastWriteTime(szLastWriteTime);
                QString strType;
                QString strFileSize;

                if (bRegularFile)
                {
                    strType = QString::fromStdString(iterPath.extension().string());

                    if (strType.isEmpty())
                    {
                        strType = "File";
                    }

                    strFileSize = HumanFileSize(file_size(iterPath));
                }

                QString strAttrib = QString("%1%2%3").arg(
                            (is_directory(iterPath)) ? "d" : "-",
                            (status(iterPath).permissions() & owner_read) ? "r" : "-",
                            (status(iterPath).permissions() & owner_write) ? "w" : "-");

                QList<QStandardItem*> items;
                items.append(new QStandardItem(strFilename));
                items.append(new QStandardItem(strLastWriteTime));
                items.append(new QStandardItem(strType));
                items.append(new QStandardItem(strAttrib));
                auto item = new QStandardItem(strFileSize);
                item->setTextAlignment(Qt::AlignRight);
                items.append(item);
                items.first()->setData(QVariant::fromValue(&iterPath), kDirEntryRole);

                bool bAddedItem = false;

                while (pLastDirItem && !bAddedItem)
                {
                    if (PathDirectlyContainsFile(*pLastDirItem->data(kDirEntryRole).value<path*>(), iterPath))
                    {
                        pLastDirItem->appendRow(items);
                        bAddedItem = true;
                    }
                    else
                    {
                        pLastDirItem = (pLastDirItem->parent()) ? pLastDirItem->parent() : m_Model->invisibleRootItem();
                    }
                }

                if (is_directory(iterPath))
                {
                    pLastDirItem = items.first();
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
    }
}

void MainWindow::on_treeView_clicked(const QModelIndex& index)
{
    QStandardItem* item = m_Model->itemFromIndex(index);
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
