//
// Copyright 2023 Sony Semiconductor Solutions Corporation.
//
#include <cstdio>
#include <thread>
#include <chrono>
#include "SensorApp-priv.h"
#include "AsyncTask.h"

#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include "GetManager.h"

#include "IEventManager.h"
#include "SensorClient.h"
#include "ISensorServiceObserver.h"
#include "AppSharedLog-priv.h"

#define APPNAME "SensorApp"

namespace apps {
namespace sensor {

void SensorApp::init_Undefined(){
  TWSS_RINFO(APPNAME"::%s", __func__);
}

void SensorApp::callback_Undefined(uint32_t classification, float probability){
    TWSS_RINFO(APPNAME"::%s dnnrt result classification: %d, probability: %.3f", __func__, classification, static_cast<double>(probability));
}

void SensorApp::exit_Undefined(){
  TWSS_RINFO(APPNAME"::%s", __func__);
}

}  // namespace sensor
}  // namespace apps