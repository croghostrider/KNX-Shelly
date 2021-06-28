#pragma once
#include <knx.h>

#define RELAYPIN 4
#define SWINPUTPIN 5

// Parameter with single occurance
#define Heartbeat                 knx.paramInt(0)                    // int32_t
#define StartupDelaySelection     knx.paramByte(4)                   // 1 Bit, Bit 7
#define HeartbeatSelection        knx.paramByte(5)                   // 1 Bit, Bit 7
#define LockWithFalse            (knx.paramByte(6) & 0x80 ) >> 7     // 1 Bit, Bit 7
#define RelaisDisconnected       (knx.paramByte(6) & 0x40 ) >> 6     // 1 Bit, Bit 6
#define LongPress                (knx.paramByte(6) & 0x20 ) >> 5     // 1 Bit, Bit 5
#define InputDefault             (knx.paramByte(6) & 0x10 ) >> 4     // 1 Bit, Bit 4
#define ShortReaction             knx.paramByte(7)                   // 1 Bit, Bit 7
#define LongReaction              knx.paramByte(8)                   // 1 Bit, Bit 7
#define DebounceDelaySelection    knx.paramByte(9)                   // 1 Bit, Bit 7
#define StartupDelay              knx.paramByte(10)                  // 1 Bit, Bit 7             
#define LongPressDelay            knx.paramByte(14)                  // 1 Bit, Bit 7
#define OnReactionSelection       knx.paramByte(18)                  // 1 Bit, Bit 7
#define OffReactionSelection      knx.paramByte(19)                  // 1 Bit, Bit 7


// Communication objects with single occurance
#define KoHeartbeat knx.getGroupObject(1)
#define KoSwitch knx.getGroupObject(2)
#define KoSwitchStatus knx.getGroupObject(3)
#define KoSwitchLock knx.getGroupObject(4)
#define KoBinaryStaus knx.getGroupObject(5)
#define KoBinaryLongStaus knx.getGroupObject(6)

