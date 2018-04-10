#include <QApplication>

#include "C_PadCalibrateForm.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    C_PadCalibrateForm form;
    form.show();

    return a.exec();
}
