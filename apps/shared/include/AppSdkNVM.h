//
// Copyright 2023 Sony Semiconductor Solutions Corporation.
//
#ifndef APP_SHARED_INCLUDE_APPSDKNVM_H_
#define APP_SHARED_INCLUDE_APPSDKNVM_H_

#include <cstdint>
#include <map>

#include "AppSdkType.h"
#include "AppSdkLOG-priv.h"
#include "BroadcastEvent.h"
#include "SettingsManager.h"
#include "SettingsValue.h"

namespace apps {
namespace shared {
namespace appsdk {
using nvm_key_id_t = pst::setting::key_id_t;
using nvm_value_t = pst::setting::SettingsValue;

class
AppSdkNVM
{
 private:
  using st_t = pst::setting::status_t;
  using st_result_t = struct {const char* mess; status_t result;};
  using chk_st_t = std::map<st_t, st_result_t>;

 public:
  AppSdkNVM();
  ~AppSdkNVM() = default;
  void Init(EventNotify);
  void Term(void);

  status_t SetNvmInt(nvm_key_id_t,int);
  status_t SetNvmBool(nvm_key_id_t,bool);
  status_t SetNvmString(nvm_key_id_t,std::string);
  status_t SetNvmString(nvm_key_id_t,const char*);
  status_t SetNvmBinary(nvm_key_id_t,const uint8_t*,size_t);

  status_t SetNvmIntOnUnResettable(nvm_key_id_t,int);
  status_t SetNvmBoolOnUnResettable(nvm_key_id_t,bool);
  status_t SetNvmStringOnUnResettable(nvm_key_id_t,std::string);
  status_t SetNvmStringOnUnResettable(nvm_key_id_t,const char*);
  status_t SetNvmBinaryOnUnResettable(nvm_key_id_t,const uint8_t*,size_t);

  status_t GetNvm(nvm_key_id_t,nvm_value_t*);

  status_t ResetAllNvmValues(void);

 private:
  status_t check_result(const char*,st_t);

 private:
  pst::setting::SettingsManager* mgr_;

  const static chk_st_t chk_status_;
};

}  // namespace appsdk
}  // namespace shared
}  // namespace apps

#endif  // APP_SHARED_INCLUDE_APPSDKNVM_H_
