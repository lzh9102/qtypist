#include "mainwindow.h"
#include <QSettings>
#include <QMessageBox>
#include <QTimer>
#include "ui_mainwindow.h"
#include "queuedisplay.h"
#include "filedialog.h"
#include "datasource.h"

#define DEFAULT_FONT_SIZE 25
#define MIN_LINE_COUNT 50

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_display(new QueueDisplay(this)),
    m_dataSource(new DataSource(this))
{
    ui->setupUi(this);
    ui->displayLayout->addWidget(m_display);
    ui->mainToolBar->setVisible(false);

    setupEvents();
    loadSettings();
}

MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}

// Handle Return key in the input box
void MainWindow::slotHandleInput()
{
    QString string = ui->txtInput->text().trimmed();
    if (!string.isEmpty() && !m_display->isEmpty()) {
        const bool correct = judgeInput(string);
        if (correct) { // correct input
            ui->txtInput->clear();
            m_display->pop();
        } else { // incorrect input
            ui->txtInput->selectAll();
        }
        updateStatus(correct);
        refillQueue();
    }
}

void MainWindow::slotTextEdited()
{
    if (!m_display->isEmpty() && ui->actionAutoCommit->isChecked()) {
        if (judgeInput(ui->txtInput->text()))
            slotHandleInput();
    }
}

void MainWindow::slotSkip()
{
    if (!m_display->isEmpty()) {
        m_display->pop();
    }
}

void MainWindow::slotUnderline(bool checked)
{
    m_display->setUnderlineFront(checked);
}

void MainWindow::slotWindowLoaded()
{
    openFileDialog();
}

void MainWindow::slotOpenFile()
{
    openFileDialog();
}

void MainWindow::loadSettings()
{
    QSettings settings;

    const int font_size = settings.value("fontsize", DEFAULT_FONT_SIZE).toInt();
    QFont font = ui->txtInput->font();
    font.setPointSize(font_size);
    ui->txtInput->setFont(font);
    m_display->setFontSize(font_size);

    const bool underline = settings.value("underline", true).toBool();
    ui->actionUnderline->setChecked(underline);
    slotUnderline(underline); // guarantee that m_display is updated

    ui->actionAutoCommit->setChecked(settings.value("autocommit", true).toBool());

    restoreGeometry(settings.value("window_geometry").toByteArray());
}

void MainWindow::saveSettings()
{
    QSettings settings;
    settings.setValue("underline", ui->actionUnderline->isChecked());
    settings.setValue("autocommit", ui->actionAutoCommit->isChecked());
    settings.setValue("window_geometry", saveGeometry());
}

void MainWindow::setupEvents()
{
    connect(ui->txtInput, SIGNAL(returnPressed())
            , this, SLOT(slotHandleInput()));
    connect(ui->actionExit, SIGNAL(triggered())
            , this, SLOT(close()));
    connect(ui->actionUnderline, SIGNAL(triggered(bool))
            , this, SLOT(slotUnderline(bool)));
    connect(ui->actionOpen, SIGNAL(triggered())
            , this, SLOT(slotOpenFile()));
    connect(ui->txtInput, SIGNAL(textChanged(QString))
            , this, SLOT(slotTextEdited()));
    connect(ui->actionSkip, SIGNAL(triggered())
            , this, SLOT(slotSkip()));

    // call slotWindowReady() when program enters the event loop
    QTimer::singleShot(0, this, SLOT(slotWindowLoaded()));
}

bool MainWindow::judgeInput(QString string)
{
    return string == m_display->front();
}

void MainWindow::updateStatus(bool correct)
{
    const char *image_path;
    QColor text_color;

    if (correct) {
        image_path = ":/icon/images/correct.png";
        text_color = this->palette().color(QPalette::Text);
    } else {
        image_path = ":/icon/images/incorrect.png";
        text_color = Qt::red;
    }

    // update correct/incorrect icon
    ui->lblStatus->setPixmap(QPixmap(image_path));

    // update inputbox text color
    QPalette palette = ui->txtInput->palette();
    palette.setColor(QPalette::Text, text_color);
    ui->txtInput->setPalette(palette);
}

void MainWindow::openFileDialog()
{
    FileDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        // remove existing phrases and load the new list
        m_dataSource->clear();
        if (!m_dataSource->addFile(dialog.selectedFile())) {
            QMessageBox::critical(this, tr("Error")
                                  , tr("Failed to read file."));
        } else {
            m_display->clear();
            refillQueue();
        }
    }
}

void MainWindow::refillQueue()
{
    if (!m_dataSource->isEmpty()) {
        while (m_display->count() < MIN_LINE_COUNT)
            m_display->push(m_dataSource->next());
    }
}
