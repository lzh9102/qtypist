/* qtypist - a typing training program
 *
 * Copyright (C) 2012 Timothy Lin <lzh9102@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 or 3 of the License.
 *
 * project page: https://github.com/lzh9102/qtypist
 */

#include "workingset.h"
#include "datasource.h"
#include <limits>
#include <cstdlib>
#include <QDebug>
#include <QRegExp>

#define DEFAULT_SIZE 20
#define DEFAULT_COUNT 5
#define INITIAL_PRIORITY std::numeric_limits<int>::max()
//#define INITIAL_PRIORITY 0

WorkingSet::Entry::Entry(QString line, int p)
    : priority(p), count(0)
{
    // half-width and full-width semicolons
    QStringList fields = line.split(QRegExp("  |\t"), QString::SkipEmptyParts);
    if (fields.size() >= 1)
        word = fields[0].trimmed();
    if (fields.size() >= 2)
        comment = fields[1].trimmed();
}

WorkingSet::WorkingSet(DataSource &source, QObject *parent) :
    QObject(parent), m_source(source), m_size(DEFAULT_SIZE), m_index(-1)
{
}

void WorkingSet::reset()
{
    m_list.clear();
    m_index = -1;
}

QString WorkingSet::next()
{
    if (m_list.isEmpty())
        loadWords(m_size);
    const int size = m_list.size();
    int index = -1;
    for (int i=0; i<size-1; i++) {
        if (rand() % 3 == 0) { // 1/3 chance to choose the item with higher priority
            index = i;
            break;
        }
    }
    if (index == -1) { // no item has been selected, use the last item
        index = size-1;
    }
    m_index = index;
    if (m_index >= 0) {
        Entry& entry = m_list[m_index];
        if (entry.count >= DEFAULT_COUNT) {
            // if the word has appeared enough times
            // then remove it from the list
            // and load a new word
            m_list.removeAt(m_index);
            loadWords(m_size - m_list.size());
            m_index = -1;
            updatePriority(0);
            return next();
        } else {
            ++entry.count;
            return entry.word;
        }
    } else
        return "";
}

void WorkingSet::updatePriority(int priority)
{
    if (m_index >= 0) {
        m_list[m_index].priority = priority;
        qSort(m_list);
    }

    QString dbgmsg = "updatePriority(): ";
    for (int i=0; i<m_list.size(); i++) {
        dbgmsg += m_list[i].word + " " + QString::number(m_list[i].priority) + "; ";
    }
    qDebug() << dbgmsg;
}

QString WorkingSet::currentPhrase()
{
    if (m_index >= 0)
        return m_list.at(m_index).word;
    else
        return "";
}

QString WorkingSet::currentComment()
{
    if (m_index >= 0)
        return m_list.at(m_index).comment;
    else
        return "";
}

void WorkingSet::loadWords(int n)
{
    for (int i=0; i<n; i++) {
        m_list.push_back(Entry(m_source.next(), INITIAL_PRIORITY));
    }
    qSort(m_list);

    QString dbgmsg = "loadWords(): ";
    for (int i=0; i<m_list.size(); i++) {
        dbgmsg += m_list[i].word + " " + QString::number(m_list[i].priority) + "; ";
    }
    qDebug() << dbgmsg;
}
