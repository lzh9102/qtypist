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

#ifndef HISTORYDISPLAY_H
#define HISTORYDISPLAY_H

#include <QWidget>

class HistoryDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit HistoryDisplay(QWidget *parent = 0);
    enum Direction { TOP_TO_BOTTOM, BOTTOM_TO_TOP };

    int count() const;
    
signals:
    
public slots:
    void setDisplayCount(int n);
    void push(QString s);
    void clear();
    void setDirection(Direction dir);
    void setFontSize(int p);
    
private:
    QStringList m_strings;
    int m_displayCount;
    Direction m_direction;
    bool m_underline;
    bool m_hideParen; ///< hide parenthesized text
    void paintEvent(QPaintEvent *);
    void setUnderlineFront(bool b); // disabled
    void setHideParen(bool b); // disabled
};

#endif // HISTORYDISPLAY_H
