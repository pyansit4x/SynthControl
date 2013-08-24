#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void update_dials(void);

    void on_ch_dial_valueChanged(int value);

    void on_vol_dial_valueChanged(int value);

    void on_pan_dial_valueChanged(int value);

    void on_pgm_dial_valueChanged(int value);

    void on_cc_dial_valueChanged(int value);

    void on_ccval_dial_valueChanged(int value);

    void on_connect_btn_clicked();

    void on_disconnect_btn_clicked();

    void on_rstset_btn_clicked();

    void on_reverb_cmbox_currentIndexChanged(int index);

    void on_chorus_cmbox_currentIndexChanged(int index);

    void on_s3d_chbox_clicked(bool checked);

    void on_master_slider_valueChanged(int value);

    void on_synth_slider_valueChanged(int value);

    void on_mbass_slider_valueChanged(int value);

    void on_mtreble_slider_valueChanged(int value);

    void on_sbass_slider_valueChanged(int value);

    void on_streble_slider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
