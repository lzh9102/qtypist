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
     * @param priority the priority of the current (replaced) word
     * @return the next word
     */
    QString next(int priority);

    /**
     * @brief get the current word
     * @return the current word
     */
    QString current();

signals:
    
public slots:

private:
    void loadWords(int n);
    void updatePriority(int priority);

    class Entry {
    public:
        Entry(QString w, int p) : word(w), priority(p), count(0) { }
        // sort by reverse priority (higher priority < lower priority)
        bool operator<(const Entry& other) const { return priority > other.priority; }
        QString word;
        int priority;
        int count; ///< how many times have the word been used
    };
    DataSource& m_source;
    QList<Entry> m_list;
    int m_size;
    int m_index;
};

#endif // WORKINGSET_H
