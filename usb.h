#ifndef USB_H
#define USB_H

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <strsafe.h>

#include <winusb.h>
#include <Usb100.h>
#include <Setupapi.h>

#define USBCONTROL_RESPONSE				0xDD

#define USBCONTROL_CONNECTED			1
#define USBCONTROL_DISCONNECTED			2
#define USBCONTROL_SET_MIXER_FEATURE	3
#define USBCONTROL_SET_EMU8000_FEATURE	4
#define USBCONTROL_GET_SETTINGS			5

#define USBCONTROL_MIXER_L				1
#define USBCONTROL_MIXER_R				2

#define USBCONTROL_MIXER_MASTER			1
#define USBCONTROL_MIXER_VOICE			2
#define USBCONTROL_MIXER_MIDI			3
#define USBCONTROL_MIXER_BASS			4
#define USBCONTROL_MIXER_TREBLE			5
#define USBCONTROL_MIXER_3DSE			6

#define USBCONTROL_EMU8000_EQ			1
#define USBCONTROL_EMU8000_REVERB		2
#define USBCONTROL_EMU8000_CHORUS		3

struct PIPE_ID {
    UCHAR  PipeInId;
    UCHAR  PipeOutId;
};

BOOL GetDeviceHandle(GUID guidDeviceInterface, PHANDLE hDeviceHandle);
BOOL GetWinUSBHandle(HANDLE hDeviceHandle, PWINUSB_INTERFACE_HANDLE phWinUSBHandle);
BOOL GetUSBDeviceSpeed(WINUSB_INTERFACE_HANDLE hDeviceHandle, UCHAR* pDeviceSpeed);
BOOL QueryDeviceEndpoints(WINUSB_INTERFACE_HANDLE hDeviceHandle, PIPE_ID* pipeid);
ULONG send_config(WINUSB_INTERFACE_HANDLE h, PIPE_ID p, PUCHAR bf);

#endif // USB_H
