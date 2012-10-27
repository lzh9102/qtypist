#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QDialog>

namespace Ui {
class FileDialog;
}

class FileDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit FileDialog(QWidget *parent = 0);
    ~FileDialog();

    /**
     * @brief Get the full path of the selected file.
     */
    QString selectedFile() const;

private slots:
    void slotAccepted();

private:
    Ui::FileDialog *ui;
};

#endif // FILEDIALOG_H
