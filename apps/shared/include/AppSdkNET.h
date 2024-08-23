//
// Copyright 2023 Sony Semiconductor Solutions Corporation.
//
#ifndef APP_SHARED_INCLUDE_APPSDKNET_H_
#define APP_SHARED_INCLUDE_APPSDKNET_H_

#include <cstdint>
#include <map>

#include "AppSdkType.h"
#include "INetworkCommunicationObserver.h"
#include "NetworkCommunicationClient.h"
#include "AppSdkLOG-priv.h"

namespace apps {
namespace shared {
namespace appsdk {

enum class
net_ctrl_t {
  kEnablePairedDevice = action_t::kNetEnablePairedDevice,
  kPairing = action_t::kNetPairing,
  kDisable = action_t::kNetDisable,
  kRotate = action_t::kNetRotate,
};
enum class
media_ctrl_t {
  kPlay = action_t::kMusicPlay,
  kPause = action_t::kMusicPause,
  kNextTrack = action_t::kMusicNext,
  kPrevTrack = action_t::kMusicPrev,
  kFastForward = action_t::kMusicForward,
  kFastRewind = action_t::kMusicRewind,
};
enum class
voice_ctrl_t {
  kStart = action_t::kVoiceStart,
  kEnd = action_t::kVoiceEnd,
  kHold = action_t::kVoiceHold,
  kUnHold = action_t::kVoiceUnHold,
};

class
AppSdkNET :
  public pst::netcom::INetworkCommunicationObserver
{
 private:
  using act_proc_t = std::map<action_t, status_t (AppSdkNET::*) (action_t)>;
  struct net_opt_t {
    pst::netcom::connect_control_type_t type;
    pst::netcom::connection_id_t id;
  };
  using act_net_t = std::map<action_t, net_opt_t>;
  struct mus_opt_t {
    pst::netcom::media_control_type_t type;
    pst::netcom::media_control_option option;
  };
  using act_mus_t = std::map<action_t ,mus_opt_t>;
  using act_call_t = std::map<action_t, pst::netcom::call_control_type_t>;

  using st_t = pst::netcom::status_t;
  using st_result_t = struct {const char* mess; status_t result;};
  using chk_st_t = std::map<st_t, st_result_t>;

 public:
  AppSdkNET(void);
  ~AppSdkNET() = default;

  bool Init(EventNotify);
  void Term(void);
  inline status_t ControlNetwork(net_ctrl_t c) {
    return(ExecAction(static_cast<action_t>(c)));
  }
  inline status_t ControlMedia(media_ctrl_t c) {
    return(ExecAction(static_cast<action_t>(c)));
  }
  inline status_t ControlVoice(voice_ctrl_t c) {
    return(ExecAction(static_cast<action_t>(c)));
  }

  status_t RemoveConnectionHistory(void);

 protected:
  status_t ExecAction(action_t);

 private:
  void OnNotifyConnectStatusChanged(
    const uint8_t,
    const bool) override;
  void OnNotifyStreamStatusChanged(
    const uint8_t,
    const bool) override;
  void OnNotifyMediaStatusChanged(
    const uint8_t,
    const pst::netcom::media_status_t) override;
  void OnNotifyCallStatusChanged(
    const uint8_t,
    const pst::netcom::call_status_t) override;

  status_t kick_act_enable_paired_device(action_t);
  status_t kick_act_pairing(action_t);
  status_t kick_act_disable(action_t);
  status_t kick_act_rotate(action_t);
  status_t kick_act_music(action_t);
  status_t kick_act_voice(action_t);
  status_t check_result(const char*,st_t);

 private:
  pst::netcom::NetworkCommunicationClient* mgr_;
  EventNotify event_proc_;
  act_net_t action_to_network_;
  act_mus_t action_to_music_;

  const static act_proc_t action_proc_;
  const static act_call_t action_to_call_;

  using mst_event_t = std::map<pst::netcom::media_status_t,event_t>;
  const static mst_event_t mst_to_event_;
  using cst_event_t = std::map<pst::netcom::call_status_t,event_t>;
  const static cst_event_t cst_to_event_;

  const static chk_st_t chk_status_;

  JPDEF(pst::netcom::media_status_t, j_media_st_);
  JPDEF(pst::netcom::call_status_t, j_call_st_);
};

}  // namespace appsdk
}  // namespace shared
}  // namespace apps

#endif  // APP_SHARED_INCLUDE_APPSDKNET_H_
