//
// Copyright 2023 Sony Semiconductor Solutions Corporation.
//
#include "GetManager.h"
#define kCNAME "NET"
#include "AppSdkNET.h"

namespace apps {
namespace shared {
namespace appsdk {

const AppSdkNET::act_proc_t
AppSdkNET::action_proc_ = {
  { action_t::kNetEnablePairedDevice, &AppSdkNET::kick_act_enable_paired_device },
  { action_t::kNetPairing, &AppSdkNET::kick_act_pairing },
  { action_t::kNetDisable, &AppSdkNET::kick_act_disable },
  { action_t::kNetRotate, &AppSdkNET::kick_act_rotate },
  { action_t::kMusicPlay, &AppSdkNET::kick_act_music },
  { action_t::kMusicPause, &AppSdkNET::kick_act_music },
  { action_t::kMusicForward, &AppSdkNET::kick_act_music },
  { action_t::kMusicRewind, &AppSdkNET::kick_act_music },
  { action_t::kMusicNext, &AppSdkNET::kick_act_music },
  { action_t::kMusicPrev, &AppSdkNET::kick_act_music },
  { action_t::kVoiceStart, &AppSdkNET::kick_act_voice },
  { action_t::kVoiceEnd, &AppSdkNET::kick_act_voice },
  { action_t::kVoiceHold, &AppSdkNET::kick_act_voice },
  { action_t::kVoiceUnHold, &AppSdkNET::kick_act_voice },
};
const AppSdkNET::act_call_t
AppSdkNET::action_to_call_ = {
  { action_t::kVoiceStart, pst::netcom::call_control_type_t::kOffHook },
  { action_t::kVoiceEnd, pst::netcom::call_control_type_t::kOnHook },
  { action_t::kVoiceHold, pst::netcom::call_control_type_t::kHold },
  { action_t::kVoiceUnHold, pst::netcom::call_control_type_t::kRetrieve },
};

const AppSdkNET::mst_event_t
AppSdkNET::mst_to_event_ = {
  { pst::netcom::media_status_t::kPlaying, event_t::kMusicPlay },
  { pst::netcom::media_status_t::kPaused, event_t::kMusicPause },
  { pst::netcom::media_status_t::kInactive, event_t::kMusicStop },
  { pst::netcom::media_status_t::kSeeking, event_t::kMusicSeek },
};
const AppSdkNET::cst_event_t
AppSdkNET::cst_to_event_ = {
  { pst::netcom::call_status_t::kIncomingRingtone, event_t::kVoiceRing },
  { pst::netcom::call_status_t::kGuidanceRingtone, event_t::kSysVoiceRingInternal },
  { pst::netcom::call_status_t::kTalking, event_t::kVoiceStart },
  { pst::netcom::call_status_t::kWaiting, event_t::kVoiceEnd },
  { pst::netcom::call_status_t::kHeld, event_t::kVoiceHold },
};

const AppSdkNET::chk_st_t
AppSdkNET::chk_status_ = {
  { st_t::kSuccess,
    { "kSuccess",           status_t::kSuccess } },
  { st_t::kInvalidParam,
    { "kInvalidParam",      status_t::kNET_kInvalidParam } },
  { st_t::kSuccessNotSwitched,
    { "kNotSwitched",       status_t::kNET_kNotSwitched } },
  { st_t::kInvalidConnectionStopped,
    { "kConnectionStopped", status_t::kNET_kConnectionStopped } },
  { st_t::kInvalidNoConnecting,
    { "kNoConnecting",      status_t::kNET_kNoConnecting } },
  { st_t::kInvalidNoDeviceInfo,
    { "kNoDeviceInfo",      status_t::kNET_kNoDeviceInfo } },
  { st_t::kInvalidQueueFull,
    { "kQueueFull",         status_t::kNET_kQueueFull } },
  { st_t::kInvalidState,
    { "kInvalidState",      status_t::kNET_kInvalidState } },
  { st_t::kInvalidNotSupport,
    { "kNotSupport",        status_t::kNET_kNotSupport } },
  { st_t::kInvalidBufferFull,
    { "kBufferFull",        status_t::kNET_kBufferFull } },
};
JPVAR(AppSdkNET::j_media_st_) =
{
  { pst::netcom::media_status_t::kPlaying, "kPlaying" },
  { pst::netcom::media_status_t::kPaused, "kPaused" },
  { pst::netcom::media_status_t::kInactive, "kInactive" },
  { pst::netcom::media_status_t::kSeeking, "kSeeking" },
};
JPVAR(AppSdkNET::j_call_st_) =
{
  { pst::netcom::call_status_t::kIncomingRingtone, "kIncomingRingtone" },
  { pst::netcom::call_status_t::kGuidanceRingtone, "kGuidanceRingtone" },
  { pst::netcom::call_status_t::kTalking, "kTalking" },
  { pst::netcom::call_status_t::kWaiting, "kWaiting" },
  { pst::netcom::call_status_t::kHeld, "kHeld" },
};


AppSdkNET::AppSdkNET(void)
{
  {
    mus_opt_t opt;

    opt.type = pst::netcom::media_control_type_t::kPlay;
    action_to_music_[action_t::kMusicPlay] = opt;

    opt.type = pst::netcom::media_control_type_t::kPause;
    action_to_music_[action_t::kMusicPause] = opt;

    opt.type = pst::netcom::media_control_type_t::kSeekTrack;
    opt.option.seek = pst::netcom::seek_track_type_t::kFastForward;
    action_to_music_[action_t::kMusicForward] = opt;
    opt.option.seek = pst::netcom::seek_track_type_t::kFastRewind;
    action_to_music_[action_t::kMusicRewind] = opt;

    opt.type = pst::netcom::media_control_type_t::kMoveTrack;
    opt.option.move = pst::netcom::move_track_type_t::kNext;
    action_to_music_[action_t::kMusicNext] = opt;
    opt.option.move = pst::netcom::move_track_type_t::kPrevious;
    action_to_music_[action_t::kMusicPrev] = opt;
  }
  {
    net_opt_t opt;

    opt.type = pst::netcom::connect_control_type_t::kSwitchRotate;
    action_to_network_[action_t::kNetRotate] = opt;
  }
}

bool
AppSdkNET::Init(
  EventNotify proc
)
{
  LOGDEBUG();

  mgr_ = &easel::GetManager<pst::netcom::NetworkCommunicationClient>();
  if(status_t::kSuccess != CHK(mgr_->RegisterObserver(this))) {
    LOGERR("regist error");
    return(false);
  }

  event_proc_ = proc;

  return(true);
}

void
AppSdkNET::Term(
  void
)
{
  LOGDEBUG();

  if(mgr_) {
    CHK(mgr_->DeregisterObserver());
  }
}

status_t
AppSdkNET::RemoveConnectionHistory(
  void
)
{
  return (CHK(mgr_->RemoveConnectionHistory(pst::netcom::connect_history_type_t::kAll)));
}

status_t
AppSdkNET::ExecAction(
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

void
AppSdkNET::OnNotifyConnectStatusChanged(
  const uint8_t id,
  const bool st
)
{
  LOGINFO("id=%d, status=%s",id , st ? "true" : "false");
  if(static_cast<pst::netcom::connection_id_t>(id)!=pst::netcom::connection_id_t::kPair){
    event_proc_(st ? event_t::kNetConnect : event_t::kNetDisconnect);
  }
}

void
AppSdkNET::OnNotifyStreamStatusChanged(
  const uint8_t,
  const bool st
)
{
  LOGINFO("status=%s", st ? "true" : "false");

  event_proc_(st ? event_t::kStreamUsed : event_t::kStreamFree);
}

void
AppSdkNET::OnNotifyMediaStatusChanged(
  const uint8_t,
  const pst::netcom::media_status_t st
)
{
  LOGINFO("status=%s(%d)", JP(j_media_st_, st));

  if(mst_to_event_.count(st)) {
    event_proc_(mst_to_event_.at(st));
  } else {
    LOGDEBUG("not notify");
  }
}

void
AppSdkNET::OnNotifyCallStatusChanged(
  const uint8_t,
  const pst::netcom::call_status_t st
)
{
  LOGINFO("status=%s(%d)", JP(j_call_st_, st));

  if(cst_to_event_.count(st)) {
    event_proc_(cst_to_event_.at(st));
  } else {
    LOGDEBUG("not notify");
  }
}

status_t
AppSdkNET::kick_act_enable_paired_device(
  action_t
) {
  LOGDEBUG();

  auto result = status_t::kInvalidAPI;

  bool b = false;
  result = CHK(mgr_->GetConnectionHistoryStatus(&b));
  if(result!=status_t::kSuccess){
    LOGDEBUG("error get connection history:%d,ret:%d",b,result);
    return result;
  }else if(!b){
    LOGDEBUG("no connection history:%d,ret:%d",b,result);
    return status_t::kNET_kNoDeviceInfo;
  }

  result = CHK(mgr_->SetConnectControl(pst::netcom::connect_control_type_t::kConnect,
                                        pst::netcom::connection_id_t::kAuto));

  return(result);
}

status_t
AppSdkNET::kick_act_pairing(
  action_t
) {
  LOGDEBUG();

  return(CHK(mgr_->SetBlePairingMode()));
}

status_t
AppSdkNET::kick_act_disable(
  action_t
) {
  LOGDEBUG();

  auto result = status_t::kInvalidAPI;

  result = CHK(mgr_->SetConnectControl(pst::netcom::connect_control_type_t::kCancel,
                                        pst::netcom::connection_id_t::kAll));
  if(result==status_t::kSuccess){
    result = CHK(mgr_->SetConnectControl(pst::netcom::connect_control_type_t::kDisconnect,
                                          pst::netcom::connection_id_t::kAll));
  } else {
    LOGDEBUG("erro cancel request");
  }
  return(result);
}

status_t
AppSdkNET::kick_act_rotate(
  action_t act
) {
  LOGDEBUG();

  auto result = status_t::kInvalidAPI;

  if(action_to_network_.count(act)) {
    result = CHK(mgr_->SetConnectControl(action_to_network_.at(act).type,
                                         action_to_network_.at(act).id));
  } else {
    LOGDEBUG("no entry");
    result = status_t::kInvalidParameter;
  }

  return(result);
}

status_t
AppSdkNET::kick_act_music(
  action_t act
) {
  constexpr auto current_id = pst::netcom::connection_id_t::kAuto;

  LOGDEBUG();

  auto result = status_t::kInvalidAPI;

  if(action_to_music_.count(act)) {
    result = CHK(mgr_->SetMediaControl(current_id,
                                       action_to_music_.at(act).type,
                                       &action_to_music_.at(act).option));
  } else {
    LOGDEBUG("no entry");
    result = status_t::kInvalidParameter;
  }

  return(result);
}

status_t
AppSdkNET::kick_act_voice(
  action_t act
) {
  constexpr auto current_id = pst::netcom::connection_id_t::kAuto;

  LOGDEBUG();

  auto result = status_t::kInvalidAPI;

  if(action_to_call_.count(act)) {
    result = CHK(mgr_->SetCallControl(current_id, action_to_call_.at(act)));
  } else {
    LOGDEBUG("no entry");
    result = status_t::kInvalidParameter;
  }

  return(result);
}

status_t
AppSdkNET::check_result(
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
