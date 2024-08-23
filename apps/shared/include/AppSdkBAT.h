//
// Copyright 2023 Sony Semiconductor Solutions Corporation.
//
#ifndef APP_SHARED_INCLUDE_APPSDKBAT_H_
#define APP_SHARED_INCLUDE_APPSDKBAT_H_

#include <cstdint>
#include <map>

#include "AppSdkType.h"
#include "AppSdkLOG-priv.h"
#include "BroadcastEvent.h"
#include "BatteryManager.h"
#include "BatteryEvents.h"
//#include "BatteryTypes.h"

namespace apps {
namespace shared {
namespace appsdk {

using bat_lv_statecb_t = pst::base::function<event_t(uint32_t)>;

class
AppSdkBAT
{
 private:
  using st_t = pst::battery::status_t;
  using st_result_t = struct {const char* mess; status_t result;};
  using chk_st_t = std::map<st_t, st_result_t>;

 public:
  AppSdkBAT(void);
  ~AppSdkBAT() = default;

  void Init(EventNotify);
  void Term(void);

  status_t RegisterBatteryLevel(bat_lv_statecb_t state_cb_f);
  status_t DeregisterBatteryLevel(void);

 private:
  void on_notify_event(const pst::core::Event&);
  status_t check_result(const char*,st_t);

 private:
  EventNotify event_proc_;

  std::vector<pst::core::EventObserverHandle> handles_;

  using rcevent_t = std::vector<event_t>;
  const static rcevent_t rcevent_;
  void event_type_lv(uint32_t);
  bat_lv_statecb_t notify_func_;

  const static chk_st_t chk_status_;
};

}  // namespace appsdk
}  // namespace shared
}  // namespace apps

#endif  // APP_SHARED_INCLUDE_APPSDKBAT_H_
