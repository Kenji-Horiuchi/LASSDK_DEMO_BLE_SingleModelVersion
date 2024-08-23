//
// Copyright 2023 Sony Semiconductor Solutions Corporation.
//
#include <algorithm>
#include "GetManager.h"
#define kCNAME "PWR"
#include "AppSdkPWR.h"

namespace apps {
namespace shared {
namespace appsdk {

const AppSdkPWR::chk_st_t
AppSdkPWR::chk_status_ = {
  { st_t::kSuccess,
    { "kSuccess",status_t::kSuccess } },
  { st_t::kInvalid,
    { "kInvalid",status_t::kPWR_kInvalid } },
  { st_t::kInvalidState,
    { "kInvalidState",status_t::kPWR_kInvalidState } },
  { st_t::kInternalError,
    { "kInternalError",status_t::kPWR_kInternalError } },
};

AppSdkPWR::AppSdkPWR()
{
  LOGDEBUG();
}

void
AppSdkPWR::Init(
  EventNotify proc,
  std::string& lock 
)
{
  LOGDEBUG();
  pwr_mgr_ = &easel::GetManager<pst::power::PowerManager>();
  lock_ = lock;
  event_proc_ = proc;
}

void
AppSdkPWR::Term(
  void
)
{
  LOGDEBUG();
}

void
AppSdkPWR::Reboot(
  void
)
{
  easel::SubmitDetachedAsyncTask(
    [this]() {
      pwr_mgr_->Reboot();
    });
}

void
AppSdkPWR::Sleep(
  void
)
{
  easel::SubmitDetachedAsyncTask(
    [this]() {
      pwr_mgr_->Sleep(pst::power::sleep_wake_trigger_t::kVinOrWakeupSignal,0u);
    });
}

status_t
AppSdkPWR::WakeLock(
  void
)
{
  return (CHK(pwr_mgr_->WakeLock(lock_.c_str())));
}

status_t
AppSdkPWR::WakeUnlock(
  void
)
{
  return (CHK(pwr_mgr_->WakeUnLock(lock_.c_str())));
}

status_t
AppSdkPWR::check_result(
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
