#include "queuedisplay.h"
#include <QPainter>
#include <QPen>
#include <QDebug>
#include <algorithm>

#define DEFAULT_DISPLAY_COUNT 20

QueueDisplay::QueueDisplay(QWidget *parent) :
    QWidget(parent),
    m_displayCount(DEFAULT_DISPLAY_COUNT),
    m_direction(BOTTOM_TO_TOP),
    m_underline(false)
{
}

void QueueDisplay::setDisplayCount(int n)
{
    if (n > 0) {
        m_displayCount = n;
        repaint();
    }
}

void QueueDisplay::push(QString s)
{
    m_strings.push_back(s);
    if (m_displayCount >= m_strings.size())
        repaint();
}

void QueueDisplay::pop()
{
    if (!m_strings.isEmpty()) {
        m_strings.pop_front();
        repaint();
    } else
        qWarning("QueueDisplay::pop(): the queue is empty");
}

QString QueueDisplay::front() const
{
    if (!m_strings.isEmpty())
        return m_strings.front();
    else {
        qWarning("QueueDisplay::top(): the queue is empty");
        return QString();
    }
}

bool QueueDisplay::isEmpty() const
{
    return m_strings.isEmpty();
}

void QueueDisplay::setDirection(Direction dir)
{
    switch (dir) {
    case TOP_TO_BOTTOM:
    case BOTTOM_TO_TOP:
        m_direction = dir;
        repaint();
        break;
    default:
        qWarning("QueueDisplay::setDirection(): incorrect direction");
    }
}

void QueueDisplay::setUnderlineFront(bool b)
{
    if (m_underline != b) {
        m_underline = b;
        repaint();
    }
}

void QueueDisplay::setFontSize(int p)
{
    QFont font = this->font();
    font.setPointSize(p);
    setFont(font);
    repaint();
}

void QueueDisplay::paintEvent(QPaintEvent *)
{
    const int count = std::min(m_strings.size(), m_displayCount);
    QPainter painter(this);
    QPen pen = painter.pen();
    const int font_height = painter.fontMetrics().height();

    for (int i=0; i<count; i++) {
        const int font_y = (m_direction == BOTTOM_TO_TOP)
                    ? height() - 1 - font_height * (i+1)
                    : font_height * i;
        const double opacity = (1.0 - i * ((double)font_height / height()));
        QRect region(0, font_y, width(), font_height);

        if (region.top() < 0 || region.bottom() >= height())
            break; // out of border

        painter.setOpacity(opacity);

        if (m_underline && i == 0) { // underline the first item in the queue
            QFont font = painter.font();
            font.setUnderline(true);
            painter.setFont(font);
        } else if (painter.font().underline()) { // cancel underline
            QFont font = painter.font();
            font.setUnderline(false);
            painter.setFont(font);
        }

        painter.drawText(region, m_strings[i]
                         , QTextOption(Qt::AlignLeft | Qt::AlignTop));
        qDebug() << "QueueDisplay::paintEvent(): "
                 << "text: " << m_strings[i]
                 << "y: " << font_y << ", h: " << font_height;
    }

    // DEBUG: draw border
    //QRect region(0, 0, width()-1, height()-1);
    //painter.drawRect(region);
}
