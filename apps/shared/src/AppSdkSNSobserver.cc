//
// Copyright 2023 Sony Semiconductor Solutions Corporation.
//
#include "AppSdkDefs-priv.h"
#include "AsyncTask.h"
#include "base_memory.h"
#define kCNAME "SNSob"
#include "AppSdkSNSobserver-priv.h"

namespace apps {
namespace shared {
namespace appsdk {

class
sensor_state_t
{
 public:
  sensor_state_t(const void* d, size_t s) { memcpy(&body_, d, s); };
  inline pst::sensor::SensorState* body(void) { return(&body_); };
 private:
  pst::sensor::SensorState body_;
};

const snsevent_observer_t::chk_st_t
snsevent_observer_t::chk_status_ = {
  { st_t::kSuccess,
    { "kSuccess",           status_t::kSuccess } },
  { st_t::kInternal,
    { "kInternal",          status_t::kSNS_kInternal } },
  { st_t::kInvalid,
    { "kInvalid",           status_t::kSNS_kInvalid } },
  { st_t::kInvalidState,
    { "kInvalidState",      status_t::kSNS_kInvalidState } },
};

JPVAR(snsevent_observer_t::j_type_) =
{
  { pst::sensor::sensor_type_t::kWear, "kWear" },
  { pst::sensor::sensor_type_t::kDnn1, "kDnn1" },
  { pst::sensor::sensor_type_t::kDnn2, "kDnn2" },
};

void
snsevent_observer_t::OnChangedSensorState(
  pst::sensor::sensor_type_t type,
  const void* data
)
{
  LOGDEBUG("type=%s(%d)", JP(j_type_, type));

  if(type == type_) {
    easel::SubmitDetachedAsyncTask(
      unsync_func_t<sensor_state_t>(
        pst::base::make_unique<sensor_state_t>(data, size_),
        [this,type](sensor_state_t* data_)
        {
          proc_(type, data_->body());
        }));
  } else {
    LOGWARN("unexpected type:%d", type);
  }
}

status_t
snsevent_observer_t::Regist(
  pst::sensor::sensor_type_t type,
  size_t size,
  state_proc_t proc
)
{
  auto result = status_t::kInvalidAPI;

  result = CHK(mgr_->RegisterObserver(type, this));
  if(result == status_t::kSuccess) {
    proc_ = proc;
    size_ = size;
    type_ = type;

    result = CHK(mgr_->EnableSensor(type));
    if(result == status_t::kSuccess) {
      run_ = true;
    }
  }

  return(result);
}

status_t
snsevent_observer_t::Deregist()
{
  auto result = status_t::kSuccess;
  if(run_) {
    result = CHK(mgr_->DisableSensor(type_));
  }
  if(proc_ != nullptr) {
    auto r = CHK(mgr_->DeregisterObserver(type_, this));
    if(r != status_t::kSuccess) {
      result = r;
    }
  }
  return(result);
}

status_t
snsevent_observer_t::check_result(
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
