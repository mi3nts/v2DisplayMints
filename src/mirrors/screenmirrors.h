#ifndef SCREENMIRRORS_H
#define SCREENMIRRORS_H

#include <QWidget>
#include <QThread>

namespace Ui {
class ScreenMirrors;
}

class ScreenUpdateThread : public QThread
{
    Q_OBJECT

public:
    void setStartPos(int x, int y);
    void setPosition(int p);
    void showCursor(bool);
    void stop();

signals:
    void screenConnect();
    void screenDisconnect();

protected:
    int  rotatePixmap(QPixmap &);
    int  writePixmapToDisplay(QPixmap &);
    int  drawCursorToDisplay(QPixmap &);

public:
    void run();

private:
    int x, y, p, tag, cur;
    QImage arrow;
};

class ScreenMirrors : public QWidget
{
    Q_OBJECT

public:
    explicit ScreenMirrors(QWidget *parent = 0);
    ~ScreenMirrors();

public slots:
    void onConfirm();
    void onConnect();
    void onDisconnect();

private:
    Ui::ScreenMirrors *ui;

    ScreenUpdateThread thread;
};

#endif // SCREENMIRRORS_H
