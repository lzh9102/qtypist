/* qtypist - a typing training program
 *
 * Copyright (C) 2012 Timothy Lin <lzh9102@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 or 3 of the License.
 *
 * project page: http://code.google.com/p/qtypist
 */

#include "queuedisplay.h"
#include <QPainter>
#include <QPen>
#include <QDebug>
#include <algorithm>

#define DEFAULT_DISPLAY_COUNT 50

QueueDisplay::QueueDisplay(QWidget *parent) :
    QWidget(parent),
    m_displayCount(DEFAULT_DISPLAY_COUNT),
    m_direction(BOTTOM_TO_TOP),
    m_underline(false),
    m_hideParen(false)
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

void QueueDisplay::clear()
{
    m_strings.clear();
    repaint();
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

int QueueDisplay::count() const
{
    return m_strings.count();
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

void QueueDisplay::setHideParen(bool b)
{
    m_hideParen = b;
    repaint();
}

void QueueDisplay::paintEvent(QPaintEvent *)
{
    const int count = std::min(m_strings.size(), m_displayCount);
    QPainter painter(this);
    const int font_height = painter.fontMetrics().height();
    QRegExp paren("\\([^)]*\\)");

    for (int i=0; i<count; i++) {
        const int font_y = (m_direction == BOTTOM_TO_TOP)
                    ? height() - 1 - font_height * (i+1)
                    : font_height * i;
        const double opacity = (1.0 - i * ((double)font_height / height()));
        QRect region(0, font_y, width(), font_height);

        if (region.bottom() < 0 || region.top() >= height())
            break; // out of border

        painter.setOpacity(opacity);

        if (i == 0) {
            QFont font = painter.font();
            if (m_underline)
                font.setUnderline(true);
            font.setBold(true);
            painter.setFont(font);
        } else {
            QFont font = painter.font();
            font.setUnderline(false);
            font.setBold(false);
            painter.setFont(font);
        }

        QString string = m_strings[i];
        if (m_hideParen)
            string.remove(paren);

        painter.drawText(region, string
                         , QTextOption(Qt::AlignLeft | Qt::AlignTop));
    }

    // DEBUG: draw border
    //QRect region(0, 0, width()-1, height()-1);
    //painter.drawRect(region);
}
