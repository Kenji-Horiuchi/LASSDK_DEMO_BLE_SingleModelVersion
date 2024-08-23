//
// Copyright 2023 Sony Semiconductor Solutions Corporation.
//
#ifndef APP_SHARED_INCLUDE_APPSDK_H_
#define APP_SHARED_INCLUDE_APPSDK_H_

#include <cstdint>
#include <map>
#include <memory>

#include "HeadlessApplicationBase.h"
#include "VolatileSettingsManager.h"
#include "PowerManager.h"

#include "AppSdkType.h"
#include "AppSdkAUD.h"
#include "AppSdkNET.h"
#include "AppSdkSNS.h"
#include "AppSdkCDL.h"
#include "AppSdkPWR.h"
#include "AppSdkNVM.h"
#include "AppSdkBAT.h"
#include "AppSdkLOG-priv.h"

namespace apps {
namespace shared {
namespace appsdk {

class AppSdkKEY;

class
AppSdk :
  public easel::HeadlessApplicationBase,
  public AppSdkAUD,
  public AppSdkNET,
  public AppSdkSNS,
  public AppSdkCDL,
  public AppSdkBAT,
  public AppSdkPWR,
  public AppSdkNVM
{
 public:
  AppSdk();
  ~AppSdk();

  easel::status_t Main(const char* const app_name, int argc, char** argv);
  virtual void Boot(void) {};
  virtual void Run(void) {};
  virtual void Term(void) {};

  status_t SetEventToAction(event_t,action_t);
  inline status_t UnSetEventToAction(event_t e) {
    return(SetEventToAction(e,action_t::kClear));
  }
  status_t SetEventToCallback(event_t,EventNotify);
  inline status_t UnSetEventToCallback(event_t e) {
    return(SetEventToCallback(e,nullptr));
  }

  status_t FactoryReset(void);

 private:
  using act_tbl_t = std::map<action_t, void (AppSdk::*) (action_t)>;
  using hook_tbl_t = std::map<event_t, event_t (AppSdk::*) (event_t)>;

  bool OnKeyEvent(pst::key::keycode_t) override;
  void ToInitialize(void) override;
  void ToActivate(void) override;
//  void ToForeground(void) override;
//  void ToBackground(void) override;
  void ToWfe(void) override;
  void ToRun(void) override;
  bool ToInactivate(void) override;
  void ToFinalize(void) override;
  void kick_power(action_t);

  void analyze_event(event_t);
  void kick_network(action_t);
  void kick_audio(action_t);
  event_t ring_start(event_t);
  event_t ring_stop(event_t);

  bool is_active_ = false;
  pst::volatilesetting::VolatileSettingsManager* vol_mgr_ = nullptr;
  pst::power::PowerManager* pwr_mgr_ = nullptr;

  std::unique_ptr<class AppSdkKEY> key_;

  std::map<event_t, action_t> event_to_action_;
  std::map<event_t, EventNotify> event_to_proc_;
  std::unique_ptr<class AppSdkSNS> sns_wear_;
  std::unique_ptr<class AppSdkSNS> sns_dnn1_;
  std::unique_ptr<class AppSdkSNS> sns_dnn2_;

  const static act_tbl_t action_proc_;
  const static std::map<event_t, bool> invalid_event_;
  const static hook_tbl_t hook_event_;
  std::string pwr_lock_;

  JPDEF(event_t, j_event_);
  JPDEF(action_t, j_action_);
};

}  // namespace appsdk
}  // namespace shared
}  // namespace apps

#endif  // APP_SHARED_INCLUDE_APPSDK_H_
