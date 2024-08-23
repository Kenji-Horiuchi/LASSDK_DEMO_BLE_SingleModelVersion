//
// Copyright 2023 Sony Semiconductor Solutions Corporation.
//
#ifndef APP_SHARED_INCLUDE_APPSDKSNS_H_
#define APP_SHARED_INCLUDE_APPSDKSNS_H_

#include <cstdint>
#include <memory>
#include <map>
#include <vector>
#include "base_functional.h"

#include "AppSdkType.h"
#include "SensorClient.h"
#include "AppSdkLOG-priv.h"

namespace apps {
namespace shared {
namespace appsdk {

class snsevent_observer_t;

using sensor_statecb_t = pst::base::function<event_t(uint32_t,float)>;
using sensor_dnn_config_t = pst::sensor::DnnEventConfig;
using sensor_initcb_t = pst::base::function<void(void)>;
using sensor_exitcb_t = pst::base::function<void(void)>;

class
AppSdkSNS
{
 public:
  AppSdkSNS(void) = default;
  ~AppSdkSNS() = default;

  void Init(EventNotify);
  void Term(void);
  status_t RegistSensorDetection(
    const char*,
    sensor_statecb_t,
    sensor_initcb_t i_cb = nullptr,
    sensor_dnn_config_t* c = nullptr);
  status_t DeregistSensorDetection(
    const char*,
    sensor_exitcb_t e_cb = nullptr);

 private:
  using type_to_size_t = std::map<pst::sensor::sensor_type_t,size_t>;
  using rcevent_t = std::vector<event_t>;
  using type_to_stfunc_t = std::map<pst::sensor::sensor_type_t,void (AppSdkSNS::*) (pst::sensor::sensor_type_t,pst::sensor::SensorState*)>;
  using wst_event_t = std::map<pst::sensor::wear_state_t,event_t>;
  using st_t = pst::sensor::status_t;
  using st_result_t = struct {const char* mess; status_t result;};
  using chk_st_t = std::map<st_t, st_result_t>;

  struct observer_info_t {
    observer_info_t(void) : notify_func(nullptr), observer(nullptr) {}
    sensor_statecb_t notify_func;
    std::string name;
    snsevent_observer_t* observer;
  };
  using observer_tbl_t = std::map<pst::sensor::sensor_type_t,observer_info_t*>;
  using type_list_t = std::vector<pst::sensor::sensor_type_t>;

  bool regist_observer(pst::sensor::sensor_type_t);
  void event_type_wear(pst::sensor::sensor_type_t,pst::sensor::SensorState*);
  void event_type_dnn(pst::sensor::sensor_type_t,pst::sensor::SensorState*);
  status_t check_result(const char*,st_t);

  pst::sensor::SensorClient* mgr_;
  EventNotify event_proc_;
  observer_tbl_t observer_tbl_;

  const static type_to_size_t type_to_size_;
  const static rcevent_t rcevent_;
  const static type_to_stfunc_t type_to_stfunc_;
  const static wst_event_t wst_to_event_;
  const static type_list_t dnn_list_;

  const static chk_st_t chk_status_;

  JPDEF(pst::sensor::sensor_type_t, j_type_);
};

}  // namespace appsdk
}  // namespace shared
}  // namespace apps

#endif  // APP_SHARED_INCLUDE_APPSDKSNS_H_
