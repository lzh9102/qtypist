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

#ifndef CHARTDISPLAY_H
#define CHARTDISPLAY_H

#include <QWidget>
#include <deque>
#include <vector>

class ChartDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit ChartDisplay(QWidget *parent = 0);

public slots:
    void clear();
    void pushData(double value);
    void paintEvent(QPaintEvent*);
    void mouseMoveEvent(QMouseEvent *);
    void leaveEvent(QEvent *);

    void getHistory(QList<double>&) const;

private:
    Q_DISABLE_COPY(ChartDisplay)
    std::deque<double> m_data;
    QPoint m_mousepos;
    unsigned int m_count; // number of the latest data to be shown
    double m_max, m_min;
    unsigned int m_cycle; // total number of pushed data
};

#endif // CHARTDISPLAY_H
