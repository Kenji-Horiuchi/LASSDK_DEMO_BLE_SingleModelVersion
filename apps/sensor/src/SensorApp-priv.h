//
// Copyright 2023 Sony Semiconductor Solutions Corporation.
//

#ifndef SENSOR_APP_SRC_SENSORAPP_PRIV_H_
#define SENSOR_APP_SRC_SENSORAPP_PRIV_H_

#include <vector>

#include "AppSdk.h"

#ifndef MAX_DNN_MODEL_NUM
#define MAX_DNN_MODEL_NUM  (3)
#endif

namespace apps {
namespace sensor {

enum class
action_t
{
  kSequence0,
  kSequence1,
  kSequence2,
  kSequence3,
  kSequence4,
};

class
SensorApp :
  public apps::shared::appsdk::AppSdk
{
 public:
  SensorApp();
  virtual ~SensorApp();

 private:
  void Boot(void) override;
  void Run(void) override;
  void Term(void) override;
//   void ToInitialize() override;
//   void ToActivate() override;
// //  void ToForeground() override;
// //  void ToBackground() override;
// //  void ToWfe() override;
// //  void ToRun() override;
//   bool ToInactivate() override;
// //  void ToFinalize() override;

 private:

  void sequence_ctrl(void);
  void exit_dnnrt(void);
  void exit_wear(void);
  void exit_guidance(void);
  void exit_sensorapp(void);
  
  using exit_tbl_t = std::map<action_t, void (SensorApp::*) (void)>;
  
  const static exit_tbl_t exit_proc_;
  action_t next_action_number_;

  void dnnrt_excute(void);
  void dnnrt_terminate(void);

  void wear_excute(void);
  void wear_terminate(void);
  void wear_attatch(void);
  void wear_detatch(void);

  // SensorAppActionDetection.cc
  void init_ActionDetection(void);
  void callback_ActionDetection(uint32_t classification, float probability);
  void exit_ActionDetection(void);
  int action_filter(uint32_t classification, float probability);


  // SensorAppTapDetection.cc
  void init_TapDetection(void);
  void callback_TapDetection(uint32_t classification, float probability);
  void exit_TapDetection(void);
  int tap_filter(uint32_t classification, float probability);

  // SensorAppUndefined.cc
  void init_Undefined(void);
  void callback_Undefined(uint32_t classification, float probability);
  void exit_Undefined(void);

  // 2024/05/30
  // SensorAppHeadGestureDetection.cc
  void init_HeadGestureDetection(void);
  void callback_HeadGestureDetection(uint32_t classification, float probability);
  void exit_HeadGestureDetection(void);
  int gesture_filter(uint32_t classification, float probability);

  typedef struct {
    const char*  model_name;
    void  (apps::sensor::SensorApp::*initFunction)(void);
    void  (apps::sensor::SensorApp::*exitFunction)(void);
    void  (apps::sensor::SensorApp::*callbackFunction)(uint32_t classification, float probability);
  } dnn_model_table_t;

  
  
  
  
  
  // dnn_model_table_t dnn_model_table[MAX_DNN_MODEL_NUM] = {
  //   {"ActionDetection",   &apps::sensor::SensorApp::init_ActionDetection,   &apps::sensor::SensorApp::exit_ActionDetection,   &apps::sensor::SensorApp::callback_ActionDetection  },
  //   {"TapDetection",      &apps::sensor::SensorApp::init_TapDetection,      &apps::sensor::SensorApp::exit_TapDetection,      &apps::sensor::SensorApp::callback_TapDetection     },
  //   {"",                  &apps::sensor::SensorApp::init_Undefined,         &apps::sensor::SensorApp::exit_Undefined,         &apps::sensor::SensorApp::callback_Undefined        }
  // };

  // 2024/04/22 HeadGestureDetection
  dnn_model_table_t dnn_model_table[MAX_DNN_MODEL_NUM] = {
    {"ActionDetection",       &apps::sensor::SensorApp::init_ActionDetection,       &apps::sensor::SensorApp::exit_ActionDetection,       &apps::sensor::SensorApp::callback_ActionDetection      },
    {"HeadGestureDetection",  &apps::sensor::SensorApp::init_HeadGestureDetection,  &apps::sensor::SensorApp::exit_HeadGestureDetection,  &apps::sensor::SensorApp::callback_HeadGestureDetection },
    {"",                  &apps::sensor::SensorApp::init_Undefined,                 &apps::sensor::SensorApp::exit_Undefined,             &apps::sensor::SensorApp::callback_Undefined            }
  };

  // pst::soundpool::resource_id_t sound_id_;

  int dnn_model_num;
  int ringtone_status;
  bool dnn_run;
  int last_action_result;
};

}  // namespace sensor
}  // namespace apps

#endif  // SENSOR_APP_SRC_SENSORAPP_PRIV_H_
