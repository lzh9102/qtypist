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
    QString current();

private:
    QStringList m_list;
    int m_index;
};

#endif // DATASOURCE_H
