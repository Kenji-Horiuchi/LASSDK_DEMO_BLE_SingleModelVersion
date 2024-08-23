//
// Copyright 2023 Sony Semiconductor Solutions Corporation.
//

#ifndef SENSOR_APP_SRC_SENSORSTREAMINGAPP_PRIV_H_
#define SENSOR_APP_SRC_SENSORSTREAMINGAPP_PRIV_H_

#include <vector>

#include "AppSdk.h"
#include "EventObserverHandle.h"
#include "HeadlessDaemonApplicationBase.h"

namespace apps {
namespace ssl {

class
SensorStreamingApp :
  public easel::HeadlessDaemonApplicationBase {

 public:
  SensorStreamingApp();
  virtual ~SensorStreamingApp();

 private:
  void start_process(void);
  void stop_process(void);
  void pause_process(void);
  void resume_process(void);

  void ToInitialize() override;
  void ToActivate() override;
//  void ToForeground() override;
//  void ToBackground() override;
  void ToWfe() override;
  void ToRun() override;
  bool ToInactivate() override;
  void ToFinalize() override;

  std::vector<pst::core::EventObserverHandle> handles_;
};

}  // namespace ssl
}  // namespace apps

#endif  // SENSOR_APP_SRC_SENSORSTREAMINGAPP_PRIV_H_
