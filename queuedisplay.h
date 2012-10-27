#ifndef QUEUEDISPLAY_H
#define QUEUEDISPLAY_H

#include <QWidget>

class QueueDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit QueueDisplay(QWidget *parent = 0);
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

#endif // QUEUEDISPLAY_H
