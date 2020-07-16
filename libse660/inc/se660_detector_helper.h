/*
 * se660_detector_helper.h
 *
 *  Created on: Mar 22, 2019
 *      Author: thinhnguyen
 */

#ifndef SE660_DETECTOR_HELPER_H_
#define SE660_DETECTOR_HELPER_H_
#ifdef __cplusplus
extern "C" {
#endif
#include <libudev.h>

int createScannerDetectorHelper();
void releaseScannerDetectorHelper();
//bloking function
int startDetectScanner(char *scanner_name);
void setScannerVID(const char* vid);
void setScannerPID(const char* pid);
void setDetectorEnableDebug();
void setDetectorDisableDebug();

#ifdef __cplusplus
}
#endif
#endif /* SE660_DETECTOR_HELPER_H_ */
