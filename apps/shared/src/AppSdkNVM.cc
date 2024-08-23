//
// Copyright 2023 Sony Semiconductor Solutions Corporation.
//
#include <algorithm>
#include "GetManager.h"
#define kCNAME "NVM"
#include "AppSdkNVM.h"

namespace apps {
namespace shared {
namespace appsdk {
const auto nv_group_ =
    pst::setting::group_id_t(PST_STATIC_ID("apps.appsdk"));

const AppSdkNVM::chk_st_t
AppSdkNVM::chk_status_ = {
  { st_t::kSuccess,
    { "kSuccess",status_t::kSuccess } },
  { st_t::kNotFound,
    { "kNotFound",status_t::kNVM_kNotFound } },
  { st_t::kInvalid,
    { "kInvalid",status_t::kNVM_kInvalid } },
  { st_t::kInvalidState,
    { "kInvalidState",status_t::kNVM_kInvalidState } },
  { st_t::kExceedMaxLength,
    { "kExceedMaxLength",status_t::kNVM_kExceedMaxLength } },
  { st_t::kInternalError,
    { "kInternalError",status_t::kNVM_kInternalError } },
  { st_t::kStorageCorrupt,
    { "kStorageCorrupt",status_t::kNVM_kStorageCorrupt } },
  { st_t::kAlreadyExists,
    { "kAlreadyExists",status_t::kNVM_kAlreadyExists } },
};

AppSdkNVM::AppSdkNVM()
{
  LOGDEBUG();
}

void
AppSdkNVM::Init(
  EventNotify
)
{
  LOGDEBUG();
  mgr_ = &easel::GetManager<pst::setting::SettingsManager>();
}

void
AppSdkNVM::Term(
  void
)
{
  LOGDEBUG();
}

status_t
AppSdkNVM::SetNvmInt(
  nvm_key_id_t key,
  int val
)
{
  return (CHK(mgr_->SetInt(nv_group_, key, val)));
}

status_t
AppSdkNVM::SetNvmBool(
  nvm_key_id_t key,
  bool val
)
{
  return (CHK(mgr_->SetBool(nv_group_, key, val)));
}

status_t
AppSdkNVM::SetNvmString(
  nvm_key_id_t key,
  std::string val
)
{
  return (CHK(mgr_->SetString(nv_group_, key, val)));
}

status_t
AppSdkNVM::SetNvmString(
  nvm_key_id_t key,
  const char* val
)
{
  return (CHK(mgr_->SetString(nv_group_, key, val)));
}

status_t
AppSdkNVM::SetNvmBinary(
  nvm_key_id_t key,
  const uint8_t* val,
  size_t size
)
{
  return (CHK(mgr_->SetBinary(nv_group_, key, val, size)));
}

status_t
AppSdkNVM::SetNvmIntOnUnResettable(
  nvm_key_id_t key,
  int val
)
{
  return (CHK(mgr_->SetIntOnUnResettable(nv_group_, key, val)));
}

status_t
AppSdkNVM::SetNvmBoolOnUnResettable(
  nvm_key_id_t key,
  bool val
)
{
  return (CHK(mgr_->SetBoolOnUnResettable(nv_group_, key, val)));
}

status_t
AppSdkNVM::SetNvmStringOnUnResettable(
  nvm_key_id_t key,
  std::string val
)
{
  return (CHK(mgr_->SetStringOnUnResettable(nv_group_, key, val)));
}

status_t
AppSdkNVM::SetNvmStringOnUnResettable(
  nvm_key_id_t key,
  const char* val
)
{
  return (CHK(mgr_->SetStringOnUnResettable(nv_group_, key, val)));
}

status_t
AppSdkNVM::SetNvmBinaryOnUnResettable(
  nvm_key_id_t key,
  const uint8_t* val,
  size_t size
)
{
  return (CHK(mgr_->SetBinaryOnUnResettable(nv_group_, key, val, size)));
}

status_t
AppSdkNVM::GetNvm(
  nvm_key_id_t key,
  nvm_value_t* val
)
{
  return (CHK(mgr_->Get(nv_group_, key, val)));
}

status_t
AppSdkNVM::ResetAllNvmValues(
  void
)
{
  return (CHK(mgr_->ResetAllSettingValues()));
}

status_t
AppSdkNVM::check_result(
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
