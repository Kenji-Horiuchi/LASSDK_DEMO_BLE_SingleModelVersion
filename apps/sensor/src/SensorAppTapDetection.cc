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

#define TAP_CLASSIFICATION_SINGLE     0
#define TAP_CLASSIFICATION_STABLE     1

#define TAP_QUEUE_SIZE         6   // set queing size
#define TAP_INIT_VALUE         1    // set stable value
#define CONFIRM_TAP            1    // set one-tap size
#define CONFIRM_BLANK          1    // set blank size between tap
#define REFRESH_SIZE           6   // set blank size after double tap
#define ACCURACY_THRESHOLD     0.6  // set accuracy to adopt (0~1)

namespace apps {
namespace sensor {

static bool g_recognition_flag = false;
static int  g_refresh_count = 0;

std::deque<uint32_t> TAP_QUEUE;

void SensorApp::init_TapDetection(){
  g_recognition_flag = false;
  g_refresh_count = 0;

  // init queue
  for (int i = 0; i < TAP_QUEUE_SIZE; ++i){
    TAP_QUEUE.push_front(TAP_CLASSIFICATION_STABLE);
  }
  PlayGuidance(shared::appsdk::effect_id_t::kFacetapDetectionStart);
  TWSS_RINFO(APPNAME"::%s FacetapDetectionStarted", __func__);
}

void SensorApp::callback_TapDetection(uint32_t classification, float probability){
  int tap_count;
  if(classification == TAP_CLASSIFICATION_SINGLE){
    TWSS_RINFO(APPNAME"::%s dnnrt result SINGLETAP", __func__);
  }else if(classification == TAP_CLASSIFICATION_STABLE){
    //TWSS_RINFO(APPNAME"::%s dnnrt result STABLE", __func__);
  }else{
    TWSS_ERR(APPNAME"::%s Unknown classification received", __func__);
  }
  
  tap_count = tap_filter(classification, probability);
  if(tap_count >= 2){
    PlayGuidance(shared::appsdk::effect_id_t::kDoubleTapDetected);
    TWSS_RINFO(APPNAME"::%s DoubleTapDetected", __func__);
  }
}

void SensorApp::exit_TapDetection(){
  // free queue
  for (int i = 0; i < TAP_QUEUE_SIZE; ++i){
    TAP_QUEUE.pop_back();
  }

  PlayGuidance(shared::appsdk::effect_id_t::kFacetapDetectionStop);
  TWSS_RINFO(APPNAME"::%s FacetapDetectionStopped", __func__);
}

int SensorApp::tap_filter(uint32_t classification, float probability){
  // data push
  if (static_cast<double>(probability) < ACCURACY_THRESHOLD){
    classification = -1;
  }
  TAP_QUEUE.push_front(classification);
  TAP_QUEUE.pop_back();

  uint32_t count = 0;
  int tap_sustain = 0;
  int blank_sustain = 0;
  bool flag = true;
  if(g_recognition_flag == true && classification == 0){
    for (int i = 0; i < TAP_QUEUE_SIZE; ++i){
      if(TAP_QUEUE[i] == 0 && flag == true){
        tap_sustain += 1;
        blank_sustain = 0;
        if(tap_sustain >= CONFIRM_TAP){
          flag = false;
          tap_sustain = 0;
          count += 1;
        }
      }else if(TAP_QUEUE[i] != 0 && flag == false){
        tap_sustain = 0;
        blank_sustain += 1;
        if(blank_sustain >= CONFIRM_BLANK){
          blank_sustain = 0;
          flag = true;
        }
      }else{
        tap_sustain = 0;
        blank_sustain = 0;
      }
    }
  }
  
  if(g_recognition_flag == false){
    if(g_refresh_count >= REFRESH_SIZE){
      g_refresh_count = 0;
      g_recognition_flag = true;
    }else{
      g_refresh_count += 1;
    }
  }

  if(count >= 2){
    g_recognition_flag = false;
  }

  return count;
}

}  // namespace sensor
}  // namespace apps