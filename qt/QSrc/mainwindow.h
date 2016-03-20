#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

  private:
    void clicked(const QModelIndex& index);

    Ui::MainWindow* m_Ui;
    QStandardItemModel* m_Model;
};

#endif // MAINWINDOW_H
