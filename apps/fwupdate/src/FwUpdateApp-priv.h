//
// Copyright 2020,2021 Sony Semiconductor Solutions Corporation.
//

#ifndef FIRMWARE_SRC_APPS_FW_UPDATE_APP_SRC_FWUPDATEAPP_PRIV_H_
#define FIRMWARE_SRC_APPS_FW_UPDATE_APP_SRC_FWUPDATEAPP_PRIV_H_

#include <memory>
#include <vector>

#include "BroadcastEvent.h"
#include "HeadlessDaemonApplicationBase.h"

namespace apps {
namespace fw_update {

class FwUpdateApp : public easel::HeadlessDaemonApplicationBase {
 public:
  FwUpdateApp();
  virtual ~FwUpdateApp();

 private:
  void RegisterEvent();
  void DeregisterEvent();
  void ToInitialize() override;
  void ToActivate() override;
  void ToWfe() override;
  void ToRun() override;
  bool ToInactivate() override;
  void ToFinalize() override;

  std::vector<pst::core::EventObserverHandle> handles_;
};

}  // namespace fw_update
}  // namespace apps

#endif  //  FIRMWARE_SRC_APPS_FW_UPDATE_APP_SRC_FWUPDATEAPP_PRIV_H_
