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

#include "datasource.h"
#include <QFile>
#include <QTextStream>
#include <cstdlib>

DataSource::DataSource(QObject *parent) :
    QObject(parent), m_index(-1)
{
}

DataSource::~DataSource()
{
}

bool DataSource::addFile(const QString &filename)
{
    // load file content
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream instr(&file);
        while (!instr.atEnd())
        {
            QString line = instr.readLine().trimmed();
            if (!line.isEmpty())
                m_list.push_back(line);
        }
        return true;
    } else {
        return false;
    }
}

void DataSource::clear()
{
    m_list.clear();
    m_index = -1;
}

bool DataSource::isEmpty() const
{
    return m_list.isEmpty();
}

QString DataSource::next()
{
    if (m_list.isEmpty())
        return "";
    else {
        m_index = rand() % m_list.size();
        return m_list[m_index].trimmed();
    }
}

QString DataSource::current()
{
    if (m_index < 0)
        return "";
    else
        return m_list[m_index].trimmed();
}
