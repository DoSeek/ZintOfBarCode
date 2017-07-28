#include "barcodemainwnd.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BarCodeMainWnd w;
    w.show();

//    //测试
//    struct zint_symbol * my_symbol = ZBarcode_Create();
//    my_symbol->symbology=BARCODE_CODE11;
//    char *testvalue ="test.png";
//    strcpy(my_symbol->outfile,testvalue);
//    ZBarcode_Encode(my_symbol,(unsigned char*)"12345678",0);
//    ZBarcode_Print(my_symbol,0);
//    ZBarcode_Delete(my_symbol);


    return a.exec();
}
