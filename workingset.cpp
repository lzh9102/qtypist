#include "workingset.h"
#include "datasource.h"
#include <limits>
#include <cstdlib>
#include <QDebug>

#define DEFAULT_SIZE 10
#define DEFAULT_COUNT 5
#define INITIAL_PRIORITY std::numeric_limits<int>::max()
//#define INITIAL_PRIORITY 0

WorkingSet::WorkingSet(DataSource &source, QObject *parent) :
    QObject(parent), m_source(source), m_size(DEFAULT_SIZE), m_index(-1)
{
}

void WorkingSet::reset()
{
    m_list.clear();
    m_index = -1;
}

QString WorkingSet::next(int priority)
{
    if (m_list.isEmpty())
        loadWords(m_size);
    else if (m_index >= 0)
        updatePriority(priority);
    const int size = m_list.size();
    int index = -1;
    for (int i=0; i<size-1; i++) {
        if (rand() % 2 == 0) { // 1/2 chance to choose the item with higher priority
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
            return next(0);
        } else {
            ++entry.count;
            return entry.word;
        }
    } else
        return "";
}

QString WorkingSet::current()
{
    if (m_index >= 0)
        return m_list.at(m_index).word;
    else
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
