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
