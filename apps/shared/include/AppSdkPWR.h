//
// Copyright 2023 Sony Semiconductor Solutions Corporation.
//
#ifndef APP_SHARED_INCLUDE_APPSDKPWR_H_
#define APP_SHARED_INCLUDE_APPSDKPWR_H_

#include <cstdint>
#include <map>

#include "AppSdkType.h"
#include "AppSdkLOG-priv.h"
#include "BroadcastEvent.h"
#include "PowerManager.h"

namespace apps {
namespace shared {
namespace appsdk {

class
AppSdkPWR
{
 private:
  using st_t = pst::power::status_t;
  using st_result_t = struct {const char* mess; status_t result;};
  using chk_st_t = std::map<st_t, st_result_t>;

 public:
  AppSdkPWR();
  ~AppSdkPWR() = default;
  void Init(EventNotify,std::string&);
  void Term(void);

  void Reboot(void);
  void Sleep(void);
  status_t WakeLock(void);
  status_t WakeUnlock(void);

 private:
  status_t check_result(const char*,st_t);

 private:
  EventNotify event_proc_;
  pst::power::PowerManager* pwr_mgr_;
  std::string lock_;
  const static chk_st_t chk_status_;
};

}  // namespace appsdk
}  // namespace shared
}  // namespace apps

#endif  // APP_SHARED_INCLUDE_APPSDKPWR_H_
