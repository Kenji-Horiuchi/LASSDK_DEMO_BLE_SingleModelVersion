//
// Copyright 2020,2021 Sony Semiconductor Solutions Corporation.
//

#ifndef FIRMWARE_SRC_APPS_BATTERY_APP_SRC_BATTERYAPP_PRIV_H_
#define FIRMWARE_SRC_APPS_BATTERY_APP_SRC_BATTERYAPP_PRIV_H_

#include <memory>
#include <vector>

#include "BroadcastEventObserver.h"
#include "Event.h"
#include "HeadlessDaemonApplicationBase.h"

namespace apps {
namespace battery {

class BatteryApp : public easel::HeadlessDaemonApplicationBase {
 public:
  BatteryApp();
  virtual ~BatteryApp();

 private:
  void RegisterEvent();
  void DeregisterEvent();
  void ToInitialize() override;
  void ToActivate() override;
  void ToWfe() override;
  void ToRun() override;
  bool ToInactivate() override;
  void ToFinalize() override;

private:
  void OnNotify(const pst::core::Event& event);
  
  class MyBroadcastObserver final : public pst::core::BroadcastEventObserver {
   public:
    explicit MyBroadcastObserver(
        BatteryApp* parent) : parent_(parent) {}
    virtual ~MyBroadcastObserver() = default;
    void Notify(const pst::core::Event& event) override {
      parent_->OnNotify(event);
    }
   private:
    BatteryApp* parent_;
  };

  std::shared_ptr<MyBroadcastObserver> observer_;
};

}  // namespace fw_update
}  // namespace apps

#endif  //  FIRMWARE_SRC_APPS_BATTERY_APP_SRC_BATTERYAPP_PRIV_H_
