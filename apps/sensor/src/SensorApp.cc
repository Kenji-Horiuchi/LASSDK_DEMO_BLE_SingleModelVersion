//
// Copyright 2023 Sony Semiconductor Solutions Corporation.
//
#include <cstdio>
#include <thread>
#include <chrono>
#include <deque>
#include "SensorApp-priv.h"
#include "AsyncTask.h"

#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include "base_memory.h"

#include "AppSharedLog-priv.h"

#define APPNAME "SensorApp"

namespace apps {
namespace sensor {

namespace {
constexpr uint32_t kNumOfWorkerThreads = 0;
constexpr size_t kWorkerThreadsStackSize = 1024;
constexpr easel::SystemConfig kSystemConfig = {
  kNumOfWorkerThreads,
  kWorkerThreadsStackSize
};

int localMain(int argc, char** argv) {
  auto app = pst::base::make_unique<SensorApp>();
  app->SetSystemConfig(kSystemConfig);
  app->Main(APPNAME, argc, argv);
  return 0;
}

}  // namespace

const SensorApp::exit_tbl_t
SensorApp::exit_proc_ = {
  { action_t::kSequence1, &SensorApp::exit_dnnrt },
  { action_t::kSequence2, &SensorApp::exit_wear },
  { action_t::kSequence3, &SensorApp::exit_guidance },
  { action_t::kSequence4, &SensorApp::exit_sensorapp }
};

SensorApp::SensorApp() = default;
SensorApp::~SensorApp() = default;

void SensorApp::Boot() {
  TWSS_RINFO(APPNAME"::%s", __func__);
  dnn_model_num = MAX_DNN_MODEL_NUM;
  ringtone_status = 0;
  dnn_run = false;

  // single tap : dnnrt
  AppSdk::SetEventToCallback(
    shared::appsdk::event_t::kKeySingle,
    [this](shared::appsdk::event_t) {
      dnnrt_excute();
    });

  // long tap : exit
  AppSdk::SetEventToCallback(
    shared::appsdk::event_t::kKeyLong,
    [this](shared::appsdk::event_t) {
      SetEventToCallback(
        shared::appsdk::event_t::kFinGuidance,
        [this](shared::appsdk::event_t) {
          sequence_ctrl();
        }
        );
      next_action_number_ = action_t::kSequence1;
      sequence_ctrl();
    });

  wear_excute();

  PlayGuidance(shared::appsdk::effect_id_t::kSensorAppStart);
}

void SensorApp::Run() {
  TWSS_RINFO(APPNAME"::%s", __func__);
}

void SensorApp::Term() {
  TWSS_RINFO(APPNAME"::%s", __func__);
}

/* dnnrt */
void SensorApp::dnnrt_excute(void){
  TWSS_RINFO(APPNAME"::%s", __func__);
  shared::appsdk::status_t ret;
  for (auto i = 0; i < dnn_model_num; i++) {
    ret = RegistSensorDetection(
      dnn_model_table[i].model_name,
      [this,i](uint32_t c, float p) {
        (this->*(dnn_model_table[i].callbackFunction))(c,p);
        return(shared::appsdk::event_t::kSensorDNNdonot);
      },
      [this,i]() {
        (this->*(dnn_model_table[i].initFunction))();
      });
    if(ret == shared::appsdk::status_t::kSuccess){
      dnn_run = true;
      AppSdk::SetEventToCallback(
        shared::appsdk::event_t::kKeySingle,
        [this](shared::appsdk::event_t) {
          dnnrt_terminate();
        });
    }else if(ret == shared::appsdk::status_t::kSNS_kInternal){
      TWSS_RINFO(APPNAME"::%s RegistSensorDetection fail : %d", __func__, ret);
      (this->*(dnn_model_table[i].exitFunction))();
    }
  }
}

void SensorApp::dnnrt_terminate(void){
  TWSS_RINFO(APPNAME"::%s", __func__);
  for (auto i = 0; i < dnn_model_num; i++) {
    DeregistSensorDetection(
      dnn_model_table[i].model_name,
      [this,i]() {
        (this->*(dnn_model_table[i].exitFunction))();
      });
  }

  AppSdk::SetEventToCallback(
    shared::appsdk::event_t::kKeySingle,
    [this](shared::appsdk::event_t) {
      dnnrt_excute();
    });
  dnn_run = false;
}

/* wear action */
void SensorApp::wear_attatch(void){
    TWSS_RINFO(APPNAME"::%s", __func__);
}

void SensorApp::wear_detatch(void){
    TWSS_RINFO(APPNAME"::%s", __func__);
}

void SensorApp::wear_excute(void){
  TWSS_RINFO(APPNAME"::%s", __func__);

  AppSdk::SetEventToCallback(
    shared::appsdk::event_t::kWearAttached,
    [this](shared::appsdk::event_t) {
      wear_attatch();
    }
  );

  AppSdk::SetEventToCallback(
    shared::appsdk::event_t::kWearDetached,
    [this](shared::appsdk::event_t) {
      wear_detatch();
    }
  );

}

void SensorApp::wear_terminate(void){
  TWSS_RINFO(APPNAME"::%s", __func__);
  AppSdk::UnSetEventToCallback(
    shared::appsdk::event_t::kWearAttached
  );

  AppSdk::UnSetEventToCallback(
    shared::appsdk::event_t::kWearDetached
  );
}

/* app exit sequence */
void SensorApp::sequence_ctrl(void) {
  if(this->exit_proc_.count(next_action_number_)){
    (this->*exit_proc_.at(next_action_number_))();
  }
}

void SensorApp::exit_dnnrt(void) {
  next_action_number_ = action_t::kSequence2;
  if (dnn_run){
    dnnrt_terminate();
  }else{
    sequence_ctrl();
  }
}

void SensorApp::exit_wear(void) {
  next_action_number_ = action_t::kSequence3;
  wear_terminate();
  sequence_ctrl();
}

void SensorApp::exit_guidance(void) {
  next_action_number_ = action_t::kSequence4;
  PlayGuidance(shared::appsdk::effect_id_t::kSensorAppFinish);
}

void SensorApp::exit_sensorapp(void) {
  Exit();
}

}  // namespace sensor
}  // namespace apps

extern "C" int APP_MAIN(int argc, char** argv);
extern "C" int APP_MAIN(int argc, char** argv) {
  apps::sensor::localMain(argc, argv);
  return 0;
}
