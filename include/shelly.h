#pragma once
#include <knx.h>

#define RELAYPIN 4
#define SWINPUTPIN 5

// Parameter with single occurance
#define StartupDelayAlive         knx.paramInt(0)                     // int32_t
#define StartupDelaySelection     knx.paramWord(4)                    // char*, 2 Byte
#define Heartbeat                 knx.paramWord(6)                    // char*, 2 Byte
#define Lock                     (knx.paramByte(10) & 0x80 ) >> 7     // 1 Bit, Bit 7
#define RelaisDisconnected          (knx.paramByte(10) & 0x40 ) >> 6     // 1 Bit, Bit 6
#define LongPress                (knx.paramByte(10) & 0x20 ) >> 5     // 1 Bit, Bit 5
#define InputDefault             (knx.paramByte(10) & 0x10 ) >> 4     // 1 Bit, Bit 4
#define ShortReaction             knx.paramWord(11)                   // char*, 2 Byte
#define LongReaction              knx.paramWord(13)                   // char*, 2 Byte
#define debounceDelay             knx.paramWord(15)                   // char*, 2 Byte 
#define StartupDelayRestart       knx.paramInt(17)                    // int32_t              
#define LongPressDelay            knx.paramInt(21)                    // int32_t
#define OnReaction                knx.paramInt(25)                    // char*, 2 Byte
#define OffReaction               knx.paramInt(27)                    // char*, 2 Byte



// Communication objects with single occurance
#define KoHeartbeat knx.getGroupObject(1)
#define KoSwitch knx.getGroupObject(2)
#define KoSwitchStatus knx.getGroupObject(3)
#define KoSwitchLock knx.getGroupObject(4)
#define KoBinaryStaus knx.getGroupObject(5)
#define KoBinaryLongStaus knx.getGroupObject(6)

