#include "barcodemainwnd.h"
#include "ui_barcodemainwnd.h"

BarCodeMainWnd::BarCodeMainWnd(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BarCodeMainWnd)
{
    ui->setupUi(this);
}

BarCodeMainWnd::~BarCodeMainWnd()
{
    delete ui;
}

void BarCodeMainWnd::on_btnPrint_clicked()
{
    //将图片绘制在100个像素之内 看下效果
    /*QPrinter printer;
    QPrintDialog printdialog(&printer, this);
    if (printdialog.exec())
    {
        //这里我如何保证不进行拉伸呢
        QString strname = printer.printerName();
        int dpi = printer.logicalDpiX();
        int width = printer.paperRect(QPrinter::Millimeter).width()*(dpi*1.0/25.4);     //这个再试下
        QPainter painter(&printer);
        QImage image("./test.png", "PNG");
        QRect rt1 = image.rect();
        QRect rtwindow = painter.window();
        QRect rtview = painter.viewport();
        int nmax = 0;
        if (image.width() + 20 < width)
            nmax = std::max(image.width() + 20, width);
        else
            nmax = std::min(image.width() + 20, width);

        QRect rtimage(0, 0, nmax, rtwindow.height());
        QRect rtimageview(0, 0, nmax, rtview.height());
        int nmin = std::min(image.width(), nmax - 80);
        QRectF rtdes(40, 10, nmin, 100);
        QRectF rtsrc(0, 0, image.width(), image.height());
        painter.setWindow(rtimage);
        painter.setViewport(rtimageview);
        painter.drawImage(rtdes, image, rtsrc);
        painter.setWindow(rtwindow);
        painter.setViewport(rtview);
    }*/

    QString strMsg = ui->lineEdit->text();
//    if (!strMsg.isEmpty())
//        printBarcode(strMsg);

//    if(!strMsg.isEmpty())
//        printBarCodeVLabel(strMsg);

    QImage barCode128Img;
    if(!strMsg.isEmpty())
        barCode128Img = ConstructBarCode(strMsg, "XBox Player Control Sony Company");

    ui->label->setPixmap(QPixmap().fromImage(barCode128Img));
    QImage barCode128ScaledImg = barCode128Img.scaled(barCode128Img.width()*0.9, barCode128Img.height(),Qt::KeepAspectRatio);
    barCode128Img.save("barcode128.png");
    barCode128ScaledImg.save("barcode128Scaled.png");
    printBarcode(barCode128Img);
}

// Code93 : BJ100080
// Code128 : 95270078
void BarCodeMainWnd::printBarcode(const QString &strTitle)
{
    //打印部分
    //这里先生成图片
    QString strImagePath("");
    strImagePath = QCoreApplication::applicationDirPath() + "//test.png";

    //测试1：
    struct zint_symbol * my_symbol = ZBarcode_Create();
    my_symbol->symbology =  BARCODE_CODE128/*BARCODE_CODE93*/;//BARCODE_CODE11;
    strcpy(my_symbol->outfile, strImagePath.toLocal8Bit().toStdString().c_str());
    ZBarcode_Encode(my_symbol, (unsigned char*)strTitle.toStdString().c_str(), 0);
    ZBarcode_Print(my_symbol, 0);
    ZBarcode_Delete(my_symbol);

//    //测试2：
//    struct zint_symbol * my_symbol = ZBarcode_Create();
//    my_symbol->symbology=BARCODE_CODE11;
//    char *testvalue ="test.png";
//    strcpy(my_symbol->outfile,testvalue);
//    ZBarcode_Encode(my_symbol,(unsigned char*)"12345678",0);
//    ZBarcode_Print(my_symbol,0);
//    ZBarcode_Delete(my_symbol);

//    //测试3：
//    struct zint_symbol * my_symbol = ZBarcode_Create();
//    struct zint_render_line my_render_line;
//    my_render_line.x = my_render_line.y = 2;
//    my_render_line.length = 40;
//    my_render_line.next = NULL;

//    char* label = "Made in Chain";
//    struct zint_render_string my_zint_render_string;
//    my_zint_render_string.x = my_zint_render_string.y = 2;
//    my_zint_render_string.fsize =  50;
//    my_zint_render_string.length = 40;
//    my_zint_render_string.text = (unsigned char*)label;
//    my_zint_render_string.next = NULL;

//    struct zint_render my_zint_render;
//    my_zint_render.height = 10;
//    my_zint_render.width = 40;
//    my_zint_render.strings = &my_zint_render_string;
//    my_zint_render.lines = &my_render_line;
//    my_zint_render.rings = NULL;
//    my_zint_render.hexagons = NULL;

//    my_symbol->rendered = NULL;
//    my_symbol->symbology =  BARCODE_CODE128;
//    strcpy(my_symbol->outfile, strImagePath.toLocal8Bit().toStdString().c_str());
//    ZBarcode_Encode(my_symbol, (unsigned char*)strTitle.toStdString().c_str(), 0);
//    ZBarcode_Print(my_symbol, 0);
//    ZBarcode_Delete(my_symbol);

    //绘制
    QPrinter printer;
    QPrintDialog printdialog(&printer, this);
    if (printdialog.exec())
    {
        //这里我如何保证不进行拉伸呢
        //QString strname = printer.printerName();
        int dpi = printer.logicalDpiX();
        int nmm = printer.paperRect(QPrinter::Millimeter).width();
        int width = std::min(nmm, 80)*(dpi*1.0/25.4);     //这里选择80mm纸张
        QPainter painter(&printer);
        QImage image(strImagePath, "PNG");
        QRect rt1 = image.rect();
        QRect rtwindow = painter.window();
        QRect rtview = painter.viewport();
        int nmax = 0;
        if (image.width() + 20 < width)
            nmax = std::max(image.width() + 20, width);
        else
            nmax = std::min(image.width() + 20, width);

        QRect rtimage(0, 0, nmax, rtwindow.height());
        QRect rtimageview(0, 0, nmax, rtview.height());
        int nmin = std::min(image.width(), nmax - 80);
        QRectF rtdes(40, 10, nmin, 100);
        QRectF rtsrc(0, 0, image.width(), image.height());
        painter.setWindow(rtimage);
        painter.setViewport(rtimageview);
        painter.drawImage(rtdes, image, rtsrc);
        painter.setWindow(rtwindow);
        painter.setViewport(rtview);
    }

    //这里再删除图片
    //QFile file(strImagePath);
    //QFile::remove(strImagePath);
}

void BarCodeMainWnd::printBarcode(const QImage &barCodeImg)
{
    //绘制
    QPrinter printer;
    QPrintDialog printdialog(&printer, this);
    if (printdialog.exec())
    {
        //这里我如何保证不进行拉伸呢
        //QString strname = printer.printerName();
        int dpi = printer.logicalDpiX();
        int nmm = printer.paperRect(QPrinter::Millimeter).width();
        int width = std::min(nmm, 40)*(dpi*1.0/25.4);     //这里选择40mm纸张
        QPainter painter(&printer);
        //QImage image(strImagePath, "PNG");
        QRect rt1 = barCodeImg.rect();
        QRect rtwindow = painter.window();
        QRect rtview = painter.viewport();
        int nmax = 0;
        if (barCodeImg.width() + 20 < width)
            nmax = std::max(barCodeImg.width() + 20, width);
        else
            nmax = std::min(barCodeImg.width() + 20, width);

        QRect rtimage(0, 0, nmax, rtwindow.height());
        QRect rtimageview(0, 0, nmax, rtview.height());
        int nmin = std::min(barCodeImg.width(), nmax - 40);
        QRectF rtdes(40, 10, nmin, 100);
        QRectF rtsrc(0, 0, barCodeImg.width(), barCodeImg.height());
        painter.setWindow(rtimage);
        painter.setViewport(rtimageview);
        painter.drawImage(rtdes, barCodeImg, rtsrc);
        painter.setWindow(rtwindow);
        painter.setViewport(rtview);
    }
}

// dpi : dots per inch,直接来说就是一英寸多少个像素点。常见取值 120，160，240。我一般称作像素密度，简称密度。
// 1英寸 : 25.4毫米(mm)
void BarCodeMainWnd::printBarCodeVLabel(const QString &strNum, const QString &label/*="Made in China"*/)
{
    //这里先生成图片
    QString strImagePath("");
    strImagePath = QCoreApplication::applicationDirPath() + "//test.png";

    //生成图片
    struct zint_symbol * my_symbol = ZBarcode_Create();
    my_symbol->symbology =  BARCODE_CODE128;
    strcpy(my_symbol->outfile, strImagePath.toLocal8Bit().toStdString().c_str());
    ZBarcode_Encode(my_symbol, (unsigned char*)strNum.toStdString().c_str(), 0);
    ZBarcode_Print(my_symbol, 0);
    ZBarcode_Delete(my_symbol);

    //计算label串尺寸
    QFont font;
    font.setFamily("Courier New");
    font.setPixelSize(12);
    QSize labelSize = calcStringMetrics(label, font);

    //绘制
    QPrinter printer;
    QPrintDialog printdialog(&printer, this);
    if (printdialog.exec())  //打开打印设置窗口
    {
        //这里我如何保证不进行拉伸呢
        int dpi = printer.logicalDpiX();
        int prtPaperWidth = printer.paperRect(QPrinter::Millimeter).width();
        int prtWidth = std::min(prtPaperWidth, 80)*(dpi*1.0/25.4);     //这里选择80mm纸张

        QPainter painter(&printer);
        QImage barCodeImg(strImagePath, "PNG");
        //获取打印机初始窗口大小、视口大小
        QRect rcPrtWindow = painter.window();
        QRect rcPrtViewPort = painter.viewport();

        //计算条码区域
        int nmax = 0;
        if (barCodeImg.width() + 20 < prtWidth)
            nmax = std::max(barCodeImg.width() + 20, prtWidth) + 10;
        else
            nmax = std::min(barCodeImg.width() + 20, prtWidth) + 10;

        QRect rcCodeBar(0, 0, nmax, rcPrtWindow.height());
        QRect rcCodeBarView(0, 0, nmax, rcPrtViewPort.height());
        int nmin = std::min(barCodeImg.width(), nmax - 80);
        QRectF rtdes(0, 14, nmin, 100);
        QRectF rtsrc(0, 0, barCodeImg.width(), barCodeImg.height());
        painter.setWindow(rcCodeBar);
        painter.setViewport(rcCodeBarView);
        painter.drawText((barCodeImg.width()-labelSize.width())/2,10,label);
        painter.drawImage(rtdes, barCodeImg, rtsrc);
        painter.setWindow(rcPrtWindow);
        painter.setViewport(rcPrtViewPort);
    }

    //这里再删除图片
    QFile file(strImagePath);
    QFile::remove(strImagePath);
}

QImage BarCodeMainWnd::ConstructBarCode(const QString &strNum, const QString &describeLabel, const QString &madeInfoLabel/*="Made in China"*/, const QSize &size/*(40mm, 30mm)*/)
{
    //获取物理显示设备的尺寸信息
    int dpiX = 0, dpiY = 0;
    {
        QScreen *primaryScreen = qApp->primaryScreen();
        dpiX = primaryScreen->physicalDotsPerInchX();  //X轴方向：每英寸多个像素点
        dpiY = primaryScreen->physicalDotsPerInchY();  //Y轴方向：每英寸多个像素点
    }

    // 1 mm = 1.0/25.4 (in)       --  1.0/BARCODENS::MllimeterPerInch           1毫米=1/25.4英寸
    // dpiX dpi = dpiX (pix/in)   --  (1.0/BARCODENS::MllimeterPerInch)*dpiX    1英寸=dpiX像素点
    // 1 mm = 1/25.4*dpiX (pix)   --  1*(1.0/BARCODENS::MllimeterPerInch)*dpiX  dize.width()毫米=size.width()*(1.0/BARCODENS::MllimeterPerInch)*dpiX像素点
    int devPixWidth = size.width()*(1.0/BARCODENS::MllimeterPerInch)*dpiX;    //像素
    int devPixHeigth = size.height()*(1.0/BARCODENS::MllimeterPerInch)*dpiY;  //像素


    //这里先生成图片
    QString strImagePath("");
    strImagePath = QCoreApplication::applicationDirPath() + "//test.png";

    //生成图片
    struct zint_symbol * my_symbol = ZBarcode_Create();
    my_symbol->symbology =  BARCODE_CODE128;
    my_symbol->height = 25;  //条码高度(输入参数)
    //my_symbol->width = 40;   //条码宽度(输出参数)
    //my_symbol->output_options += SMALL_TEXT;  //使用更小的字体显示编号
    //strcpy_s((char*)my_symbol->text, sizeof(my_symbol->text),describeLabel.toStdString().c_str());
    strcpy(my_symbol->outfile, strImagePath.toLocal8Bit().toStdString().c_str());
    ZBarcode_Encode(my_symbol, (unsigned char*)strNum.toStdString().c_str(), 0);
    ZBarcode_Print(my_symbol, 0);
    ZBarcode_Delete(my_symbol);
    //从图片生成QIamge
    QImage barCodeImg(strImagePath, "PNG");
    QImage barCodeScaledImg = barCodeImg.scaled(barCodeImg.width()/**0.65*/, barCodeImg.height()/**0.65*/, Qt::KeepAspectRatio);  //缩放

    //(左上角)定位点
    QPoint ptMadeInfoLabel(BARCODENS::MADEINFOFFSETX, BARCODENS::MADEINFOFFSETY);    //制造国
    QPoint ptBarCode(BARCODENS::BARCODEOFFSETX, BARCODENS::BARCODEOFFSETY);          //条码
    QPoint ptDescribeLabel(BARCODENS::DESCRIBELOFFSETX, BARCODENS::DESCRIBELOFFSETY);//商品描述

    //计算madeInfoLabel串尺寸
    QFont font;
    font.setFamily("Courier New");
    font.setPixelSize(11);
    QSize szMadeInfoLabel = calcStringMetrics(madeInfoLabel, font);

    //绘制到QPixmap
    QPixmap pix(QSize(devPixWidth+50, devPixHeigth));
    pix.fill();
    QPainter p(&pix);
    p.setFont(font);

    //获取画板尺寸信息
    QRect rcWindow = p.window();
    QRect rcView = p.viewport();
    //设置画板尺寸
    //p.setWindow(-devPixWidth/2, -devPixHeigth/2, devPixWidth, -devPixHeigth);
    //p.setViewport(0, 0, devPixWidth, devPixHeigth);
    //绘制制造国信息
    QPoint ptMadeInfo((devPixWidth-szMadeInfoLabel.width())/2 + ptMadeInfoLabel.x(), ptMadeInfoLabel.y());
    p.drawText(QRect(ptMadeInfo, QPoint(devPixWidth, szMadeInfoLabel.height())), madeInfoLabel);



    //绘制条码
    //QRect rcTarget(ptBarCode.x(), ptMadeInfoLabel.y() + szMadeInfoLabel.height() + ptBarCode.y(), devPixWidth, devPixHeigth);
    //QRect rcSource(0, barCodeScaledImg.height()*0.5, barCodeScaledImg.width(), barCodeScaledImg.height());
    //p.drawImage(rcTarget, barCodeScaledImg, rcSource);
    p.drawImage((devPixWidth-barCodeScaledImg.width())/2 + ptBarCode.x(), \
                /*ptMadeInfoLabel.y() + */szMadeInfoLabel.height() + ptBarCode.y(),\
                barCodeScaledImg,\
                0, 0);
    //绘制商品信息
    font.setPixelSize(10);
    p.setFont(font);
    QPoint ptDescribe(ptDescribeLabel.x(), /*ptMadeInfo.y()+ ptBarCode.y() + ptDescribeLabel.y() + */ szMadeInfoLabel.height() + barCodeScaledImg.height());
    p.drawText(QRect(ptDescribe, QPoint(devPixWidth-BARCODENS::DESCRIBELOFFSETX, devPixHeigth)), Qt::TextWrapAnywhere, describeLabel);

    //p.setWindow(rcWindow);
    //p.setViewport(rcView);

    //结束绘制
    p.end();

    //这里再删除图片
    //QFile file(strImagePath);
    //QFile::remove(strImagePath);

    return pix.toImage();
}

QSize BarCodeMainWnd::calcStringMetrics(const QString &str, const QFont &font)
{
    //font.setPixelSize(20);  //此处是指定字体的大小
    QFontMetrics fm(font);
    int width=fm.width(str);
    int height=fm.height(); //这个是无参函数，得到字体的高度
    return QSize(width, height);
}


