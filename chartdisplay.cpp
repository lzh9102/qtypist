/* qtypist - a typing training program
 *
 * Copyright (C) 2012-2015 Che-Huai Lin <lzh9102@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 or 3 of the License.
 *
 * project page: https://github.com/lzh9102/qtypist
 */

#include "chartdisplay.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

#define OFFSET_H 40
#define OFFSET_V 10
#define DEFAULT_COUNT 20
#define DEFAULT_COLOR QColor(150,200,220)
#define DEFAULT_WIDTH 3
#define BG_COLOR QColor(255,255,255)
#define AXIS_COLOR QColor(0, 0, 0)
#define CURSOR_COLOR QColor(128,128,128)
#define MIN_HEIGHT 100

ChartDisplay::ChartDisplay(QWidget *parent) :
    QWidget(parent), m_mousepos(), m_count(DEFAULT_COUNT)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    this->setMinimumHeight(MIN_HEIGHT);
    this->setMouseTracking(true); // mouse event regardless of button pressed
    this->clear();
}

void ChartDisplay::clear()
{
    m_cycle = 0;
    m_data.clear();
    m_max = 0;
    m_min = 0;
    this->repaint();
}

void ChartDisplay::pushData(double value)
{
    m_data.push_back(value);
    if (m_data.size() > m_count)
        m_data.pop_front();
    m_max = 0;
    m_min = value;
    for (uint i=0; i<m_data.size(); i++) {
        m_max = (m_data[i] > m_max) ? m_data[i] : m_max;
        m_min = (m_data[i] < m_min) ? m_data[i] : m_min;
    }

    ++m_cycle;
    this->repaint();
}

void ChartDisplay::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QPen pen;
    double prev_val = 0;
    const int left = OFFSET_H, right = this->width()-1;
    const int top = OFFSET_V, bottom = this->height()-1;
    const int w = right - left, h = bottom - top;

    if (!m_data.empty())
        prev_val = m_data[0];

    // draw lines

    pen.setColor(DEFAULT_COLOR);
    pen.setWidth(DEFAULT_WIDTH);
    painter.setPen(pen);

    for (uint i=0; i<m_data.size(); i++) {
        int x1, y1, x2, y2;
        x1 = w * i / m_count + left;
        y1 = h - (prev_val - m_min) * h / (m_max - m_min + 1) + top;
        x2 = w * (i+1) / m_count + left;
        y2 = h - (m_data[i] - m_min) * h / (m_max - m_min + 1) + top;
        painter.drawLine(x1, y1, x2, y2);
        prev_val = m_data[i];
    }

    // draw axis
    pen.setColor(AXIS_COLOR);
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawLine(left, top, right, top);
    painter.drawLine(left, top, left, bottom);
    painter.drawLine(left, h/2+top, right, h/2+top);
    painter.drawLine(left, bottom, right, bottom);
    painter.drawLine(right, top, right, bottom);

    painter.drawText(0, top + 10, QString::number(m_max + 1, 'g', 3));
    painter.drawText(0, h/2+top, QString::number((m_max + m_min + 1) / 2, 'g', 3));
    painter.drawText(0, bottom, QString::number(m_min, 'g', 3));

    // draw cursor lines
    if (QRect(left, top, w, h).contains(m_mousepos)) {
        int index_max = m_data.size()-1;
        // calculate closest index
        int index = (m_mousepos.x() - left) * m_count / w;
        if (index >= 0 && index <= index_max) {
            int line_x = left + w * (index+1) / m_count;
            int line_y = h - (m_data[index] - m_min) * h / (m_max - m_min + 1) + top;
            pen.setColor(CURSOR_COLOR);
            pen.setWidth(1);
            painter.setPen(pen);
            painter.drawLine(line_x, top, line_x, bottom);
            painter.drawLine(left, line_y, right, line_y);
            painter.drawText(left + 3, bottom - 3, tr("Number: %1, Speed: %2")
                             .arg(m_cycle - (index_max - index))
                             .arg(m_data[index]));
        }
    }
}

void ChartDisplay::mouseMoveEvent(QMouseEvent *event)
{
    m_mousepos = event->pos();
    this->repaint();
}

// mouse leave
void ChartDisplay::leaveEvent(QEvent *)
{
    m_mousepos.setX(-1);
    m_mousepos.setY(-1);
}

void ChartDisplay::getHistory(QList<double> &v) const
{
    v.clear();
    for (int i=m_data.size()-1; i>=0; i--)
        v.append(m_data[i]);
}
