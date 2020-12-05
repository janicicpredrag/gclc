#ifndef EXPORTTORASTERFORMAT_H
#define EXPORTTORASTERFORMAT_H

#include <QDialog>

namespace Ui {
class ExportToRasterFormat;
}

class ExportToRasterFormat : public QDialog
{
    Q_OBJECT
    
public:
    explicit ExportToRasterFormat(QWidget *parent = 0);
    ~ExportToRasterFormat();
    
    bool currentFrameOnly();
    int rangeStartFrame();
    int rangeEndFrame();
    int getResolution();

private:
    Ui::ExportToRasterFormat *ui;
};

#endif // EXPORTTORASTERFORMAT_H
