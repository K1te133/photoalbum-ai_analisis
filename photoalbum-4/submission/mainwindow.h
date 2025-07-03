#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QDir>
#include "imageanalyzer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void SetPixmap(const QString path) {
        active_pixmap = QPixmap(path);
    }

    void FitImage();
    QString current_folder_;
    void SetFolder(const QString& d);
    int cur_file_index_ = 0;
    QString GetCurrentFile();
    void updateTimer();
    void updateAIText();

private slots:
    void on_btn_left_clicked();
    void slotCustomMenuRequested(QPoint pos);
    void on_btn_right_clicked();
    void on_action_1sec_triggered();
    void on_action_5sec_triggered();
    void on_action_0sec_triggered();
    void on_action_10sec_triggered();
    void dir_triggered();
    void resources();
    void on_close_triggered();
    void action_up_windows_triggered();
    void timer_out() {
        cur_file_index_++;
        int max_images = QDir(current_folder_).entryList().size();
        if (cur_file_index_ == max_images) cur_file_index_ = 0;
        SetPixmap(GetCurrentFile());
        FitImage();
    }

private:
    QLabel lbl_new_{this};
    QTimer timer{this};

    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::MainWindow *ui;
    QPixmap active_pixmap;
    ImageAnalyzer *imageAnalyzer;
};
#endif // MAINWINDOW_H
