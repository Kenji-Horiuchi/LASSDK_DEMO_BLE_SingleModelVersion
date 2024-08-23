//
// Copyright 2023 Sony Semiconductor Solutions Corporation.
//
#ifndef APP_SHARED_INCLUDE_APPSDKTYPE_H_
#define APP_SHARED_INCLUDE_APPSDKTYPE_H_

#include <cstdint>
#include "base_functional.h"

namespace apps {
namespace shared {
namespace appsdk {

enum class
status_t
{
  kSuccess,
  kInvalidParameter,
  kInvalidAPI,
  kModelNotFound,
  kModelRegistered,
  kNotRegist,
  kCradleRegistered,
  kBatteryLevelRegistered,
  // AUDIO
  kAUDIO_kAlreadyRegist,
  kAUDIO_kNotRegist,
  kAUDIO_kFatal,
  kAUDIO_kInvalidParam,
  kAUDIO_kInvalidState,
  kAUDIO_kCantSync,
  kAUDIO_kRejectPlay,
  kAUDIO_kAlreadyChanged,
  // NETWORK
  kNET_kInvalidParam,
  kNET_kNotSwitched,
  kNET_kConnectionStopped,
  kNET_kNoConnecting,
  kNET_kNoDeviceInfo,
  kNET_kQueueFull,
  kNET_kInvalidState,
  kNET_kNotSupport,
  kNET_kBufferFull,
  // SENSOR
  kSNS_kInternal,
  kSNS_kInvalidState,
  kSNS_kInvalid,
  // CRADLE
  kCDL_kInternal,
  kCDL_kInvalid,
  kCDL_kInvalidState,
  // POWER
  kPWR_kInvalid,
  kPWR_kInvalidState,
  kPWR_kInternalError,
  // NVM
  kNVM_kNotFound,
  kNVM_kInvalid,
  kNVM_kInvalidState,
  kNVM_kExceedMaxLength,
  kNVM_kInternalError,
  kNVM_kStorageCorrupt,
  kNVM_kAlreadyExists,
  // BATTERY
  kBAT_kInternalError,
};

enum class
event_t
{
  kNetConnect,
  kNetDisconnect,
  kStreamUsed,
  kStreamFree,
  kMusicPlay,
  kMusicPause,
  kMusicStop,
  kMusicSeek,
  kVoiceRing,
  kVoiceStart,
  kVoiceEnd,
  kVoiceHold,
  kChangeVolumeSpeaker,
  kChangeMuteSpeaker,
  kChangeUnMuteSpeaker,
  kChangeVolumeMic,
  kChangeMuteMic,
  kChangeUnMuteMic,
  kStartGuidance,
  kFinGuidance,
  kChangeAsmOff,
  kChangeAsmNoiseCanceling,
  kChangeAsmNormal,
  kChangeAsmVoice,
  kKeySingle,
  kKeyDouble,
  kKeyTriple,
  kKeyLong,
  kKeySingleLong,
  kKeyDoubleLong,
  kKeyTripleLong,
  kKeySingleL,
  kKeyDoubleL,
  kKeyTripleL,
  kKeyLongL,
  kKeySingleLongL,
  kKeyDoubleLongL,
  kKeyTripleLongL,
  kKeySingleR,
  kKeyDoubleR,
  kKeyTripleR,
  kKeyLongR,
  kKeySingleLongR,
  kKeyDoubleLongR,
  kKeyTripleLongR,
  kSensorDNNdonot,
  kSensorDNN1,
  kSensorDNN2,
  kSensorDNN3,
  kSensorDNN4,
  kSensorDNN5,
  kSensorDNN6,
  kSensorDNN7,
  kSensorDNN8,
  kSensorDNN9,
  kSysVoiceRingInternal,
  kWearAttached,
  kWearDetached,
  kCradleEvent1,
  kCradleEvent2,
  kCradleEvent3,
  kCradleEvent4,
  kCradleEvent5,
  kCradleEvent6,
  kCradleEvent7,
  kCradleEvent8,
  kCradleEvent9,
  kCradleEventdonot,
  kCradleInvalidFdCommand,
  kCradleConnect,
  kCradleDisconnect,
  kBatteryEvent1,
  kBatteryEvent2,
  kBatteryEvent3,
  kBatteryEvent4,
  kBatteryEvent5,
  kBatteryEventdonot,
  kBatteryEmpty,
};

enum class
action_t
{
  kClear,
  kNetEnablePairedDevice,
  kNetPairing,
  kNetDisable,
  kNetRotate,
  kMusicPlay,
  kMusicPause,
  kMusicNext,
  kMusicPrev,
  kMusicForward,
  kMusicRewind,
  kVoiceStart,
  kVoiceEnd,
  kVoiceHold,
  kVoiceUnHold,
  kAudioUpSpeaker,
  kAudioDownSpeaker,
  kAudioMuteSpeaker,
  kAudioUnMuteSpeaker,
  kAudioToggleMuteSpeaker,
  kAudioUpMic,
  kAudioDownMic,
  kAudioMuteMic,
  kAudioUnMuteMic,
  kAudioToggleMuteMic,
  kReboot,
  kSleep,
};

using EventNotify = pst::base::function<void(event_t)>;

}  // namespace appsdk
}  // namespace shared
}  // namespace apps

#endif  // APP_SHARED_INCLUDE_APPSDKTYPE_H_
