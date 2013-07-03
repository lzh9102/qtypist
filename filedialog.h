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
    QString selectedName() const;

private slots:
    void slotAccepted();
    void slotOpenListDir();

private:
    Ui::FileDialog *ui;
};

#endif // FILEDIALOG_H
