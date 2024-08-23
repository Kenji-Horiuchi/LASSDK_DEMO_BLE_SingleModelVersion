//
// Copyright 2024 Sony Semiconductor Solutions Corporation.
//
#ifndef APP_SHARED_INCLUDE_APPSDKSNSOBSERVER_PRIV_H_
#define APP_SHARED_INCLUDE_APPSDKSNSOBSERVER_PRIV_H_

#include <cstdint>
#include "base_functional.h"

#include "AppSdkType.h"
#include "ISensorServiceObserver.h"
#include "SensorClient.h"
#include "AppSdkLOG-priv.h"

namespace apps {
namespace shared {
namespace appsdk {

class
snsevent_observer_t :
  public pst::sensor::ISensorServiceObserver
{
 public:
  using state_proc_t = pst::base::function<void(pst::sensor::sensor_type_t,pst::sensor::SensorState*)>;

  snsevent_observer_t(pst::sensor::SensorClient* mgr) : mgr_(mgr) {}
  ~snsevent_observer_t() = default;

  status_t Regist(pst::sensor::sensor_type_t, size_t, state_proc_t);
  status_t Deregist();

 private:
  using st_t = pst::sensor::status_t;
  using st_result_t = struct {const char* mess; status_t result;};
  using chk_st_t = std::map<st_t, st_result_t>;

  void OnChangedSensorState(pst::sensor::sensor_type_t, const void*) override;
  status_t check_result(const char*,st_t);

  pst::sensor::sensor_type_t type_;
  pst::sensor::SensorClient* mgr_;
  size_t size_;
  state_proc_t proc_ = nullptr;
  bool run_ = false;

  const static chk_st_t chk_status_;

  JPDEF(pst::sensor::sensor_type_t, j_type_);
};

}  // namespace appsdk
}  // namespace shared
}  // namespace apps

#endif  // APP_SHARED_INCLUDE_APPSDKSNSOBSERVER_PRIV_H_
