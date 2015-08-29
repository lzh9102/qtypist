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

#ifndef WORKINGSET_H
#define WORKINGSET_H

#include <QObject>
#include <QList>

class DataSource;

/**
 * This class maintains a set of words and their priorities.
 * A word with higher priority is more likely to appear when calling @c next().
 */
class WorkingSet : public QObject
{
    Q_OBJECT
public:
    explicit WorkingSet(DataSource& source, QObject *parent = 0);

    void reset();

    /**
     * @brief proceed to the next word
     * @return the next word
     */
    QString next();

    /**
     * @brief update priority of the current word
     * @param priority
     * @return
     */
    void updatePriority(int priority);

    /**
     * @brief get the current phrase
     * @return the current phrase
     */
    QString currentPhrase();

    /**
     * @brief get the comment of the current word
     * @return the comment of the current word
     */
    QString currentComment();

signals:
    
public slots:

private:
    void loadWords(int n);

    class Entry {
    public:
        Entry(QString line, int p);
        // sort by reverse priority (higher priority < lower priority)
        bool operator<(const Entry& other) const { return priority > other.priority; }
        QString word;
        QString comment;
        int priority;
        int count; ///< how many times have the word been used
    };
    DataSource& m_source;
    QList<Entry> m_list;
    int m_size;
    int m_index;
};

#endif // WORKINGSET_H
