//
// Copyright 2023 Sony Semiconductor Solutions Corporation.
//
#include <algorithm>
#include "GetManager.h"
#define kCNAME "BAT"
#include "AppSdkBAT.h"

namespace apps {
namespace shared {
namespace appsdk {

const AppSdkBAT::chk_st_t
AppSdkBAT::chk_status_ = {
  { st_t::kSuccess,
    { "kSuccess",status_t::kSuccess } },
  { st_t::kInternalError,
    { "kInternalError",status_t::kBAT_kInternalError } },
};

const AppSdkBAT::rcevent_t
AppSdkBAT::rcevent_ =
{
  event_t::kBatteryEvent1,
  event_t::kBatteryEvent2,
  event_t::kBatteryEvent3,
  event_t::kBatteryEvent4,
  event_t::kBatteryEvent5,
};

AppSdkBAT::AppSdkBAT(void) {}

void
AppSdkBAT::Init(
  EventNotify proc
)
{
  LOGINFO();

#define REG_EVENT(ev_id) \
    handles_.push_back(easel::RegisterEventObserver( \
        ev_id, \
        [this](const pst::core::Event& event) { \
            LOGDEBUG("%s received", #ev_id); \
            on_notify_event(event); \
        }))
  REG_EVENT(pst::battery::kEventChargeLevelChangeBroadcast);
  REG_EVENT(pst::battery::kEventLowBatteryVoltageBroadcast);
#undef REG_EVENT

  event_proc_ = proc;

}

void
AppSdkBAT::Term(
  void
)
{
  LOGINFO();
}

status_t
AppSdkBAT::RegisterBatteryLevel(
  bat_lv_statecb_t state_cb_f
)
{
  auto result = status_t::kInvalidAPI;
  if(state_cb_f == nullptr) {
    result = status_t::kInvalidParameter;
  } else {
      if(notify_func_!=nullptr) {
        result = status_t::kBatteryLevelRegistered; 
      } else {
        notify_func_ = state_cb_f;
        result = status_t::kSuccess;
      }
  }

  return(result);
}

status_t
AppSdkBAT::DeregisterBatteryLevel(
  void
)
{
  auto result = status_t::kInvalidAPI;

  if(!notify_func_) {
    result = status_t::kNotRegist; 
  } else {
    notify_func_ = nullptr;
    result = status_t::kSuccess;
  }

  return(result);
}

void
AppSdkBAT::event_type_lv(
  uint32_t lv
)
{
  if(notify_func_) {
    auto rc = notify_func_(lv);
    if(std::find(rcevent_.begin(), rcevent_.end(), rc) != rcevent_.end()) {
      event_proc_(rc);
    } else {
      if(rc != event_t::kBatteryEventdonot) {
        LOGERR("not supported code:%d", rc);
      } else {
        LOGDEBUG("battery event do not");
      }
    }
  }
}

void AppSdkBAT::on_notify_event(const pst::core::Event& event) {
  LOGINFO();
  const auto event_id = event.GetEventId();
  if (event_id == pst::battery::kEventChargeLevelChangeBroadcast) {
    event_type_lv(event.GetExtra<uint32_t>());
  }else if (event_id == pst::battery::kEventLowBatteryVoltageBroadcast) {
    event_proc_(event_t::kBatteryEmpty);
  } else {
    LOGINFO("not notify");
  }
}

status_t
AppSdkBAT::check_result(
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
