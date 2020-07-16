/*
 * se660_detector_helper.c
 *
 *  Created on: Mar 22, 2019
 *      Author: thinhnguyen
 */

#include "libse660/inc/se660_detector_helper.h"



#include <stdio.h>
#include <string.h>

static char SCANNER_PID[5];								//	"0197"
static char SCANNER_VID[5]; 							//	"24ea"

#define SUBSYSTEM 									"input"
static int DEBUG_D = 1;

#define DEBUG_PRINT(fmt, ...) \
        do { if (DEBUG_D) printf("%s:%d:%s(): " fmt, __FILE__, \
                                __LINE__, __func__, __VA_ARGS__); } while (0)
struct udev* udev = NULL;

const char *udev_get_properties_value_by_key(const char *key, struct udev_device* dev) {
	struct udev_list_entry *list_entry;
	static char retVal[255] = "\0";
	udev_list_entry_foreach(list_entry, udev_device_get_properties_list_entry(dev)) {
        const char *name = udev_list_entry_get_name(list_entry);
        const char *value = udev_list_entry_get_value(list_entry);
		if (strcmp(name, key) == 0) {
			strncpy(retVal, value, 255);
		}
	}
 return retVal;
}

static int process_device(struct udev_device* dev, char* scanner_name)
{
	int retVal = -1;
	char VID[32] = "\0";
	char PID[32] = "\0";
    if (dev != NULL) {
        if (udev_device_get_devnode(dev)) {
        	strncpy(VID, udev_get_properties_value_by_key("ID_VENDOR_ID", dev), 32);
        	strncpy(PID, udev_get_properties_value_by_key("ID_MODEL_ID", dev), 32);
        	if (strcmp(VID, SCANNER_VID) == 0 && strcmp(PID, SCANNER_PID) == 0) {
        		strcpy(scanner_name, udev_get_properties_value_by_key("DEVNAME", dev));
        		retVal = 0;
        	}
        }
        udev_device_unref(dev);
    }
    return retVal;
}

static int enumerate_devices(char* scanner_name)
{
    struct udev_enumerate* enumerate = udev_enumerate_new(udev);
    int retVal = -1;
    udev_enumerate_add_match_subsystem(enumerate, SUBSYSTEM);
    udev_enumerate_scan_devices(enumerate);

    struct udev_list_entry* devices = udev_enumerate_get_list_entry(enumerate);
    struct udev_list_entry* entry;

    udev_list_entry_foreach(entry, devices) {
        const char* path = udev_list_entry_get_name(entry);
        DEBUG_PRINT("%s\n", path);
        struct udev_device* dev = udev_device_new_from_syspath(udev, path);
        if (process_device(dev, scanner_name) == 0) {
        	retVal = 0;
        	break;
        }
    }

    udev_enumerate_unref(enumerate);
    return retVal;
}

static int monitor_devices(struct udev* udev, char* scanner_name)
{
    struct udev_monitor* mon = udev_monitor_new_from_netlink(udev, "udev");
    int retVal = -1;
    udev_monitor_filter_add_match_subsystem_devtype(mon, SUBSYSTEM, NULL);
    udev_monitor_enable_receiving(mon);

    int fd = udev_monitor_get_fd(mon);
    while (1) {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(fd, &fds);

        int ret = select(fd+1, &fds, NULL, NULL, NULL);
        if (ret <= 0)
            break;

        if (FD_ISSET(fd, &fds)) {
            struct udev_device* dev = udev_monitor_receive_device(mon);
            if (process_device(dev, scanner_name) == 0) {
				retVal = 0;
				break;
			}
        }
    }
    return retVal;
}



int createScannerDetectorHelper()
{
	udev = udev_new();
	if (!udev) {
		DEBUG_PRINT("%s", "udev_new() failed\n");
		return -1;
	}
	return 0;
}
void releaseScannerDetectorHelper() {
	if (udev != NULL)
		udev_unref(udev);
}
void setDetectorEnableDebug() {
	DEBUG_D = 1;
}
void setDetectorDisableDebug() {
	DEBUG_D = 0;
}

int startDetectScanner(char *scanner_name) {
	int retVal = -1;
	if (udev != NULL) {
		if (enumerate_devices(scanner_name) != 0)
		{
			retVal = monitor_devices(udev, scanner_name);
		} else {
			retVal = 0;
		}
	}
	return retVal;
}




void setScannerVID(const char *vid)
{
    strncpy(SCANNER_VID, vid, sizeof (SCANNER_VID));
}

void setScannerPID(const char *pid)
{
    strncpy(SCANNER_PID, pid, sizeof (SCANNER_PID));
}
