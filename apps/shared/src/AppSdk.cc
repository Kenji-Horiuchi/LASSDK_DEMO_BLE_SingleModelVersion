//
// Copyright 2023 Sony Semiconductor Solutions Corporation.
//
#include "AppSdkKEY-priv.h"
#include "AsyncTask.h"
#include "base_memory.h"
#define kCNAME "BASE"
#include "AppSdk.h"
#include "GetManager.h"

namespace apps {
namespace shared {
namespace appsdk {

const auto key_boot =
    pst::volatilesetting::key_id_t(PST_STATIC_ID("appsdk.boot"));

const AppSdk::act_tbl_t
AppSdk::action_proc_ = {
  { action_t::kNetEnablePairedDevice, &AppSdk::kick_network },
  { action_t::kNetPairing, &AppSdk::kick_network },
  { action_t::kNetDisable, &AppSdk::kick_network },
  { action_t::kNetRotate, &AppSdk::kick_network },
  { action_t::kMusicPlay, &AppSdk::kick_network },
  { action_t::kMusicPause, &AppSdk::kick_network },
  { action_t::kMusicNext, &AppSdk::kick_network },
  { action_t::kMusicPrev, &AppSdk::kick_network },
  { action_t::kMusicForward, &AppSdk::kick_network },
  { action_t::kMusicRewind, &AppSdk::kick_network },
  { action_t::kVoiceStart, &AppSdk::kick_network },
  { action_t::kVoiceEnd, &AppSdk::kick_network },
  { action_t::kVoiceHold, &AppSdk::kick_network },
  { action_t::kVoiceUnHold, &AppSdk::kick_network },
  { action_t::kAudioUpSpeaker, &AppSdk::kick_audio },
  { action_t::kAudioDownSpeaker, &AppSdk::kick_audio },
  { action_t::kAudioMuteSpeaker, &AppSdk::kick_audio },
  { action_t::kAudioUnMuteSpeaker, &AppSdk::kick_audio },
  { action_t::kAudioUpMic, &AppSdk::kick_audio },
  { action_t::kAudioDownMic, &AppSdk::kick_audio },
  { action_t::kAudioMuteMic, &AppSdk::kick_audio },
  { action_t::kAudioUnMuteMic, &AppSdk::kick_audio },
  { action_t::kReboot, &AppSdk::kick_power },
  { action_t::kSleep, &AppSdk::kick_power },
};
const std::map<event_t,bool>
AppSdk::invalid_event_ = {
  { event_t::kSensorDNNdonot, false },
  { event_t::kSysVoiceRingInternal, false },
};
const AppSdk::hook_tbl_t
AppSdk::hook_event_ = {
  { event_t::kSysVoiceRingInternal, &AppSdk::ring_start },
  { event_t::kVoiceStart, &AppSdk::ring_stop },
  { event_t::kVoiceEnd, &AppSdk::ring_stop },
};

JPVAR(AppSdk::j_event_) =
{
  { event_t::kNetConnect, "kNetConnect" },
  { event_t::kNetDisconnect, "kNetDisconnect" },
  { event_t::kStreamUsed, "kStreamUsed" },
  { event_t::kStreamFree, "kStreamFree" },
  { event_t::kMusicPlay, "kMusicPlay" },
  { event_t::kMusicPause, "kMusicPause" },
  { event_t::kMusicStop, "kMusicStop" },
  { event_t::kMusicSeek, "kMusicSeek" },
  { event_t::kVoiceRing, "kVoiceRing" },
  { event_t::kVoiceStart, "kVoiceStart" },
  { event_t::kVoiceEnd, "kVoiceEnd" },
  { event_t::kVoiceHold, "kVoiceHold" },
  { event_t::kChangeVolumeSpeaker, "kChangeVolumeSpeaker" },
  { event_t::kChangeMuteSpeaker, "kChangeMuteSpeaker" },
  { event_t::kChangeUnMuteSpeaker, "kChangeUnMuteSpeaker" },
  { event_t::kChangeVolumeMic, "kChangeVolumeMic" },
  { event_t::kChangeMuteMic, "kChangeMuteMic" },
  { event_t::kChangeUnMuteMic, "kChangeUnMuteMic" },
  { event_t::kStartGuidance, "kStartGuidance" },
  { event_t::kFinGuidance, "kFinGuidance" },
  { event_t::kChangeAsmOff, "kChangeAsmOff" },
  { event_t::kChangeAsmNoiseCanceling, "kChangeAsmNoiseCanceling" },
  { event_t::kChangeAsmNormal, "kChangeAsmNormal" },
  { event_t::kChangeAsmVoice, "kChangeAsmVoice" },
  { event_t::kKeySingle, "kKeySingle" },
  { event_t::kKeyDouble, "kKeyDouble" },
  { event_t::kKeyTriple, "kKeyTriple" },
  { event_t::kKeyLong, "kKeyLong" },
  { event_t::kKeySingleLong, "kKeySingleLong" },
  { event_t::kKeyDoubleLong, "kKeyDoubleLong" },
  { event_t::kKeyTripleLong, "kKeyTripleLong" },
  { event_t::kKeySingleL, "kKeySingleL" },
  { event_t::kKeyDoubleL, "kKeyDoubleL" },
  { event_t::kKeyTripleL, "kKeyTripleL" },
  { event_t::kKeyLongL, "kKeyLongL" },
  { event_t::kKeySingleLongL, "kKeySingleLongL" },
  { event_t::kKeyDoubleLongL, "kKeyDoubleLongL" },
  { event_t::kKeyTripleLongL, "kKeyTripleLongL" },
  { event_t::kKeySingleR, "kKeySingleR" },
  { event_t::kKeyDoubleR, "kKeyDoubleR" },
  { event_t::kKeyTripleR, "kKeyTripleR" },
  { event_t::kKeyLongR, "kKeyLongR" },
  { event_t::kKeySingleLongR, "kKeySingleLongR" },
  { event_t::kKeyDoubleLongR, "kKeyDoubleLongR" },
  { event_t::kKeyTripleLongR, "kKeyTripleLongR" },
  { event_t::kSensorDNNdonot, "kSensorDNNdonot" },
  { event_t::kSensorDNN1, "kSensorDNN1" },
  { event_t::kSensorDNN2, "kSensorDNN2" },
  { event_t::kSensorDNN3, "kSensorDNN3" },
  { event_t::kSensorDNN4, "kSensorDNN4" },
  { event_t::kSensorDNN5, "kSensorDNN5" },
  { event_t::kSensorDNN6, "kSensorDNN6" },
  { event_t::kSensorDNN7, "kSensorDNN7" },
  { event_t::kSensorDNN8, "kSensorDNN8" },
  { event_t::kSensorDNN9, "kSensorDNN9" },
  { event_t::kSysVoiceRingInternal, "kSysVoiceRingInternal" },
  { event_t::kWearAttached, "kWearAttached" },
  { event_t::kWearDetached, "kWearDetached" },
  { event_t::kCradleEvent1, "kCradleEvent1" },
  { event_t::kCradleEvent2, "kCradleEvent2" },
  { event_t::kCradleEvent3, "kCradleEvent3" },
  { event_t::kCradleEvent4, "kCradleEvent4" },
  { event_t::kCradleEvent5, "kCradleEvent5" },
  { event_t::kCradleEvent6, "kCradleEvent6" },
  { event_t::kCradleEvent7, "kCradleEvent7" },
  { event_t::kCradleEvent8, "kCradleEvent8" },
  { event_t::kCradleEvent9, "kCradleEvent9" },
  { event_t::kCradleConnect, "kCradleConnect" },
  { event_t::kCradleDisconnect, "kCradleDisconnect" },
  { event_t::kBatteryEvent1, "kBatteryEvent1" },
  { event_t::kBatteryEvent2, "kBatteryEvent2" },
  { event_t::kBatteryEvent3, "kBatteryEvent3" },  
  { event_t::kBatteryEvent4, "kBatteryEvent4" },
  { event_t::kBatteryEvent5, "kBatteryEvent5" },
  { event_t::kBatteryEmpty, "kBatteryEmpty" },
};

JPVAR(AppSdk::j_action_) =
{
  { action_t::kClear, "kClear" },
  { action_t::kNetEnablePairedDevice, "kNetEnablePairedDevice" },
  { action_t::kNetPairing, "kNetPairing" },
  { action_t::kNetDisable, "kNetDisable" },
  { action_t::kNetRotate, "kNetRotate" },
  { action_t::kMusicPlay, "kMusicPlay" },
  { action_t::kMusicPause, "kMusicPause" },
  { action_t::kMusicNext, "kMusicNext" },
  { action_t::kMusicPrev, "kMusicPrev" },
  { action_t::kMusicForward, "kMusicForward" },
  { action_t::kMusicRewind, "kMusicRewind" },
  { action_t::kVoiceStart, "kVoiceStart" },
  { action_t::kVoiceEnd, "kVoiceEnd" },
  { action_t::kVoiceHold, "kVoiceHold" },
  { action_t::kVoiceUnHold, "kVoiceUnHold" },
  { action_t::kAudioUpSpeaker, "kAudioUpSpeaker" },
  { action_t::kAudioDownSpeaker, "kAudioDownSpeaker" },
  { action_t::kAudioMuteSpeaker, "kAudioMuteSpeaker" },
  { action_t::kAudioUnMuteSpeaker, "kAudioUnMuteSpeaker" },
  { action_t::kAudioUpMic, "kAudioUpMic" },
  { action_t::kAudioDownMic, "kAudioDownMic" },
  { action_t::kAudioMuteMic, "kAudioMuteMic" },
  { action_t::kAudioUnMuteMic, "kAudioUnMuteMic" },
  { action_t::kReboot, "kReboot" },
  { action_t::kSleep, "kSleep" },
};

AppSdk::AppSdk()
{
  LOGDEBUG();
}

AppSdk::~AppSdk()
{
  LOGDEBUG();
}

easel::status_t
AppSdk::Main(
  const char* const app_name,
  int argc, char** argv
)
{
  pwr_lock_ = app_name;
  return (HeadlessApplicationBase::Main(app_name, argc, argv));
}

void
AppSdk::ToInitialize(
  void
)
{
  LOGDEBUG();
  vol_mgr_ = &easel::GetManager<pst::volatilesetting::VolatileSettingsManager>();
  pwr_mgr_ = &easel::GetManager<pst::power::PowerManager>();

  key_.reset(new AppSdkKEY(
    [this](event_t ev) {
      this->analyze_event(ev);
    }));

  AppSdkNVM::Init(
    [this](event_t ev) {
      this->analyze_event(ev);
    });

  AppSdkPWR::Init(
    [this](event_t ev) {
      this->analyze_event(ev);
    },pwr_lock_);

  if(! AppSdkAUD::Init(
    [this](event_t ev) {
      this->analyze_event(ev);
    })
     ) {
    LOGERR("audio service iniitialize error");
  }

  if(! AppSdkNET::Init(
    [this](event_t ev) {
      this->analyze_event(ev);
    })
     ) {
    LOGERR("network service iniitialize error");
  }

  AppSdkSNS::Init(
    [this](event_t ev) {
      this->analyze_event(ev);
    });

  AppSdkCDL::Init(
    [this](event_t ev) {
      this->analyze_event(ev);
    });

  AppSdkBAT::Init(
    [this](event_t ev) {
      this->analyze_event(ev);
    });

  this->Boot();
}

void
AppSdk::ToActivate(
  void
)
{
  LOGDEBUG();
  is_active_ = true;

  this->Run();

  bool b = false;
  vol_mgr_->Get(key_boot, &b);
  if(!b){
    vol_mgr_->Set(key_boot, true);
    pwr_mgr_->FinishBootup();
  }
}

void
AppSdk::ToWfe(
  void
)
{
  LOGDEBUG();

  is_active_ = false;
}

void
AppSdk::ToRun(
  void
)
{
  LOGDEBUG();

  is_active_ = true;
}

bool
AppSdk::ToInactivate(
  void
)
{
  LOGDEBUG();

  this->Term();

  this->AppSdkAUD::Term();
  this->AppSdkNET::Term();
  this->AppSdkSNS::Term();
  this->AppSdkCDL::Term();
  this->AppSdkBAT::Term();
  this->AppSdkPWR::Term();
  this->AppSdkNVM::Term();

  return(true);
}

void
AppSdk::ToFinalize(
  void
)
{
  LOGDEBUG();

  is_active_ = false;
}

bool
AppSdk::OnKeyEvent(
  pst::key::keycode_t keycode
)
{
  auto result = false;

  if(key_) {
    result = key_->CheckEvent(keycode);
  }

  return(result);
}

status_t
AppSdk::FactoryReset(
  void
)
{
  auto result = status_t::kInvalidAPI;

  result = AppSdkNET::RemoveConnectionHistory();

  if(result==status_t::kSuccess){
    result = AppSdkNVM::ResetAllNvmValues();
    if(result==status_t::kSuccess){
      AppSdkPWR::Reboot();
    }
  }

  return (result);
}

status_t
AppSdk::SetEventToAction(
  event_t event,
  action_t act
)
{
  auto result = status_t::kInvalidAPI;

  if(invalid_event_.count(event)) {
    LOGWARN("invalid event %s(%d)", JP(j_event_, event));
    return(status_t::kInvalidParameter);
  }

  if(j_event_.count(event)) {
    if(j_action_.count(act)) {
      if(act == action_t::kClear) {
        LOGDEBUG("clear %s(%d)", JP(j_event_, event));
        event_to_action_.erase(event);
      } else {
        LOGDEBUG("%s(%d)=%s(%d)", JP(j_event_, event), JP(j_action_, act));
        event_to_action_[event] = act;
      }
      result = status_t::kSuccess;
    } else {
      LOGWARN("invalid action %d", act);
      result = status_t::kInvalidParameter;
    }
  } else {
    LOGWARN("invalid event %d", event);
    result = status_t::kInvalidParameter;
  }

  return(result);
}

status_t
AppSdk::SetEventToCallback(
  event_t event,
  EventNotify func
)
{
  auto result = status_t::kInvalidAPI;

  if(invalid_event_.count(event)) {
    LOGWARN("invalid event %s(%d)", JP(j_event_, event));
    return(status_t::kInvalidParameter);
  }

  if(j_event_.count(event)) {
    LOGDEBUG("%s=%s(%d)", func ? "hook" : "unhook", JP(j_event_, event));
    if(func == nullptr) {
      event_to_proc_.erase(event);
    } else {
      event_to_proc_[event] = func;
    }
    result = status_t::kSuccess;
  } else {
    LOGWARN("invalid event %d", event);
    result = status_t::kInvalidParameter;
  }

  return(result);
}

void
AppSdk::analyze_event(
  event_t ev
)
{
  if(hook_event_.count(ev)) {
    ev = (this->*hook_event_.at(ev))(ev);
  }

  if(is_active_) {
    easel::SubmitDetachedAsyncTask(
      [this, ev]() {
        if(event_to_action_.count(ev)) {
          auto act = event_to_action_.at(ev);
          LOGDEBUG("exec ev=%s(%d) to act=%s(%d)", JP(j_event_, ev), JP(j_action_, act));
          (this->*action_proc_.at(act))(act);
        }

        if(event_to_proc_.count(ev)) {
          LOGDEBUG("exec ev=%s(%d) to call", JP(j_event_, ev));
          (event_to_proc_.at(ev))(ev);
        }
      });
  } else {
    LOGWARN("execution restricted ev:%s(%d)\n", JP(j_event_, ev));
  }
}

void
AppSdk::kick_network(
  action_t act
)
{
  LOGDEBUG("act=%s(%d)\n", JP(j_action_, act));
  this->AppSdkNET::ExecAction(act);
}

void
AppSdk::kick_audio(
  action_t act
)
{
  LOGDEBUG("act=%s(%d)\n", JP(j_action_, act));
  this->AppSdkAUD::ExecAction(act);
}

event_t
AppSdk::ring_start(
  event_t
)
{
  LOGDEBUG();

  this->AppSdkAUD::RingControl(true);

  return(event_t::kVoiceRing);
}

event_t
AppSdk::ring_stop(
  event_t ev
)
{
  LOGDEBUG();

  this->AppSdkAUD::RingControl(false);

  return(ev);
}

void
AppSdk::kick_power(
  action_t act
)
{
  LOGDEBUG("act=%s(%d)\n", JP(j_action_, act));
  if(act == action_t::kReboot){
    AppSdkPWR::Reboot();
  }else if(act == action_t::kSleep){
    AppSdkPWR::Sleep();
  }
}

}  // namespace appsdk
}  // namespace shared
}  // namespace apps
