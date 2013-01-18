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

#ifndef HISTORYDISPLAY_H
#define HISTORYDISPLAY_H

#include <QWidget>

class HistoryDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit HistoryDisplay(QWidget *parent = 0);
    enum Direction { TOP_TO_BOTTOM, BOTTOM_TO_TOP };

    QString front() const;
    bool isEmpty() const;
    int count() const;
    
signals:
    
public slots:
    void setDisplayCount(int n);
    void push(QString s);
    void pop();
    void clear();
    void setDirection(Direction dir);
    void setUnderlineFront(bool b);
    void setFontSize(int p);
    void setHideParen(bool b);
    
private:
    QStringList m_strings;
    int m_displayCount;
    Direction m_direction;
    bool m_underline;
    bool m_hideParen; ///< hide parenthesized text
    void paintEvent(QPaintEvent *);
};

#endif // HISTORYDISPLAY_H
