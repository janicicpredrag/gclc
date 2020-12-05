#ifndef WATCHWINDOW_H
#define WATCHWINDOW_H

#include <QDialog>

namespace Ui {
class WatchWindow;
}

class WatchWindow : public QDialog
{
    friend class Watch;

    Q_OBJECT

public:
    explicit WatchWindow(QWidget *parent = 0);
    ~WatchWindow();

    void showValues();
    QString getObjectName(int row, int column);
    void setObjectValue(int row, int column, QString s);

private slots:
    void on_tableWidget_cellEntered(int row, int column);
    void on_tableWidget_cellChanged(int row, int column);

signals:
    void changedcell (int row, int column);

private:
    Ui::WatchWindow *ui;

};

#endif // WATCHWINDOW_H
