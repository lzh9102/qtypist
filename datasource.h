#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QString>
#include <QStringList>

class DataSource : public QObject
{
    Q_OBJECT

public:
    explicit DataSource(QObject *parent = 0);
    ~DataSource();

    bool addFile(const QString& filename);
    void clear();
    bool isEmpty() const;

    QString next();

private:
    QStringList m_list;
};

#endif // DATASOURCE_H
