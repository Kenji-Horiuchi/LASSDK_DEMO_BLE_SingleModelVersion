//
// Copyright 2023 Sony Semiconductor Solutions Corporation.
//
#ifndef APP_SHARED_INCLUDE_APPSDKAUD_H_
#define APP_SHARED_INCLUDE_APPSDKAUD_H_

#include <cstdint>
#include <map>

#include "AppSdkType.h"
#include "IAudioObserver.h"
#include "AudioClient.h"
#include "AppSdkLOG-priv.h"

namespace apps {
namespace shared {
namespace appsdk {

using effect_id_t = pst::audio::guide_id_t;
enum class effect_target_t {
  kMyself,
  kSync,
  kLeft,
  kRight
};

enum class
volume_ctrl_t {
  kUp,
  kDown,
  kToggle,
  kMute,
  kUnMute,
};

enum class
effect_mix_t {
  kDown,
  kOverlap,
  kMute,
};
enum class
effect_pri_t {
  kLow,
  kHigh,
};

enum class
asm_ctrl_t : uint8_t {
  kOff = static_cast<uint8_t>(pst::audio::as_mode_t::kOff),
  kNoiseCanceling = static_cast<uint8_t>(pst::audio::as_mode_t::kNoiseCanceling),
  kNormal = static_cast<uint8_t>(pst::audio::as_mode_t::kAmbientSoundModeNormal),
  kVoice = static_cast<uint8_t>(pst::audio::as_mode_t::kAmbientSoundModeVoice),
};

constexpr static int32_t
 DEFAULT_VOLUME_STEP = 2;

constexpr static effect_target_t
 DEFAULT_GUIDANCE_TARGET = effect_target_t::kSync;
constexpr static effect_mix_t
 DEFAULT_GUIDANCE_MIX = effect_mix_t::kOverlap;
constexpr static effect_pri_t
 DEFAULT_GUIDANCE_PRI = effect_pri_t::kLow;
constexpr static bool
 DEFAULT_GUIDANCE_LOOP = false;

constexpr static effect_id_t
 DEFAULT_RINGTONE_ID = effect_id_t::kBluetoothDevice1Connected;
constexpr static pst::audio::guide_target_t
DEFAULT_RINGTONE_TARGET = pst::audio::guide_target_t::kBoth;
constexpr static pst::audio::guide_mix_t
 DEFAULT_RINGTONE_MIX = pst::audio::guide_mix_t::kOverlap;
constexpr static pst::audio::guide_priority_t
 DEFAULT_RINGTONE_PRI = pst::audio::guide_priority_t::kUser2;

class
AppSdkAUD :
  public pst::audio::IAudioObserver
{
 public:
  AppSdkAUD(void);
  ~AppSdkAUD() = default;

  bool Init(EventNotify);
  void Term(void);
  inline status_t ChangeSpeakerVolume(uint8_t v) {
    return(ChangeVolume(vm_t::kSpeaker,v));
  }
  inline status_t ChangeSpeakerVolume(volume_ctrl_t c) {
    return(ControlVolume(c,vctrl_to_act_));
  }
  inline status_t SetSpeakerStep(uint8_t s) {
    return(SetStep(vm_t::kSpeaker,s));
  }
  inline status_t ChangeMicVolume(uint8_t v) {
    return(ChangeVolume(vm_t::kMic,v));
  }
  inline status_t ChangeMicVolume(volume_ctrl_t c) {
    return(ControlVolume(c,mctrl_to_act_));
  }
  inline status_t SetMicStep(uint8_t s) {
    return(SetStep(vm_t::kMic,s));
  }
  status_t PlayGuidance(effect_id_t,effect_target_t,effect_mix_t,effect_pri_t,bool);
  inline status_t PlayGuidance(effect_id_t id) {
    return(
      PlayGuidance(
        id,
        DEFAULT_GUIDANCE_TARGET,
        DEFAULT_GUIDANCE_MIX,
        DEFAULT_GUIDANCE_PRI,
        DEFAULT_GUIDANCE_LOOP));
  }
  inline status_t PlayGuidance(effect_id_t id,bool l) {
    return(
      PlayGuidance(
        id,
        DEFAULT_GUIDANCE_TARGET,
        DEFAULT_GUIDANCE_MIX,
        DEFAULT_GUIDANCE_PRI,
        l));
  }
  inline status_t PlayGuidance(effect_id_t id,effect_target_t s,bool l) {
    return(
      PlayGuidance(
        id,
        s,
        DEFAULT_GUIDANCE_MIX,
        DEFAULT_GUIDANCE_PRI,
        l));
  }
  inline status_t PlayGuidance(effect_id_t id,effect_mix_t m) {
    return(
      PlayGuidance(
        id,
        DEFAULT_GUIDANCE_TARGET,
        m,
        DEFAULT_GUIDANCE_PRI,
        DEFAULT_GUIDANCE_LOOP));
  }
  inline status_t PlayGuidance(effect_id_t id,effect_mix_t m,effect_pri_t p) {
    return(
      PlayGuidance(
        id,
        DEFAULT_GUIDANCE_TARGET,
        m,
        p,
        DEFAULT_GUIDANCE_LOOP));
  }
  inline status_t PlayGuidance(effect_id_t id,effect_mix_t m,effect_pri_t p,bool l) {
    return(
      PlayGuidance(
        id,
        DEFAULT_GUIDANCE_TARGET,
        m,
        p,
        l));
  }
  status_t StopGuidance(void);
  status_t ChangeRingtone(effect_id_t);
  status_t ChangeAmbientSoundMode(asm_ctrl_t);

 private:
  enum
  lr_t
  {
    kLeft,
    kRight
  };
  using vm_t = pst::audio::volume_dev_t;
  using act_proc_t = std::map<action_t, status_t (AppSdkAUD::*) (action_t)>;
  using relv_t = struct {vm_t mode; int sign;};
  using act_relv_t = std::map<action_t, relv_t>;
  using mute_t = struct {vm_t mode; pst::audio::mute_sw_t sw;};
  using act_mute_t = std::map<action_t, mute_t>;
  using relv_tbl_t = std::map<vm_t,int32_t>;
  using vctrl_act_t = std::map<volume_ctrl_t,action_t>;
  using gstate_to_event_t = std::map<pst::audio::guide_state_t,event_t>;
  using eff_target_tbl_t = std::map<effect_target_t,pst::audio::guide_target_t>;
  using eff_sync_tbl_t = std::map<effect_target_t,bool>;
  using st_t = pst::audio::status_t;
  using st_result_t = struct {const char* mess; status_t result;};
  using chk_st_t = std::map<st_t, st_result_t>;

 protected:
  status_t ExecAction(action_t);
  status_t ChangeVolume(vm_t,uint8_t);
  status_t ControlVolume(volume_ctrl_t,const vctrl_act_t&);
  status_t SetStep(vm_t,uint8_t);
  void RingControl(bool);

 private:
  void OnNotifyGuidanceStatus(
    pst::audio::guide_state_t,
    pst::audio::guide_cause_t) override;
  void OnNotifyVolumeStatus(
    pst::audio::volume_dev_t,
    pst::audio::volume_state_t,
    pst::audio::volume_cause_t) override;
  void OnNotifyAmbientSoundControlStatus(
    pst::audio::as_mode_t) override;

  status_t kick_act_relv(action_t);
  status_t kick_act_mute(action_t);
  bool get_position(void);
  status_t check_result(const char*,st_t);

 private:
  pst::audio::AudioClient* mgr_ = nullptr;
  EventNotify event_proc_ = nullptr;
  relv_tbl_t step_volume_;
  bool is_ringtone_ = false;
  effect_id_t ringtone_id_ = DEFAULT_RINGTONE_ID;
  std::unique_ptr<lr_t> ch_ = nullptr;
  eff_target_tbl_t eff_target_tbl_;

  const static act_proc_t action_proc_;

  const static act_relv_t action_to_relv_;
  const static act_mute_t action_to_mute_;
  using vdev_event_t = std::map<vm_t,event_t>;
  const static vdev_event_t vdev_to_event_;
  using mute_key_t = std::pair<vm_t,bool>;
  using mute_event_t = std::map<mute_key_t,event_t>;
  const static mute_event_t mute_to_event_;
  const static relv_tbl_t init_step_volume_;
  using eff_mix_t = std::map<effect_mix_t, pst::audio::guide_mix_t>;
  const static eff_mix_t eff_mix_tbl_;
  using eff_pri_t = std::map<effect_pri_t, pst::audio::guide_priority_t>;
  const static eff_pri_t eff_pri_tbl_;
  const static vctrl_act_t vctrl_to_act_;
  const static vctrl_act_t mctrl_to_act_;
  const static gstate_to_event_t gstate_to_event_;
  const static  eff_sync_tbl_t eff_sync_tbl_;
  using asm_event_t = std::map<pst::audio::as_mode_t,event_t>;
  const static asm_event_t asm_to_event_;

  const static chk_st_t chk_status_;

  JPDEF(vm_t, j_vm_);
  JPDEF(volume_ctrl_t, j_vctrl_);
  JPDEF(pst::audio::volume_state_t, j_vstate_);
  JPDEF(pst::audio::guide_state_t, j_gstate_);
  JPDEF(pst::audio::guide_cause_t, j_gcause_);
  JPDEF(pst::audio::as_mode_t, j_asm_);
};

}  // namespace appsdk
}  // namespace shared
}  // namespace apps

#endif  // APP_SHARED_INCLUDE_APPSDKAUD_H_
