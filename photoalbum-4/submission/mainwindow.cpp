#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QTimer>


QPixmap ResizeImgToFit(const QPixmap &src, int window_width, int window_height) {
    int img_w = src.width();
    int img_h = src.height();

    double w_ratio = double(img_w) / window_width;
    double h_ratio = double(img_h) / window_height;

    if (w_ratio < h_ratio) {
        return src.scaledToWidth(window_width);
    } else {
        return src.scaledToHeight(window_height);
    }
}

QString MainWindow::GetCurrentFile() {
    QDir dir(current_folder_);
    QStringList filters = {"*.jpg", "*.jpeg", "*.png", "*.bmp", "*.gif"};
    QStringList list_files = dir.entryList(filters, QDir::Files | QDir::NoDotAndDotDot);

    if (list_files.isEmpty()) {
        ui->btn_left->setDisabled(true);
        ui->btn_right->setDisabled(true);
        lbl_new_.clear();
        return QString();
    } else {
        ui->btn_left->setEnabled(true);
        ui->btn_right->setEnabled(true);
    }

    if (cur_file_index_ >= list_files.size()) cur_file_index_ = 0;
    if (cur_file_index_ < 0) cur_file_index_ = list_files.size() - 1;

    return dir.filePath(list_files[cur_file_index_]);
}

void MainWindow::SetFolder(const QString& d) {
    current_folder_ = d;
    cur_file_index_ = 0;
    SetPixmap(GetCurrentFile());
    FitImage();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , imageAnalyzer(new ImageAnalyzer())
{
    qDebug() << "MainWindow: Конструктор запущен в" << QDateTime::currentDateTime().toString();
    ui->setupUi(this);
    SetFolder(":/cats/images/");
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    // Прячем основное меню
    ui->menuBar->hide();
    // Разрешаем вызов контекстного меню
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QMainWindow::customContextMenuRequested, this, &MainWindow::slotCustomMenuRequested);

    connect(ui->action_choose_dir, &QAction::triggered, this, &MainWindow::dir_triggered);
    connect(&timer, &QTimer::timeout, this, &MainWindow::timer_out);
    connect(ui->action_1sec, &QAction::triggered, this, &MainWindow::on_action_1sec_triggered);
    connect(ui->action_5sec, &QAction::triggered, this, &MainWindow::on_action_5sec_triggered);
    connect(ui->action_10sec, &QAction::triggered, this, &MainWindow::on_action_10sec_triggered);
    connect(ui->action_0sec, &QAction::triggered, this, &MainWindow::on_action_0sec_triggered);
    connect(ui->action_use_resources, &QAction::triggered, this, &MainWindow::resources);
    connect(ui->action_up_windows, &QAction::triggered, this, &MainWindow::action_up_windows_triggered);
    connect(ui->close, &QAction::triggered, this, &MainWindow::on_close_triggered);

    // Пытаемся загрузить модель
    QString modelPath = "C:/Users/aaukh/Downloads/model.onnx";
    QFileInfo modelFile(modelPath);
    qDebug() << "Текущая рабочая директория:" << QDir::currentPath();
    qDebug() << "Проверка пути к модели:" << modelPath << ", существует:" << modelFile.exists();
    if (!imageAnalyzer->loadModel(modelPath)) {
        qDebug() << "Ошибка загрузки модели AI.";
    } else {
        qDebug() << "Модель AI успешно загружена.";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete imageAnalyzer; // Очищаем объект AI
}

void MainWindow::FitImage()
{
    QPixmap l = ResizeImgToFit(active_pixmap, width(), height());
    lbl_new_.setPixmap(l);
    lbl_new_.resize(l.width(), l.height());
    int lbl_x = (width() - l.width()) / 2;
    int lbl_y = (height() - l.height()) / 2;
    lbl_new_.move(lbl_x, lbl_y);

    // Выполняем AI-анализ, если модель загружена
    if (imageAnalyzer->isModelLoaded()) {
        QImage img = active_pixmap.toImage();
        if (imageAnalyzer->isImageAIDetected(img)) {
            qDebug() << "AI-анализ: Вероятность сгенерированного изображения:"
                     << imageAnalyzer->getAIProbability();
            update();
        } else {
            qDebug() << "AI-анализ: Не удалось выполнить (ошибка обработки).";
        }
    }
    updateAIText();
}

void MainWindow::resizeEvent(QResizeEvent*)
{
    FitImage();
}

void MainWindow::on_btn_left_clicked()
{
    QDir dir(current_folder_);
    QStringList filters = {"*.jpg", "*.jpeg", "*.png", "*.bmp", "*.gif"};
    QStringList list_files = dir.entryList(filters, QDir::Files | QDir::NoDotAndDotDot);

    if (list_files.isEmpty()) {
        return;
    }

    cur_file_index_--;

    if (cur_file_index_ < 0) {
        cur_file_index_ = list_files.size() - 1;
    }

    SetPixmap(GetCurrentFile());
    FitImage();

    ui->action_0sec->setChecked(true);
    on_action_0sec_triggered();
}

void MainWindow::on_btn_right_clicked()
{
    cur_file_index_++;
    int max_images = QDir(current_folder_).entryList().size();
    if (cur_file_index_ >= max_images) cur_file_index_ = 0;
    SetPixmap(GetCurrentFile());
    FitImage();

    ui->action_0sec->setChecked(true);
    on_action_0sec_triggered();
}

void MainWindow::slotCustomMenuRequested(QPoint pos)
{
    ui->menu->popup(this->mapToGlobal(pos));
}

void MainWindow::on_action_1sec_triggered()
{
    ui->action_5sec->setChecked(false);
    ui->action_10sec->setChecked(false);
    ui->action_0sec->setChecked(false);
    updateTimer();
}

void MainWindow::on_action_5sec_triggered()
{
    ui->action_1sec->setChecked(false);
    ui->action_10sec->setChecked(false);
    ui->action_0sec->setChecked(false);
    updateTimer();
}

void MainWindow::on_action_10sec_triggered()
{
    ui->action_1sec->setChecked(false);
    ui->action_5sec->setChecked(false);
    ui->action_0sec->setChecked(false);
    updateTimer();
}

void MainWindow::on_action_0sec_triggered()
{
    ui->action_1sec->setChecked(false);
    ui->action_5sec->setChecked(false);
    ui->action_10sec->setChecked(false);
    updateTimer();
}

void MainWindow::dir_triggered()
{
    QString dir = QFileDialog::getExistingDirectory(this, QString("Открыть папку"), QDir::currentPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    SetFolder(dir);
    ui->action_use_resources->setChecked(false);
    ui->action_choose_dir->setChecked(true);
}

void MainWindow::on_close_triggered()
{
    this->close();
}

void MainWindow::action_up_windows_triggered()
{
    Qt::WindowFlags flags = windowFlags();
    if (ui->action_up_windows->isChecked()) {
        flags |= Qt::WindowStaysOnTopHint;
    } else {
        flags &= ~Qt::WindowStaysOnTopHint;
    }
    setWindowFlags(flags);
    show();
}

void MainWindow::updateTimer()
{
    int n = 0;

    if (ui->action_1sec->isChecked()) {
        n = 1;
    } else if (ui->action_5sec->isChecked()) {
        n = 5;
    } else if (ui->action_10sec->isChecked()) {
        n = 10;
    }

    if (n > 0) {
        timer.start(n * 1000);
    } else {
        timer.stop();
    }
}

void MainWindow::resources()
{
    SetFolder(":/cats/images/");
    ui->action_choose_dir->setChecked(false);
    ui->action_use_resources->setChecked(true);
}

void MainWindow::updateAIText() {
    if (imageAnalyzer->isModelLoaded() && imageAnalyzer->getAIProbability() > 0.0f) {
        float probability = imageAnalyzer->getAIProbability();
        QString text;
        if (probability > 0.55) {
            text = "Фото не сгенерировано";
        } else if (probability < 0.45) {
            text = "Фото сгенерировано";
        } else {
            text = "Модель не способна оценить это фото";
        }
        ui->ai_res_out->setStyleSheet("color: gray; font-size: 12px; font-weight: bold;");
        ui->ai_res_out->setText(text);
    }
}
