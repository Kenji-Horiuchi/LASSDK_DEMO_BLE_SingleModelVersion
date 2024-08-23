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
#include "GetManager.h"

#include "IEventManager.h"
#include "SensorClient.h"
#include "ISensorServiceObserver.h"
#include "AppSharedLog-priv.h"

#define APPNAME "SensorApp"

#define ACTION_CLASSIFICATION_STABLE    0
#define ACTION_CLASSIFICATION_WALKING   1
#define ACTION_CLASSIFICATION_RUNNING   2


#define ACTION_QUEUE_SIZE         10  // set queing size
#define ACCURACY_THRESHOLD       0.6  // set accuracy to adopt (0~1)

#include "INetworkCommunicationObserver.h"
#include "NetworkCommunicationClient.h" // #include "NetworkCommunicationService.h"
#include "BLEApp-priv.h"

namespace apps {
namespace sensor {

std::deque<uint32_t> ACTION_QUEUE;

// static int g_ringtone_status = 0;

    
    // 今回（下から移動）
    static std::vector<uint8_t> send_data(1, 100); // 初期化時にデータを格納しないとうまく動かない // std::vector<uint8_t> send_data;
    static uint8_t connection_id = 1;
    static uint16_t size = 1;
    static std::string ClassificationStr = "";
    static GATT_DATA gatt_dataset(send_data, connection_id, size);
    // globalに記述するとエラーになる
    // auto& client = easel::GetManager<pst::netcom::NetworkCommunicationClient>();


void SensorApp::init_ActionDetection(){
  TWSS_RINFO(APPNAME"::%s", __func__);
  ringtone_status = 0;
  last_action_result = -1;
  for (int i = 0; i < ACTION_QUEUE_SIZE; ++i){
    ACTION_QUEUE.push_front(ACTION_CLASSIFICATION_STABLE);
  }

  PlayGuidance(shared::appsdk::effect_id_t::kActionDetectionStart);
  TWSS_RINFO(APPNAME"::%s ActionDetectionStarted", __func__);
}

void SensorApp::callback_ActionDetection(uint32_t classification, float probability){
  int action_result;

  // ***** これだけここに残す（globalに記述するとエラーになる）
  auto& client = easel::GetManager<pst::netcom::NetworkCommunicationClient>();

  if(classification == ACTION_CLASSIFICATION_STABLE){
    TWSS_RINFO(APPNAME"::%s dnnrt result STABLE:%.3f", __func__,static_cast<double>(probability));
    ClassificationStr = "STABLE";
  }else if(classification == ACTION_CLASSIFICATION_WALKING){
    TWSS_RINFO(APPNAME"::%s dnnrt result WALKING:%.3f", __func__,static_cast<double>(probability));
    ClassificationStr = "WALKING";
  }else if(classification == ACTION_CLASSIFICATION_RUNNING){
    TWSS_RINFO(APPNAME"::%s dnnrt result RUNNING:%.3f", __func__,static_cast<double>(probability));
    ClassificationStr = "RUNNING";
  }else{
    TWSS_ERR(APPNAME"::%s Unknown classification received", __func__);
    ClassificationStr = "OTHERS";
  }

  // ***** 検出結果にデータ書き換え
  gatt_dataset.SendData_StrToUint8(ClassificationStr, gatt_dataset.GetID(), gatt_dataset.GetSIZE()); // ID, SIZEは変更しない

  action_result = action_filter(classification, probability);
  if(action_result == ACTION_CLASSIFICATION_STABLE){
    if(last_action_result != action_result) {
      last_action_result = action_result;
      TWSS_RINFO(APPNAME"::%s filter result STABLE", __func__);

      // ***** BLEでデータ送信
      const auto status = client.SendData(gatt_dataset.GetID(), gatt_dataset.GetData(), gatt_dataset.GetSIZE()); // statusにデータ送信が成功したかの判定を格納
      TWSS_RINFO(APPNAME"::%s status[%s] connection_id[%d], *data[%d], size[%d]", __func__, status, gatt_dataset.GetID(), *gatt_dataset.GetData(), gatt_dataset.GetSIZE());

    }
    if(ringtone_status > 0){
      StopGuidance();
      // std::this_thread::sleep_for(std::chrono::milliseconds(200));
      TWSS_RINFO(APPNAME"::%s Guidance Stop", __func__);
    }
    ringtone_status = 0;
  }else if(action_result == ACTION_CLASSIFICATION_WALKING){
    if(last_action_result != action_result) {
      last_action_result = action_result;
      TWSS_RINFO(APPNAME"::%s filter result WALKING", __func__);

      // ***** BLEでデータ送信
      const auto status = client.SendData(gatt_dataset.GetID(), gatt_dataset.GetData(), gatt_dataset.GetSIZE()); // statusにデータ送信が成功したかの判定を格納
      TWSS_RINFO(APPNAME"::%s status[%s] connection_id[%d], *data[%d], size[%d]", __func__, status, gatt_dataset.GetID(), *gatt_dataset.GetData(), gatt_dataset.GetSIZE());
      
    }
    if(ringtone_status > 0 && ringtone_status != 1){
      StopGuidance();
      // std::this_thread::sleep_for(std::chrono::milliseconds(200));
      TWSS_RINFO(APPNAME"::%s Guidance Stop", __func__);
    }
    if(ringtone_status != 1){
      TWSS_RINFO(APPNAME"::%s Guidance Start", __func__);
      // PlayGuidance(shared::appsdk::effect_id_t::kWalkingSound, shared::appsdk::effect_mix_t::kDown, shared::appsdk::effect_pri_t::kLow, true);
      ringtone_status = 1;
    }
  }else if(action_result == ACTION_CLASSIFICATION_RUNNING){
    if(last_action_result != action_result) {
      last_action_result = action_result;
      TWSS_RINFO(APPNAME"::%s filter result RUNNING", __func__);

      // ***** BLEでデータ送信
      const auto status = client.SendData(gatt_dataset.GetID(), gatt_dataset.GetData(), gatt_dataset.GetSIZE()); // statusにデータ送信が成功したかの判定を格納
      TWSS_RINFO(APPNAME"::%s status[%s] connection_id[%d], *data[%d], size[%d]", __func__, status, gatt_dataset.GetID(), *gatt_dataset.GetData(), gatt_dataset.GetSIZE());
      
    }
    if(ringtone_status > 0 && ringtone_status != 2){
      StopGuidance();
      // std::this_thread::sleep_for(std::chrono::milliseconds(200));
      TWSS_RINFO(APPNAME"::%s Guidance Stop", __func__);
    }
    if(ringtone_status != 2){
      
      // PlayGuidance(shared::appsdk::effect_id_t::kRunningSound, shared::appsdk::effect_mix_t::kDown, shared::appsdk::effect_pri_t::kLow, true);
      TWSS_RINFO(APPNAME"::%s Guidance Start", __func__);
      ringtone_status = 2;
    }
  }
}

void SensorApp::exit_ActionDetection(){
  TWSS_RINFO(APPNAME"::%s", __func__);
  if(ringtone_status > 0){
    StopGuidance();
    //std::this_thread::sleep_for(std::chrono::milliseconds(200));
    TWSS_RINFO(APPNAME"::%s ActionDetectionStopped", __func__);
  }

  for (int i = 0; i < ACTION_QUEUE_SIZE; ++i){
    ACTION_QUEUE.pop_back();
  }
  
  PlayGuidance(shared::appsdk::effect_id_t::kActionDetectionStop);
  TWSS_RINFO(APPNAME"::%s", __func__);
}

int SensorApp::action_filter(uint32_t classification, float probability){
  // data push
  if (static_cast<double>(probability) < ACCURACY_THRESHOLD){
    classification = -1;
  }
  ACTION_QUEUE.push_front(classification);
  ACTION_QUEUE.pop_back();

  // filter処理
  int result = -1;
  bool filter = true;
  
  for (int i = 0; i < ACTION_QUEUE_SIZE; ++i){
    if(ACTION_QUEUE[i] != classification && filter == true){
      filter = false;
    }
  }

  if(filter){
    result = classification;
  }

  return result;
}

}  // namespace sensor
}  // namespace apps