#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "usb.h"

static const GUID EMU8000_GUID = {0xBA7BCE35, 0xD219, 0x3195, {0x9F, 0xC2, 0x89, 0x52, 0x8A, 0x5D, 0xE1, 0x0A}};
WINUSB_INTERFACE_HANDLE usb_handle;
PIPE_ID pipe;
UCHAR buf[64];
bool connected = false;
int ch, cc, ccval[16][128], pgm[16];

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusBar->showMessage("Disconnected");
    update_dials();
}

MainWindow::~MainWindow()
{
    if(connected == true)
    {
        buf[0] = USBCONTROL_DISCONNECTED;
        send_config(usb_handle, pipe, buf);
        WinUsb_Free(usb_handle);
        ui->statusBar->showMessage("Disconnected");
    }
    delete ui;
}

void MainWindow::update_dials()
{
    char chbuf[16];
    sprintf_s(chbuf, 16, "CH: %i", ch + 1);
    ui->ch_box->setTitle(chbuf);
    ui->ch_dial->setValue(ch + 1);

    sprintf_s(chbuf, 16, "Vol: %i", ccval[ch][7]);
    ui->vol_box->setTitle(chbuf);
    ui->vol_dial->setValue(ccval[ch][7]);

    sprintf_s(chbuf, 16, "Pan: %i", ccval[ch][10] - 64);
    ui->pan_box->setTitle(chbuf);
    ui->pan_dial->setValue(ccval[ch][10] - 64);

    sprintf_s(chbuf, 16, "Pgm: %i", pgm[ch]);
    ui->pgm_box->setTitle(chbuf);
    ui->pgm_dial->setValue(pgm[ch]);

    sprintf_s(chbuf, 16, "CC: %i", cc);
    ui->cc_box->setTitle(chbuf);
    ui->cc_dial->setValue(cc);

    sprintf_s(chbuf, 16, "Val: %i", ccval[ch][cc]);
    ui->ccval_box->setTitle(chbuf);
    ui->ccval_dial->setValue(ccval[ch][cc]);
}

void MainWindow::on_ch_dial_valueChanged(int value)
{
    ch = value - 1;
    update_dials();
}

void MainWindow::on_vol_dial_valueChanged(int value)
{
    ccval[ch][7] = value;
    update_dials();
}

void MainWindow::on_pan_dial_valueChanged(int value)
{
    ccval[ch][10] = value + 64;
    update_dials();
}

void MainWindow::on_pgm_dial_valueChanged(int value)
{
    pgm[ch] = value;
    update_dials();
}

void MainWindow::on_cc_dial_valueChanged(int value)
{
    cc = value;
    update_dials();
}

void MainWindow::on_ccval_dial_valueChanged(int value)
{
    ccval[ch][cc] = value;
    update_dials();
}

void MainWindow::on_connect_btn_clicked()
{
    if(connected == false)
    {
        HANDLE dev_handle;
        UCHAR speed;
        GetDeviceHandle(EMU8000_GUID, &dev_handle);
        GetWinUSBHandle(dev_handle, &usb_handle);
        GetUSBDeviceSpeed(usb_handle, &speed);
        QueryDeviceEndpoints(usb_handle, &pipe);
        buf[0] = USBCONTROL_CONNECTED;
        if(send_config(usb_handle, pipe, buf) != -1)
        {
            ui->statusBar->showMessage("Connected");
            connected = true;
            buf[0] = USBCONTROL_GET_SETTINGS;
            send_config(usb_handle, pipe, buf);
            ui->master_slider->setValue(buf[1]>>3);
            ui->synth_slider->setValue(buf[5]>>3);
            ui->mbass_slider->setValue(buf[7]);
            ui->mtreble_slider->setValue(buf[8]);
            ui->sbass_slider->setValue(buf[10]);
            ui->streble_slider->setValue(buf[11]);
            ui->reverb_cmbox->setCurrentIndex(buf[12]);
            ui->chorus_cmbox->setCurrentIndex(buf[13]);
            ui->s3d_chbox->setChecked(buf[9]);
        }else{
            ui->statusBar->showMessage("Disconnected");
            connected = false;
        }
    }
}

void MainWindow::on_disconnect_btn_clicked()
{
    if(connected == true)
    {
        buf[0] = USBCONTROL_DISCONNECTED;
        send_config(usb_handle, pipe, buf);
        connected = false;
        ui->statusBar->showMessage("Disconnected");
    }
}

void MainWindow::on_rstset_btn_clicked()
{
    ui->reverb_cmbox->setCurrentIndex(0);
    ui->chorus_cmbox->setCurrentIndex(0);
    ui->s3d_chbox->setChecked(0);
    ui->master_slider->setValue(23);
    ui->synth_slider->setValue(31);
    ui->mbass_slider->setValue(8);
    ui->mtreble_slider->setValue(8);
    ui->sbass_slider->setValue(8);
    ui->streble_slider->setValue(9);
}

void MainWindow::on_reverb_cmbox_currentIndexChanged(int index)
{
    if(connected == true)
    {
        buf[0] = USBCONTROL_SET_EMU8000_FEATURE;
        buf[1] = USBCONTROL_EMU8000_REVERB;
        buf[2] = index;
        send_config(usb_handle, pipe, buf);
    }
}

void MainWindow::on_chorus_cmbox_currentIndexChanged(int index)
{
    if(connected == true)
    {
        buf[0] = USBCONTROL_SET_EMU8000_FEATURE;
        buf[1] = USBCONTROL_EMU8000_CHORUS;
        buf[2] = index;
        send_config(usb_handle, pipe, buf);
    }
}

void MainWindow::on_s3d_chbox_clicked(bool checked)
{
    if(connected == true)
    {
        buf[0] = USBCONTROL_SET_MIXER_FEATURE;
        buf[1] = USBCONTROL_MIXER_3DSE;
        buf[2] = checked;
        send_config(usb_handle, pipe, buf);
    }
}

void MainWindow::on_master_slider_valueChanged(int value)
{
    if(connected == true)
    {
        buf[0] = USBCONTROL_SET_MIXER_FEATURE;
        buf[1] = USBCONTROL_MIXER_MASTER;
        buf[2] = USBCONTROL_MIXER_L;
        buf[3] = value<<3;
        send_config(usb_handle, pipe, buf);
        buf[0] = USBCONTROL_SET_MIXER_FEATURE;
        buf[1] = USBCONTROL_MIXER_MASTER;
        buf[2] = USBCONTROL_MIXER_R;
        buf[3] = value<<3;
        send_config(usb_handle, pipe, buf);
    }
}

void MainWindow::on_synth_slider_valueChanged(int value)
{
    if(connected == true)
    {
        buf[0] = USBCONTROL_SET_MIXER_FEATURE;
        buf[1] = USBCONTROL_MIXER_MIDI;
        buf[2] = USBCONTROL_MIXER_L;
        buf[3] = value<<3;
        send_config(usb_handle, pipe, buf);
        buf[0] = USBCONTROL_SET_MIXER_FEATURE;
        buf[1] = USBCONTROL_MIXER_MIDI;
        buf[2] = USBCONTROL_MIXER_R;
        buf[3] = value<<3;
        send_config(usb_handle, pipe, buf);
    }
}

void MainWindow::on_mbass_slider_valueChanged(int value)
{
    if(connected == true)
    {
        buf[0] = USBCONTROL_SET_MIXER_FEATURE;
        buf[1] = USBCONTROL_MIXER_BASS;
        buf[2] = USBCONTROL_MIXER_L;
        buf[3] = value;
        send_config(usb_handle, pipe, buf);
        buf[0] = USBCONTROL_SET_MIXER_FEATURE;
        buf[1] = USBCONTROL_MIXER_BASS;
        buf[2] = USBCONTROL_MIXER_R;
        buf[3] = value;
        send_config(usb_handle, pipe, buf);
    }
}

void MainWindow::on_mtreble_slider_valueChanged(int value)
{
    if(connected == true)
    {
        buf[0] = USBCONTROL_SET_MIXER_FEATURE;
        buf[1] = USBCONTROL_MIXER_TREBLE;
        buf[2] = USBCONTROL_MIXER_L;
        buf[3] = value;
        send_config(usb_handle, pipe, buf);
        buf[0] = USBCONTROL_SET_MIXER_FEATURE;
        buf[1] = USBCONTROL_MIXER_TREBLE;
        buf[2] = USBCONTROL_MIXER_R;
        buf[3] = value;
        send_config(usb_handle, pipe, buf);
    }
}

void MainWindow::on_sbass_slider_valueChanged(int value)
{
    if(connected == true)
    {
        buf[0] = USBCONTROL_SET_EMU8000_FEATURE;
        buf[1] = USBCONTROL_EMU8000_EQ;
        buf[2] = value;
        buf[3] = ui->streble_slider->value();
        send_config(usb_handle, pipe, buf);
    }
}

void MainWindow::on_streble_slider_valueChanged(int value)
{
    if(connected == true)
    {
        buf[0] = USBCONTROL_SET_EMU8000_FEATURE;
        buf[1] = USBCONTROL_EMU8000_EQ;
        buf[2] = ui->sbass_slider->value();
        buf[3] = value;
        send_config(usb_handle, pipe, buf);
    }
}
