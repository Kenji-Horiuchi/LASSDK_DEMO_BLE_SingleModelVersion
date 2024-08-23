//
// Copyright 2023 Sony Semiconductor Solutions Corporation.
//
#ifndef APP_SHARED_INCLUDE_APPSDKCDL_H_
#define APP_SHARED_INCLUDE_APPSDKCDL_H_

#include <cstdint>
#include <map>

#include "AppSdkType.h"
#include "CradleClient.h"
#include "CradleEvents.h"
#include "AppSdkLOG-priv.h"
#include "BroadcastEvent.h"
#include "CradleTypes.h"
#include "VolatileSettingsManager.h"
#include "AppHandleCradleFdCommand.h"
#include "AppHandleCradleGpCommand.h"

namespace apps {
namespace shared {
namespace appsdk {

using gp_cmd_param_t = pst::cradle::GpCommandParameter;
using cdl_gp_statecb_t = pst::base::function<event_t(gp_cmd_param_t *)>;

using fd_cmd_param_t = pst::cradle::FdCommandParameter;
using cdl_fd_statecb_t = pst::base::function<event_t(fd_cmd_param_t *)>;

using fd_write_param_t = pst::cradle::FdWriteParameter;
using fd_write_type_t = pst::cradle::fd_write_type_t;

class
AppSdkCDL : 
    public pst::cradle::ICradleObserver
{
 private:
  using st_t = pst::cradle::status_t;
  using st_result_t = struct {const char* mess; status_t result;};
  using chk_st_t = std::map<st_t, st_result_t>;

 public:
  AppSdkCDL(void);
  ~AppSdkCDL() = default;

  bool Init(EventNotify);
  void Term(void);

  status_t RegisterCradleGpCommand(cdl_gp_statecb_t state_cb_f);
  status_t DeregisterCradleGpCommand(void);
  status_t RegisterFdCommand(cdl_fd_statecb_t state_cb_f);
  status_t DeregisterFdCommand(void);
  void SendFdData(fd_write_param_t);
private:
  void OnNotifyGpRequest(pst::cradle::GpCommandParameter *command) override;
  void OnNotifyFdRequest(pst::cradle::FdCommandParameter *command) override;

  void on_notify_event(const pst::core::Event&);
  status_t check_result(const char*,st_t);

 private:
  pst::cradle::CradleClient* mgr_;
  pst::volatilesetting::VolatileSettingsManager* vol_mgr_ = nullptr;
  EventNotify event_proc_;

  using cdl_event_t = std::map<pst::cradle::cradle_state_t,event_t>;
  const static cdl_event_t cdl_to_event_;
  std::vector<pst::core::EventObserverHandle> handles_;

  using rcevent_t = std::vector<event_t>;
  const static rcevent_t rcevent_;
  void event_type_gp(pst::cradle::GpCommandParameter *command);
  cdl_gp_statecb_t notify_func_gp_;
  int event_type_fd(pst::cradle::FdCommandParameter *command);
  cdl_fd_statecb_t notify_func_fd_;

  const static chk_st_t chk_status_;
};

}  // namespace appsdk
}  // namespace shared
}  // namespace apps

#endif  // APP_SHARED_INCLUDE_APPSDKCDL_H_
