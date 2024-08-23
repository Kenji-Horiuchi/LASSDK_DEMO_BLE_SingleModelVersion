//
// Copyright 2023 Sony Semiconductor Solutions Corporation.
//
#include <cstdio>
#include <thread>
#include <chrono>
#include <deque>
#include "SensorApp-priv.h"
#include "AsyncTask.h"

// #include <cstdlib>
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

// // #define ACTION_CLASSIFICATION_STABLE    1 // 0 // Stay
// // #define ACTION_CLASSIFICATION_WALKING   0 // 1 // Hand Writing
// // #define ACTION_CLASSIFICATION_RUNNING   2 // Shake
// #define ACTION_CLASSIFICATION_PLACED   3 // Placed // 今は使っていない
#define HEADGESTURE_CLASSIFICATION_STABLE    2 // 0
#define HEADGESTURE_CLASSIFICATION_VERTICAL   0 // 1
#define HEADGESTURE_CLASSIFICATION_HORIZONTAL   1 // 2


#define GESTURE_QUEUE_SIZE         5  // Head Gesture // 10  // set queing size
#define ACCURACY_THRESHOLD       0.6  // set accuracy to adopt (0~1)

// #include "NetworkCommunicationSample-priv.h"
#include "INetworkCommunicationObserver.h"
#include "NetworkCommunicationClient.h" // #include "NetworkCommunicationService.h"
#include "BLEApp-priv.h"

namespace apps {
  namespace sensor {

    std::deque<uint32_t> GESTURE_QUEUE; // 他のファイルでも同じ変数名で宣言してしまうと重複エラーが出る (staticをつければ大丈夫)

    // static int g_ringtone_status = 0;

    
    // 今回（下から移動）
    static std::vector<uint8_t> send_data(1, 100); // 初期化時にデータを格納しないとうまく動かない // std::vector<uint8_t> send_data;
    static uint8_t connection_id = 1;
    static uint16_t size = 1;
    static std::string ClassificationStr = "";
    static GATT_DATA gatt_dataset(send_data, connection_id, size);
    // globalに記述するとエラーになる
    // auto& client = easel::GetManager<pst::netcom::NetworkCommunicationClient>();


    void SensorApp::init_HeadGestureDetection(){
        TWSS_RINFO(APPNAME"::%s", __func__);
        ringtone_status = 0;
        last_action_result = -1;
        for (int i = 0; i < GESTURE_QUEUE_SIZE; ++i){
        GESTURE_QUEUE.push_front(HEADGESTURE_CLASSIFICATION_STABLE);
        }

        // PlayGuidance(shared::baselib::effect_id_t::kActionDetectionStart);
        //   PlayGuidance(shared::baselib::effect_id_t::kGoogleAssistantIsNotConnected);
        PlayGuidance(shared::appsdk::effect_id_t::kGoogleAssistantIsNotConnected);
        TWSS_RINFO(APPNAME"::%s HeadGestureDetectionStarted", __func__);
    }

    void SensorApp::callback_HeadGestureDetection(uint32_t classification, float probability){
      int action_result;

      // ***** これだけここに残す（globalに記述するとエラーになる）
      auto& client = easel::GetManager<pst::netcom::NetworkCommunicationClient>();

      if(classification == HEADGESTURE_CLASSIFICATION_STABLE){
        // TWSS_RINFO(APPNAME"::%s dnnrt result STABLE", __func__);
        TWSS_RINFO(APPNAME"::%s dnnrt result STABLE:%.3f", __func__,static_cast<double>(probability));
        ClassificationStr = "STABLE";
      }else if(classification == HEADGESTURE_CLASSIFICATION_VERTICAL){
        // TWSS_RINFO(APPNAME"::%s dnnrt result HAND WRITING", __func__);
        // TWSS_RINFO(APPNAME"::%s dnnrt result WALKING", __func__);
        // TWSS_RINFO(APPNAME"::%s dnnrt result VERTICAL MOTION", __func__);
        TWSS_RINFO(APPNAME"::%s dnnrt result VERTICAL MOTION:%.3f", __func__,static_cast<double>(probability));
        ClassificationStr = "WALKING"; // VERTICALに変更する // BLEApp.cc側も変更する必要がある
      }else if(classification == HEADGESTURE_CLASSIFICATION_HORIZONTAL){
        // TWSS_RINFO(APPNAME"::%s dnnrt result SHAKING", __func__);
        // TWSS_RINFO(APPNAME"::%s dnnrt result RUNNING", __func__);
        // TWSS_RINFO(APPNAME"::%s dnnrt result HORIZONTAL MOTION", __func__);
        TWSS_RINFO(APPNAME"::%s dnnrt result HORIZONTAL MOTION:%.3f", __func__,static_cast<double>(probability));
        ClassificationStr = "RUNNING"; // HORIZONTALに変更する // BLEApp.cc側も変更する必要がある

      // }else if(classification == ACTION_CLASSIFICATION_PLACED){
      //   TWSS_RINFO(APPNAME"::%s dnnrt result PLACED", __func__);
      //   ClassificationStr = "OTHERS";
      }else{
        TWSS_ERR(APPNAME"::%s Unknown classification received", __func__);
        ClassificationStr = "OTHERS";
      }
      
      // ***** 検出結果にデータ書き換え
      gatt_dataset.SendData_StrToUint8(ClassificationStr, gatt_dataset.GetID(), gatt_dataset.GetSIZE()); // ID, SIZEは変更しない

      action_result = gesture_filter(classification, probability);
      if(action_result == HEADGESTURE_CLASSIFICATION_STABLE){
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
      }else if(action_result == HEADGESTURE_CLASSIFICATION_VERTICAL){
        if(last_action_result != action_result) {
            last_action_result = action_result;
            TWSS_RINFO(APPNAME"::%s filter result VERTICAL", __func__);
            
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
          // PlayGuidance(shared::baselib::effect_id_t::kWalkingSound, shared::baselib::effect_mix_t::kDown, shared::baselib::effect_pri_t::kLow, true);
          ringtone_status = 1;
        }
      }else if(action_result == HEADGESTURE_CLASSIFICATION_HORIZONTAL){
        if(last_action_result != action_result) {
            last_action_result = action_result;
            TWSS_RINFO(APPNAME"::%s filter result HORIZONTAL", __func__);
            
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

          // PlayGuidance(shared::baselib::effect_id_t::kRunningSound, shared::baselib::effect_mix_t::kDown, shared::baselib::effect_pri_t::kLow, true);
          TWSS_RINFO(APPNAME"::%s Guidance Start", __func__);
          ringtone_status = 2;
        }
      }
    }

    void SensorApp::exit_HeadGestureDetection(){
        TWSS_RINFO(APPNAME"::%s", __func__);
        if(ringtone_status > 0){
            StopGuidance();
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            // TWSS_RINFO(APPNAME"::%s ActionDetectionStopped", __func__);
            TWSS_RINFO(APPNAME"::%s HeadGestureDetectionStopped", __func__);
        }

        for (int i = 0; i < GESTURE_QUEUE_SIZE; ++i){
            GESTURE_QUEUE.pop_back();
        }
        
        // 終了ガイダンス再生追加
        // PlayGuidance(shared::baselib::effect_id_t::kActionDetectionStop);
        PlayGuidance(shared::appsdk::effect_id_t::kActionDetectionStop);
        TWSS_RINFO(APPNAME"::%s", __func__);
    }

    int SensorApp::gesture_filter(uint32_t classification, float probability){
      // data push
      if (static_cast<double>(probability) < ACCURACY_THRESHOLD){
        classification = -1;
      }
      GESTURE_QUEUE.push_front(classification);
      GESTURE_QUEUE.pop_back();

      // filter処理
      int result = -1;
      bool filter = true;
      
      for (int i = 0; i < GESTURE_QUEUE_SIZE; ++i){
        if(GESTURE_QUEUE[i] != classification && filter == true){
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