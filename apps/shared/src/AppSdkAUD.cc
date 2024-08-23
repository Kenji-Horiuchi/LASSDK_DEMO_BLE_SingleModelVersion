//
// Copyright 2023 Sony Semiconductor Solutions Corporation.
//
#include "PstSystemProperty.h"
#include "GetManager.h"
#define kCNAME "AUD"
#include "AppSdkAUD.h"

namespace apps {
namespace shared {
namespace appsdk {

const AppSdkAUD::act_proc_t
AppSdkAUD::action_proc_ = {
  { action_t::kAudioUpSpeaker, &AppSdkAUD::kick_act_relv },
  { action_t::kAudioDownSpeaker, &AppSdkAUD::kick_act_relv },
  { action_t::kAudioUpMic, &AppSdkAUD::kick_act_relv },
  { action_t::kAudioDownMic, &AppSdkAUD::kick_act_relv },
  { action_t::kAudioMuteSpeaker, &AppSdkAUD::kick_act_mute },
  { action_t::kAudioUnMuteSpeaker, &AppSdkAUD::kick_act_mute },
  { action_t::kAudioToggleMuteSpeaker, &AppSdkAUD::kick_act_mute },
  { action_t::kAudioMuteMic, &AppSdkAUD::kick_act_mute },
  { action_t::kAudioUnMuteMic, &AppSdkAUD::kick_act_mute },
  { action_t::kAudioToggleMuteMic, &AppSdkAUD::kick_act_mute },
};

const AppSdkAUD::act_relv_t
AppSdkAUD::action_to_relv_ = {
  { action_t::kAudioUpSpeaker,     { vm_t::kSpeaker, 1 } },
  { action_t::kAudioDownSpeaker,   { vm_t::kSpeaker, -1 } },
  { action_t::kAudioUpMic,         { vm_t::kMic, 1 } },
  { action_t::kAudioDownMic,       { vm_t::kMic, -1 } },
};
const AppSdkAUD::act_mute_t
AppSdkAUD::action_to_mute_ = {
  { action_t::kAudioMuteSpeaker,       { vm_t::kSpeaker, pst::audio::mute_sw_t::kMute } },
  { action_t::kAudioUnMuteSpeaker,     { vm_t::kSpeaker, pst::audio::mute_sw_t::kUnMute } },
  { action_t::kAudioToggleMuteSpeaker, { vm_t::kSpeaker, pst::audio::mute_sw_t::kToggle } },
  { action_t::kAudioMuteMic,           { vm_t::kMic, pst::audio::mute_sw_t::kMute } },
  { action_t::kAudioUnMuteMic,         { vm_t::kMic, pst::audio::mute_sw_t::kUnMute } },
  { action_t::kAudioToggleMuteMic,     { vm_t::kMic, pst::audio::mute_sw_t::kToggle } },
};
const AppSdkAUD::vdev_event_t
AppSdkAUD::vdev_to_event_ = {
  { vm_t::kSpeaker, event_t::kChangeVolumeSpeaker },
  { vm_t::kMic, event_t::kChangeVolumeMic },
};
const AppSdkAUD::mute_event_t
AppSdkAUD::mute_to_event_ = {
  { { vm_t::kSpeaker, true },  event_t::kChangeMuteSpeaker },
  { { vm_t::kSpeaker, false }, event_t::kChangeUnMuteSpeaker },
  { { vm_t::kMic, true },  event_t::kChangeMuteMic },
  { { vm_t::kMic, false }, event_t::kChangeUnMuteMic },
};
const AppSdkAUD::relv_tbl_t
AppSdkAUD::init_step_volume_ = {
  { vm_t::kSpeaker, DEFAULT_VOLUME_STEP },
  { vm_t::kMic, DEFAULT_VOLUME_STEP },
};
const AppSdkAUD::eff_mix_t
AppSdkAUD::eff_mix_tbl_ = {
  { effect_mix_t::kDown, pst::audio::guide_mix_t::kDown },
  { effect_mix_t::kOverlap, pst::audio::guide_mix_t::kOverlap },
  { effect_mix_t::kMute, pst::audio::guide_mix_t::kMute },
};
const AppSdkAUD::eff_pri_t
AppSdkAUD::eff_pri_tbl_ = {
  { effect_pri_t::kLow, pst::audio::guide_priority_t::kUser4 },
  { effect_pri_t::kHigh, pst::audio::guide_priority_t::kUser3 },
};
const AppSdkAUD::vctrl_act_t
AppSdkAUD::vctrl_to_act_ = {
  { volume_ctrl_t::kUp,     action_t::kAudioUpSpeaker },
  { volume_ctrl_t::kDown,   action_t::kAudioDownSpeaker },
  { volume_ctrl_t::kMute,   action_t::kAudioMuteSpeaker },
  { volume_ctrl_t::kUnMute, action_t::kAudioUnMuteSpeaker },
  { volume_ctrl_t::kToggle, action_t::kAudioToggleMuteSpeaker },
};
const AppSdkAUD::vctrl_act_t
AppSdkAUD::mctrl_to_act_ = {
  { volume_ctrl_t::kUp,     action_t::kAudioUpMic },
  { volume_ctrl_t::kDown,   action_t::kAudioDownMic },
  { volume_ctrl_t::kMute,   action_t::kAudioMuteMic },
  { volume_ctrl_t::kUnMute, action_t::kAudioUnMuteMic },
  { volume_ctrl_t::kToggle, action_t::kAudioToggleMuteMic },
};
const AppSdkAUD::gstate_to_event_t
AppSdkAUD::gstate_to_event_ =
{
  { pst::audio::guide_state_t::kStart, event_t::kStartGuidance },
  { pst::audio::guide_state_t::kFinish, event_t::kFinGuidance },
};
const AppSdkAUD::eff_sync_tbl_t
AppSdkAUD::eff_sync_tbl_ =
{
  { effect_target_t::kMyself , false },
  { effect_target_t::kSync , true },
  { effect_target_t::kLeft , true },
  { effect_target_t::kRight , true },
};

const AppSdkAUD::asm_event_t
AppSdkAUD::asm_to_event_ = {
  { pst::audio::as_mode_t::kOff, event_t::kChangeAsmOff },
  { pst::audio::as_mode_t::kNoiseCanceling, event_t::kChangeAsmNoiseCanceling },
  { pst::audio::as_mode_t::kAmbientSoundModeNormal, event_t::kChangeAsmNormal },
  { pst::audio::as_mode_t::kAmbientSoundModeVoice, event_t::kChangeAsmVoice },
};

const AppSdkAUD::chk_st_t
AppSdkAUD::chk_status_ = {
  { st_t::kSuccess,
    { "kSuccess", status_t::kSuccess } },
  { st_t::kSuccessAsync,
    { "kSuccessAsync", status_t::kSuccess } },
  { st_t::kAlreadyRegist,
    { "kAlreadyRegist", status_t::kAUDIO_kAlreadyRegist } },
  { st_t::kNotRegist,
    { "kNotRegist", status_t::kAUDIO_kNotRegist } },
  { st_t::kFatal,
    { "kFatal", status_t::kAUDIO_kFatal } },
  { st_t::kInvalidParam,
    { "kInvalidParam", status_t::kAUDIO_kInvalidParam } },
  { st_t::kInvalidState,
    { "kInvalidState", status_t::kAUDIO_kInvalidState } },
  { st_t::kCantSync,
    { "kCantSync", status_t::kAUDIO_kCantSync } },
  { st_t::kRejectPlay,
    { "kRejectPlay", status_t::kAUDIO_kRejectPlay } },
};
JPVAR(AppSdkAUD::j_vm_) =
{
  { vm_t::kSpeaker, "kSpeaker" },
  { vm_t::kMic, "kMic" },
  { vm_t::kSidetone, "kSidetone" },
  { vm_t::kSeamless, "kSeamless" },
};
JPVAR(AppSdkAUD::j_vctrl_) =
{
  { volume_ctrl_t::kUp, "kUp" },
  { volume_ctrl_t::kDown, "kDown" },
  { volume_ctrl_t::kMute, "kMute" },
  { volume_ctrl_t::kUnMute, "kUnMute" },
  { volume_ctrl_t::kToggle, "kToggle" },
};
JPVAR(AppSdkAUD::j_vstate_) =
{
  { pst::audio::volume_state_t::kChangeVolume, "kChangeVolume" },
  { pst::audio::volume_state_t::kChangeMute, "kChangeMute" },
};
JPVAR(AppSdkAUD::j_gstate_) =
{
  { pst::audio::guide_state_t::kStart, "kStart" },
  { pst::audio::guide_state_t::kFinish, "kFinish" },
};
JPVAR(AppSdkAUD::j_gcause_) =
{
  { pst::audio::guide_cause_t::kMyself, "kMyself" },
  { pst::audio::guide_cause_t::kBoth, "kBoth" },
  { pst::audio::guide_cause_t::kFinished, "kFinished" },
  { pst::audio::guide_cause_t::kCanceled, "kCanceled" },
  { pst::audio::guide_cause_t::kDecodeError, "kDecodeError" },
  { pst::audio::guide_cause_t::kTooLate, "kTooLate" },
};
JPVAR(AppSdkAUD::j_asm_) =
{
  { pst::audio::as_mode_t::kOff, "kOff" },
  { pst::audio::as_mode_t::kNoiseCanceling, "kNoiseCanceling" },
  { pst::audio::as_mode_t::kAmbientSoundModeNormal, "kAmbientSoundModeNormal" },
  { pst::audio::as_mode_t::kAmbientSoundModeVoice, "kAmbientSoundModeVoice" },
};

AppSdkAUD::AppSdkAUD(void)
{
  step_volume_ = init_step_volume_;
}

bool
AppSdkAUD::Init(
  EventNotify proc
)
{
  LOGDEBUG();

  mgr_ = &easel::GetManager<pst::audio::AudioClient>();
  if(status_t::kSuccess != CHK(mgr_->RegisterObserver(this))) {
    LOGERR("regist error");
    return(false);
  }

  event_proc_ = proc;

  eff_target_tbl_.emplace(effect_target_t::kMyself ,pst::audio::guide_target_t::kMyself);
  eff_target_tbl_.emplace(effect_target_t::kSync ,pst::audio::guide_target_t::kBoth);

  if(! get_position()) {
    LOGWARN("read position fail");
  }
  if(ch_ && *ch_ == kLeft) {
    eff_target_tbl_.emplace(effect_target_t::kLeft ,pst::audio::guide_target_t::kMyself);
  } else {
    eff_target_tbl_.emplace(effect_target_t::kRight ,pst::audio::guide_target_t::kMyself);
  }

  return(true);
}

void
AppSdkAUD::Term(
  void
)
{
  LOGDEBUG();

  if(mgr_) {
    CHK(mgr_->DeregisterObserver(this));
  }
}

status_t
AppSdkAUD::ExecAction(
  action_t act
) {
  LOGDEBUG("act=(%d)", act);

  auto result = status_t::kInvalidAPI;

  if(action_proc_.count(act)) {
    result = (this->*action_proc_.at(act))(act);
  } else {
    LOGDEBUG("no entry");
    result = status_t::kInvalidParameter;
  }

  return(result);
}

status_t
AppSdkAUD::ChangeVolume(
  vm_t dev,
  uint8_t vol
)
{
  LOGINFO("dev:%s(%d) vol:%d", JP(j_vm_, dev), vol);

  auto result = status_t::kInvalidAPI;

  pst::audio::volume_control_t ctrl(vol);
  ctrl.Device(dev);
  result = CHK(mgr_->SetVolume(ctrl));

  return(result);
}

status_t
AppSdkAUD::ControlVolume(
  volume_ctrl_t ctrl,
  const vctrl_act_t& tbl
)
{
  LOGINFO("ctrl:%s(%d)", JP(j_vctrl_, ctrl));

  auto result = status_t::kInvalidAPI;

  if(tbl.count(ctrl)) {
    auto act = tbl.at(ctrl);
    result = (this->*action_proc_.at(act))(act);
  } else {
    result = status_t::kInvalidParameter;
  }

  return(result);
}

status_t
AppSdkAUD::SetStep(
  vm_t dev,
  uint8_t step
)
{
  LOGINFO("dev:%s(%d) step:%d", JP(j_vm_, dev), step);

  auto result = status_t::kInvalidAPI;
  if(step >= pst::audio::kMinVolumeStep && step <= pst::audio::kMaxVolumeStep) {
    step_volume_[dev] = static_cast<int32_t>(step);
    result = status_t::kSuccess;
  } else {
    result = status_t::kInvalidParameter;
  }

  return(result);
}

status_t
AppSdkAUD::PlayGuidance(
  effect_id_t id,
  effect_target_t target,
  effect_mix_t mix,
  effect_pri_t pri,
  bool loop
)
{
  if(eff_mix_tbl_.count(mix) == 0) {
    LOGWARN("invalid parameter mix:[%d]", mix);
    return(status_t::kInvalidParameter);
  }
  if(eff_pri_tbl_.count(pri) == 0) {
    LOGWARN("invalid parameter pri:[%d]", pri);
    return(status_t::kInvalidParameter);
  }
  if(eff_target_tbl_.count(target) == 0) {
    LOGWARN("invalid parameter target:[%d]", target);
    return(status_t::kInvalidParameter);
  }

  GuideID(id);
  PlayTarget(eff_target_tbl_.at(target));
  Sync(eff_sync_tbl_.at(target));
  Mixing(eff_mix_tbl_.at(mix));
  Priority(eff_pri_tbl_.at(pri));
  Repeat(loop);
  return(CHK(mgr_->StartGuidance(this)));
}

status_t
AppSdkAUD::StopGuidance(
  void
)
{
  return(CHK(mgr_->StopGuidance(this)));
}

status_t
AppSdkAUD::ChangeRingtone(
  effect_id_t id
)
{
  ringtone_id_ = id;

  return(status_t::kSuccess);
}

status_t
AppSdkAUD::ChangeAmbientSoundMode(
  asm_ctrl_t o
)
{
  pst::audio::as_mode_control_t m;
  auto ret = CHK(mgr_->GetAmbientSoundControlMode(m));
  if(ret == status_t::kSuccess){
    if(static_cast<pst::audio::as_mode_t>(o) != m.Mode()){
      LOGINFO("ctrl:%s(%d)",JP(j_asm_, m.Mode()));
      m.Mode(static_cast<pst::audio::as_mode_t>(o));
      ret = CHK(mgr_->SetAmbientSoundControlMode(m));
    } else {
      //In the future, it will be determined by AudioService
      //and this return value will be returned.
      LOGINFO("already changed:%s(%d)",JP(j_asm_, m.Mode()));
      ret = status_t::kAUDIO_kAlreadyChanged;
    }
  }
  return ret;
}

void
AppSdkAUD::RingControl(
  bool sw
)
{
  if(sw) {
    GuideID(ringtone_id_);
    PlayTarget(DEFAULT_RINGTONE_TARGET);
    Sync(true);
    Mixing(DEFAULT_RINGTONE_MIX);
    Priority(DEFAULT_RINGTONE_PRI);
    Repeat(true);
    if(status_t::kSuccess == CHK(mgr_->StartGuidance(this))) {
      is_ringtone_ = true;
      LOGDEBUG("start internal ringtone");
    } else {
      is_ringtone_ = false;
      LOGWARN("error ringtone start");
    }
  } else {
    if(is_ringtone_) {
      mgr_->StopGuidance(this);
      is_ringtone_ = false;
      LOGDEBUG("stop internal ringtone");
    }
  }

  LOGDEBUG("ringtone=%s", is_ringtone_ ? "true" : "false");
}

void
AppSdkAUD::OnNotifyGuidanceStatus(
  pst::audio::guide_state_t state,
  pst::audio::guide_cause_t cause
)
{
  LOGINFO("state:%s(%d), cause:%s(%d)", JP(j_gstate_, state), JP(j_gcause_, cause));

  if(gstate_to_event_.count(state)) {
    event_proc_(gstate_to_event_.at(state));
  }
}

void
AppSdkAUD::OnNotifyVolumeStatus(
  pst::audio::volume_dev_t dev,
  pst::audio::volume_state_t state,
  pst::audio::volume_cause_t cause
)
{
  if(state == pst::audio::volume_state_t::kChangeVolume) {
    LOGINFO("dev:%s(%d),state:%s(%d),vol:%d", JP(j_vm_, dev), JP(j_vstate_, state), cause.volume);
    if(vdev_to_event_.count(dev)) {
      event_proc_(vdev_to_event_.at(dev));
    }
  } else {
    LOGINFO("dev:%s(%d),state:%s(%d),mute:%s", JP(j_vm_, dev), JP(j_vstate_, state), cause.mute ? "ON" : "OFF");
    mute_key_t key = std::make_pair(dev, cause.mute);
    if(mute_to_event_.count(key)) {
      event_proc_(mute_to_event_.at(key));
    }
  }
}

void
AppSdkAUD::OnNotifyAmbientSoundControlStatus(
  pst::audio::as_mode_t mode
)
{
  LOGINFO("mode:%s(%d)", JP(j_asm_, mode));
  if(asm_to_event_.count(mode)){
    event_proc_(asm_to_event_.at(mode));
  }
}

status_t
AppSdkAUD::kick_act_relv(
  action_t act
) {
  auto result = status_t::kInvalidAPI;

  if(action_to_relv_.count(act)) {
    auto dev = action_to_relv_.at(act).mode;
    auto vol = step_volume_.at(dev) * action_to_relv_.at(act).sign;
    LOGDEBUG("dev:%s(%d), step:%d", JP(j_vm_, dev), vol);

    pst::audio::volume_control_t ctrl(vol, pst::audio::volume_actsw_t::kRelative);
    ctrl.Device(dev);
    result = CHK(mgr_->SetVolume(ctrl));
  } else {
    LOGDEBUG("no entry");
    result = status_t::kInvalidParameter;
  }

  return(result);
}

status_t
AppSdkAUD::kick_act_mute(
  action_t act
) {
  auto result = status_t::kInvalidAPI;

  if(action_to_mute_.count(act)) {
    auto dev = action_to_mute_.at(act).mode;
    auto sw = action_to_mute_.at(act).sw;

    pst::audio::mute_control_t ctrl(sw, dev);
    result = CHK(mgr_->SetMute(ctrl));
  } else {
    LOGDEBUG("no entry");
    result = status_t::kInvalidParameter;
  }

  return(result);
}

bool
AppSdkAUD::get_position(
  void
)
{
  constexpr char key[] = "pst.tws_position";
  constexpr char kl[] = "left";
  constexpr char kr[] = "right";
  auto result = false;

  if(ch_ == nullptr) {
    char buffer[sizeof(kr)];
    lr_t lr;

    if(PstGetSystemProperty(key, buffer, sizeof(buffer)) > 0) {
      if(0 == std::strcmp(kl, buffer)) {
        lr = kLeft;
        result = true;
      }
      else
      if(0 == std::strcmp(kr, buffer)) {
        lr = kRight;
        result = true;
      }
    }
    if(result) {
      ch_.reset(new lr_t);
      *ch_ = lr;
    }
  } else {
    result = true;
  }

  return(result);
}

status_t
AppSdkAUD::check_result(
  const char* funcname,
  st_t st
)
{
  auto result = status_t::kInvalidAPI;
  auto mess = "unknown value";

  if(chk_status_.count(st)) {
    mess = chk_status_.at(st).mess;
    result = chk_status_.at(st).result;
  }

  if(result != status_t::kSuccess) {
    LOGWARN2(funcname, "status=%s(%d)", mess, static_cast<int>(st), "");
  }

  return(result);
}

}  // namespace appsdk
}  // namespace shared
}  // namespace apps
