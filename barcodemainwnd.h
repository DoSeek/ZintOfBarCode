#ifndef BARCODEMAINWND_H
#define BARCODEMAINWND_H

#include <QMainWindow>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrinter>
#include <QImage>
#include <QPainter>
#include <QMessageBox>
#include <math.h>
#include <QFile>
#include <QScreen>
#include "Zint/include/zint.h"

namespace Ui {
class BarCodeMainWnd;
}

namespace BARCODENS {
//    const int MADEINFOFFSETX = 40;    //制造国信息X轴偏移量
//    const int MADEINFOFFSETY = 12;    //制造国信息Y轴偏移量
//    const int BARCODEOFFSETX = 40;    //条码X轴偏移量
//    const int BARCODEOFFSETY = -2;    //条码Y轴偏移量
//    const int DESCRIBELOFFSETX = 6;  //商品描述信息X轴偏移量
//    const int DESCRIBELOFFSETY = -2;  //商品描述信息Y轴偏移量
//    const float MllimeterPerInch = 25.4;   // 1英寸=25.4mm
//    const float BarCodeScaledRate = 0.35;  //条形码缩放比例
    const int MADEINFOFFSETX = 0;    //制造国信息X轴偏移量
    const int MADEINFOFFSETY = 0;    //制造国信息Y轴偏移量
    const int BARCODEOFFSETX = 0;    //条码X轴偏移量
    const int BARCODEOFFSETY = 0;    //条码Y轴偏移量
    const int DESCRIBELOFFSETX = 3;  //商品描述信息X轴偏移量
    const int DESCRIBELOFFSETY = 0;  //商品描述信息Y轴偏移量
    const float MllimeterPerInch = 25.4;   // 1英寸=25.4mm
    const float BarCodeScaledRate = 0;  //条形码缩放比例
}

class BarCodeMainWnd : public QMainWindow
{
    Q_OBJECT

public:
    explicit BarCodeMainWnd(QWidget *parent = 0);
    ~BarCodeMainWnd();

private slots:
    void on_btnPrint_clicked();

private:
    Ui::BarCodeMainWnd *ui;

    void printBarcode(const QString &strTitle);
    void printBarcode(const QImage &barCodeImg);
    void printBarCodeVLabel(const QString &strNum, const QString &label="Made in China");
    QImage ConstructBarCode(const QString &strNum, const QString &describeLabel, const QString &madeInfoLabel="Made in China", const QSize &size=QSize(40/*mm*/, 30/*mm*/));
    QSize calcStringMetrics(const QString &str, const QFont &font);
};

#endif // BARCODEMAINWND_H
