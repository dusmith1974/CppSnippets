#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <memory>

#include <boost/filesystem.hpp>

class QStandardItemModel;

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

    void init();

  private slots:
    void on_treeView_clicked(const QModelIndex& index);
    void on_treeView_activated(const QModelIndex& index);

    void on_actionExit_triggered();


    void on_checkBox_clicked(bool checked);

private:
    void clicked(const QModelIndex& index);

    Ui::MainWindow* m_Ui;
    QStandardItemModel* m_Model;
    std::unique_ptr<boost::filesystem::path> m_pRootPath;
};

#endif // MAINWINDOW_H
