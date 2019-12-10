#include "screenmirrors.h"
#include "ui_screenmirrors.h"

#include <QScreen>
#include <QBuffer>
#include <QPixmap>
#include <QBitmap>
#include <QLibrary>
#include <QPainter>
#include <QDesktopWidget>
#include <QMessageBox>

typedef int (*_fn_void)();
typedef int (*_fn_s8)(const char *);

_fn_void _conn = NULL;     // vodisp_connect
_fn_void _disc = NULL;     // vodisp_disconnect
_fn_s8   _frame = NULL;    // vodisp_write_frame

ScreenMirrors::ScreenMirrors(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScreenMirrors)
{
    ui->setupUi(this);

    connect(ui->pbOK, SIGNAL(clicked(bool)), SLOT(onConfirm()));
    connect(&thread, SIGNAL(screenConnect()), SLOT(onConnect()));
    connect(&thread, SIGNAL(screenDisconnect()), SLOT(onDisconnect()));

    QLibrary lib("vodisp");
    if (!lib.load())
        QMessageBox::critical(NULL, "Error", "Can not find vodispext library.");

    _conn = (_fn_void)lib.resolve("vodisp_connect");
    _disc = (_fn_void)lib.resolve("vodisp_disconnect");
    _frame =  (_fn_s8)lib.resolve("vodisp_write_frame");

    if (_conn == NULL || _disc == NULL || _frame == NULL)
        exit(0);        // can not load, exit!

    thread.start();
}

ScreenMirrors::~ScreenMirrors()
{
    delete ui;

    thread.stop();
    if (!thread.wait(500))
        thread.terminate();
}

void ScreenMirrors::onConfirm()
{
    thread.setStartPos(ui->edX->text().toInt(), ui->edY->text().toInt());
    thread.setPosition(ui->rbHori->isChecked() ? 0 : 1);
    thread.showCursor(ui->cbCur->isChecked());
}

void ScreenMirrors::onConnect()
{
    ui->lbStatus->setText("Status: device connected");
}

void ScreenMirrors::onDisconnect()
{
    ui->lbStatus->setText("Status: device not found");
}

void ScreenUpdateThread::setStartPos(int sx, int sy)
{
    x = sx;
    y = sy;
}

void ScreenUpdateThread::setPosition(int sp)
{
    p = sp;
}

void ScreenUpdateThread::showCursor(bool scur)
{
    cur = scur;
}

void ScreenUpdateThread::stop()
{
    tag = 1;
}

int ScreenUpdateThread::rotatePixmap(QPixmap &pixmap)
{
    QMatrix matrix;
    matrix.rotate(90.0);

    QImage image = pixmap.toImage().transformed(matrix, Qt::FastTransformation);
    pixmap = QPixmap::fromImage(image);

    return 0;
}

int ScreenUpdateThread::writePixmapToDisplay(QPixmap &pixmap)
{
    QByteArray array;
    QBuffer buffer(&array);

    buffer.open(QBuffer::WriteOnly);
    pixmap.save(&buffer, "BMP");

    // skip 54 bytes bitmap header.
    if (_frame((char *)buffer.data().data() + 54) <= 0)
        return -1;

    return 0;
}

int ScreenUpdateThread::drawCursorToDisplay(QPixmap &pixmap)
{
    if (!cur)
        return 0;     // skip draw the cursor.

    QPainter painter(&pixmap);
    QPoint pos = QCursor::pos();
    painter.drawImage(pos.x() - x, pos.y() - y, arrow);

    return 0;
}

void ScreenUpdateThread::run()
{
    x = y = p = tag = cur = 0;
    arrow = QImage(":/cur/arrow.cur");

    _conn();    // connect to usb device.

    emit screenConnect();

    while (1) {
        QScreen *screen = QApplication::screens().first();
        QPixmap pixmap;

        if (tag)
            break;

        if (p) {
            pixmap = screen->grabWindow(0, x, y, 480, 800);
            drawCursorToDisplay(pixmap);
        } else {
            pixmap = screen->grabWindow(0, x, y, 800, 480);
            drawCursorToDisplay(pixmap);
            rotatePixmap(pixmap);
        }

        if (writePixmapToDisplay(pixmap) < 0) {
            emit screenDisconnect();
            break;
        }

        // add sleep here.

    }

    _disc();    // disconnect from usb device.
}
