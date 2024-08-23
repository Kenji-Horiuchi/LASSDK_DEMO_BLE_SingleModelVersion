//
// Copyright 2023 Sony Semiconductor Solutions Corporation.
//
#include <cstdio>

#include "SensorStreamingApp-priv.h"
#include "GetManager.h"
#include "BroadcastEvent.h"

#include "SensorStreamingManager.h"
#include "SensorStreamingTypes.h"
#include "AppSharedLog-priv.h"
#include "SensorStreamingEvents.h"

#define APPNAME "SensorStreamingApp"

namespace apps {
namespace ssl {

namespace {
constexpr uint32_t kNumOfWorkerThreads = 0;
constexpr size_t kWorkerThreadsStackSize = 1024;
constexpr easel::SystemConfig kSystemConfig = {
  kNumOfWorkerThreads,
  kWorkerThreadsStackSize
};

int localMain(int argc, char** argv) {
  auto app = pst::base::make_unique<SensorStreamingApp>();
  app->SetSystemConfig(kSystemConfig);
  app->Main(APPNAME, argc, argv);
  return 0;
}

}  // namespace

SensorStreamingApp::SensorStreamingApp() = default;
SensorStreamingApp::~SensorStreamingApp() = default;

static uint8_t ConnectionID;
//static uint8_t ConnectionID[2];

static pst::ssl::streaming_status_t StreamingStatus;
//static streaming_status_t StreamingStatus[2];

void SensorStreamingApp::start_process(void){
  TWSS_RINFO(APPNAME"::%s", __func__);
  auto& manager = easel::GetManager<pst::ssl::SensorStreamingManager>();
  auto ret = manager.ReadRequestConnectionID(&ConnectionID);
      ret = manager.StartRequestResponse(ConnectionID,pst::ssl::streaming_status_t::kEnable);
  StreamingStatus = pst::ssl::streaming_status_t::kEnable;
  if(ret != pst::ssl::status_t::kSuccess){
    TWSS_ERR(APPNAME"::%s start_process %d", __func__, static_cast<int32_t>(ret));
  }
}

void SensorStreamingApp::stop_process(void){
  TWSS_RINFO(APPNAME"::%s", __func__);
  auto& manager = easel::GetManager<pst::ssl::SensorStreamingManager>();
  auto ret = manager.ReadRequestConnectionID(&ConnectionID);
      ret = manager.StopRequestResponse(ConnectionID,pst::ssl::streaming_status_t::kDisable);
  if(ret != pst::ssl::status_t::kSuccess){
    TWSS_ERR(APPNAME"::%s stop_process %d", __func__, pst::ssl::streaming_status_t::StreamingStatus);
  }

  StreamingStatus = pst::ssl::streaming_status_t::kDisable;
  ConnectionID = 0;
  Exit();
}

void SensorStreamingApp::pause_process(void){
  TWSS_RINFO(APPNAME"::%s", __func__);
//Streaming Status check
  if(StreamingStatus == pst::ssl::streaming_status_t::kEnable){
    auto& manager = easel::GetManager<pst::ssl::SensorStreamingManager>();
    auto ret = manager.SetStreamingStatus(ConnectionID,pst::ssl::streaming_status_t::kPause);
    StreamingStatus = pst::ssl::streaming_status_t::kPause;
    if(ret != pst::ssl::status_t::kSuccess){
      TWSS_ERR(APPNAME"::%s SensorStreamingApp::pause_process %d", __func__, static_cast<int32_t>(ret));
    }
  }else{
      TWSS_ERR(APPNAME"::%s pause_process not kEnable %d", __func_);
  }
}

void SensorStreamingApp::resume_process(void){
  TWSS_RINFO(APPNAME"::%s", __func__);
//Streaming Status check
  if(StreamingStatus == pst::ssl::streaming_status_t::kPause){
    auto& manager = easel::GetManager<pst::ssl::SensorStreamingManager>();
    auto ret = manager.SetStreamingStatus(ConnectionID,pst::ssl::streaming_status_t::kEnable);
    StreamingStatus = pst::ssl::streaming_status_t::kEnable;
    if(ret != pst::ssl::status_t::kSuccess){
      TWSS_ERR(APPNAME"::%s SensorStreamingApp::resume_process %d", __func__, static_cast<int32_t>(ret));
    }
  }else{
      TWSS_ERR(APPNAME"::%s resume_process not connectted %d", __func_);
  }
}

void SensorStreamingApp::ToInitialize() {
  TWSS_RINFO("SensorStreamingApp::%s", __func__);
  StreamingStatus = pst::ssl::streaming_status_t::kDisable;
  ConnectionID = 0;
}

void SensorStreamingApp::ToActivate() {
  TWSS_RINFO("SensorStreamingApp::%s", __func__);
  if (!HasStartParameter<pst::core::event_id_t>() ||
      GetStartParameter<pst::core::event_id_t>() !=
          pst::ssl::kBroadcastSensorStartRequest) {
    TWSS_RINFO("Invalid start parameter.");
    Exit();
    return;
  }
  start_process();

  handles_.push_back(easel::RegisterEventObserver(
      pst::ssl::kBroadcastSensorStopRequest,
      [this](const pst::core::Event& /*event*/) {
          TWSS_RINFO("SensorStreamingAPP kBroadcastSensorStartRequest received");
          stop_process();
          return;
      }));

//  AppSdk::SetEventToCallback(
//    shared::appsdk::event_t::kStreamUsed,
//    [this](shared::appsdk::event_t) {
//      pause_process();
//    });

//  AppSdk::SetEventToCallback(
//    shared::appsdk::event_t::kStreamFree,
//    [this](shared::appsdk::event_t) {
//      resume_process();
//   });
}

//void SensorStreamingApp::ToForeground() {
//  TWSS_RINFO("SensorStreamingApp::%s", __func__);
//}

//void SensorStreamingApp::ToBackground() {
//  TWSS_RINFO("SensorStreamingApp::%s", __func__);
//}

void SensorStreamingApp::ToWfe() {
  TWSS_RINFO("SensorStreamingApp::%s", __func__);
}

void SensorStreamingApp::ToRun() {
  TWSS_RINFO("SensorStreamingApp::%s", __func__);
  return;
 }

bool SensorStreamingApp::ToInactivate() {
  TWSS_RINFO("SensorStreamingApp::%s", __func__);
  return true;
}

void SensorStreamingApp::ToFinalize() {
  TWSS_RINFO("SensorStreamingApp::%s", __func__);
  // stop process
  if(StreamingStatus != pst::ssl::streaming_status_t::kDisable){
    stop_process();
    StreamingStatus = pst::ssl::streaming_status_t::kDisable;
    ConnectionID = 0;
  }
}

}  // namespace sensor
}  // namespace apps

extern "C" int APP_MAIN(int argc, char** argv);
extern "C" int APP_MAIN(int argc, char** argv) {
  apps::ssl::localMain(argc, argv);
  return 0;
}
