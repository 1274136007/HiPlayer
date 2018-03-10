#include "lrcLabel.h"

LrcLabel::LrcLabel(QWidget *parent)
    : HiLabel(parent)
{
    setText(QString::fromUtf8("HiPlayer"));
    // 固定显示区域大小
    setMaximumSize(800, 60);
    setMinimumSize(800, 60);

    //歌词的线性渐变填充
    linearGradient.setStart(0, 10);//填充的起点坐标
    linearGradient.setFinalStop(0, 40);//填充的终点坐标
    //第一个参数终点坐标，相对于我们上面的区域而言，按照比例进行计算
    linearGradient.setColorAt(0.1, QColor(0, 148, 255));
    linearGradient.setColorAt(0.5, QColor(114, 232, 255));
    linearGradient.setColorAt(0.9, QColor(0, 148, 255));


    // 遮罩的线性渐变填充
    maskLinearGradient.setStart(0, 0);
    maskLinearGradient.setFinalStop(0, 40);
    maskLinearGradient.setColorAt(0.1, QColor(222, 54, 4));
    maskLinearGradient.setColorAt(0.5, QColor(255, 72, 16));
    maskLinearGradient.setColorAt(0.9, QColor(222, 54, 4));

    // 设置字体
    lrcFont.setFamily(QString::fromUtf8("微软雅黑"));
    //lrcFont.setBold(true);//字体加粗
    lrcFont.setPointSize(25);

    // 设置定时器
    lrcTimer = new QTimer(this);
    connect(lrcTimer, SIGNAL(timeout()), this, SLOT(slotTimeout()));
    lrcMaskWidth = 0;
    lrcMaskWidthInterval = 0;
}

// 开启遮罩，需要指定当前歌词开始与结束之间的时间间隔(ms)
void LrcLabel::hi_startLrcMask(qint64 intervaltime)
{
    // 这里设置每隔30毫秒更新一次遮罩的宽度，因为如果更新太频繁
    // 会增加CPU占用率，而如果时间间隔太大，则动画效果就不流畅了
    qreal count = intervaltime / 30;
     //获取遮罩每次需要增加的宽度，这里的800是部件的固定宽度
    lrcMaskWidthInterval = 800 / count;
    lrcMaskWidth = 0;
    lrcTimer->start(30);
}

void LrcLabel::hi_stopLrcMask()
{
    lrcTimer->stop();
    lrcMaskWidth = 0;
    update();
}

void LrcLabel::paintEvent(QPaintEvent *)
{

    QPainter painter(this);
    painter.setFont(lrcFont);

    if(text().size() < 2){
        setText("Music...");
    }
    QString tmp = text();
    tmp.toUtf8();
    QColor(0, 0, 0, 200);//90, 105, 115, 200
    // 先绘制底层文字，作为阴影，这样会使显示效果更加清晰，且更有质感
    painter.setPen(QColor(0, 0, 0, 200));
    painter.drawText(31, 1, 800, 60, Qt::AlignLeft, tmp);

    // 再在上面绘制渐变文字
    painter.setPen(QPen(linearGradient, 0));
    painter.drawText(30, 0, 800, 60, Qt::AlignLeft, tmp);

    // 设置歌词遮罩
    painter.setPen(QPen(maskLinearGradient, 0));
    painter.drawText(30, 0, lrcMaskWidth+30, 60, Qt::AlignLeft, tmp);
}


//右击事件
void LrcLabel::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;
    menu.addAction(QString::fromUtf8("隐藏"), this->parent(), SLOT(hide()));
    menu.exec(event->globalPos());//globalPos()为当前鼠标的位置坐标
}


void LrcLabel::slotTimeout()
{

    //每隔一段固定的时间笼罩的长度就增加一点
    lrcMaskWidth += lrcMaskWidthInterval;
    update();//更新widget，但是并不立即重绘，而是安排一个Paint事件，当返回主循环时由系统来重绘
}

void LrcLabel::hi_setFontSize(int size)//设置字体
{
    lrcFont.setPointSize(size);
}
