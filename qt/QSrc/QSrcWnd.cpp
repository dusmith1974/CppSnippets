#include "QSrcWnd.h"
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

enum ColumnNames
{
    kName = 0,
    kDate,
    kType,
    kAttr,
    kSize,
    kStub
};

Q_DECLARE_METATYPE(path*)

QSrcWnd::QSrcWnd(QWidget* parent) :
    QMainWindow(parent),
    m_Ui(new Ui::MainWindow),
    m_Model(nullptr),
    m_pRootPath(nullptr)
{
    m_Ui->setupUi(this);
}

QSrcWnd::~QSrcWnd()
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

void GetFileInfo(const path& iterPath, QString* pstrFileName, QString* pstrLastWriteTime, QString* pstrType, QString* pstrFileSize, QString* pstrAttrib)
{
    char szLastWriteTime[20];

    time_t tm = last_write_time(iterPath);
    strftime(szLastWriteTime, sizeof(szLastWriteTime), "%Y-%m-%d %H:%M", localtime(&tm));

    *pstrFileName = QString::fromStdString(iterPath.filename().string());
    *pstrLastWriteTime = szLastWriteTime;

    if (is_regular_file(iterPath))
    {
        *pstrType = QString::fromStdString(iterPath.extension().string());
        pstrType->replace(QString("."), QString(""));

        if (pstrType->isEmpty() || pstrFileName->startsWith("."))
        {
            *pstrType = "File";
        }

        *pstrFileSize = HumanFileSize(file_size(iterPath));
    }

    *pstrAttrib = QString("%1%2%3").arg((is_directory(iterPath)) ? "d" : "-",
                                        (status(iterPath).permissions() & owner_read) ? "r" : "-",
                                        (status(iterPath).permissions() & owner_write) ? "w" : "-");
}

void GetFileItems(path& path, QList<QStandardItem*>* items, QString* pstrFileName, QString* pstrLastWriteTime, QString* pstrType, QString* pstrFileSize, QString* pstrAttrib)
{
    items->append(new QStandardItem(*pstrFileName));
    items->append(new QStandardItem(*pstrLastWriteTime));
    items->append(new QStandardItem(*pstrType));
    items->append(new QStandardItem(*pstrAttrib));
    items->append(new QStandardItem(*pstrFileSize));
    items->append(new QStandardItem(QString()));

    for (auto& item : *items)
    {
        item->setEditable(false);
    }

    (*items)[kSize]->setTextAlignment(Qt::AlignRight);
    items->first()->setData(QVariant::fromValue(&path), kDirEntryRole);
}

void AddFileRow(QStandardItem* root, const path& filePath, const QList<QStandardItem*>& items, QStandardItem** pLastDirItem)
{
    bool bAddedItem = false;

    while (*pLastDirItem && !bAddedItem)
    {
        if (PathDirectlyContainsFile(*(*pLastDirItem)->data(kDirEntryRole).value<path*>(), filePath))
        {
            (*pLastDirItem)->appendRow(items);
            bAddedItem = true;
        }
        else
        {
            *pLastDirItem = ((*pLastDirItem)->parent()) ? (*pLastDirItem)->parent() : root;
        }
    }

    if (is_directory(filePath))
    {
        *pLastDirItem = items.first();
    }
}
}  // namespace

void QSrcWnd::init()
{
    InitModel();

    m_pRootPath = OpenDirectory("/Users/smithdu/test");

    if (m_pRootPath)
    {
        QStandardItem* pLastDirItem = m_Model->invisibleRootItem();
        pLastDirItem->setData(QVariant::fromValue(m_pRootPath.get()), kDirEntryRole);

        PtrVecPath pPaths = FindAllPaths(m_pRootPath);

        if (pPaths)
        {
            for (path& path : *pPaths)
            {
                QString strFileName, strLastWriteTime, strType, strFileSize, strAttrib;
                GetFileInfo(path, &strFileName, &strLastWriteTime, &strType, &strFileSize, &strAttrib);

                QList<QStandardItem*> items;
                GetFileItems(path, &items, &strFileName, &strLastWriteTime, &strType, &strFileSize, &strAttrib);

                AddFileRow(m_Model->invisibleRootItem(), path, items, &pLastDirItem);
            }

            InitTreeView();
        }
    }
}

void QSrcWnd::InitModel()
{
    QStringList headers;
    headers << "Name" << "Date modified" << "Type" << "Attr" << "Size" << "";

    m_Model = new QStandardItemModel;
    m_Model->setColumnCount(headers.size());
    m_Model->setHorizontalHeaderLabels(headers);
}

void QSrcWnd::InitTreeView()
{
    m_Ui->treeView->setAlternatingRowColors(true);
    m_Ui->treeView->setAnimated(true);
    m_Ui->treeView->sortByColumn(kName, Qt::AscendingOrder);
    m_Ui->treeView->setSortingEnabled(true);
    m_Ui->treeView->setModel(m_Model);

    m_Ui->treeView->header()->setSectionResizeMode(kName, QHeaderView::ResizeToContents);
    m_Ui->treeView->header()->setSectionResizeMode(kDate, QHeaderView::ResizeToContents);
}

void QSrcWnd::on_treeView_clicked(const QModelIndex& index)
{
    QStandardItem* item = m_Model->itemFromIndex(index);
}

void QSrcWnd::on_treeView_activated(const QModelIndex& index)
{
}

void QSrcWnd::on_actionExit_triggered()
{
    qApp->exit();
}

void QSrcWnd::on_checkBox_clicked(bool checked)
{
}
