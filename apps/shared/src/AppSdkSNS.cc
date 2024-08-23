//
// Copyright 2023 Sony Semiconductor Solutions Corporation.
//
#include <algorithm>
#include "AppSdkDefs-priv.h"
#include "GetManager.h"
#include "AsyncTask.h"
#include "base_memory.h"
#include "AppSdkSNSobserver-priv.h"
#define kCNAME "SNS"
#include "AppSdkSNS.h"

namespace apps {
namespace shared {
namespace appsdk {

const AppSdkSNS::type_to_size_t
AppSdkSNS::type_to_size_ =
{
  { pst::sensor::sensor_type_t::kWear, sizeof(pst::sensor::WearState) },
  { pst::sensor::sensor_type_t::kDnn1, sizeof(pst::sensor::DnnEventState) },
  { pst::sensor::sensor_type_t::kDnn2, sizeof(pst::sensor::DnnEventState) },
};
const AppSdkSNS::rcevent_t
AppSdkSNS::rcevent_ =
{
  event_t::kSensorDNN1,
  event_t::kSensorDNN2,
  event_t::kSensorDNN3,
  event_t::kSensorDNN4,
  event_t::kSensorDNN5,
  event_t::kSensorDNN6,
  event_t::kSensorDNN7,
  event_t::kSensorDNN8,
  event_t::kSensorDNN9,
};
const AppSdkSNS::type_to_stfunc_t
AppSdkSNS::type_to_stfunc_ =
{
  { pst::sensor::sensor_type_t::kWear, &AppSdkSNS::event_type_wear },
  { pst::sensor::sensor_type_t::kDnn1, &AppSdkSNS::event_type_dnn },
  { pst::sensor::sensor_type_t::kDnn2, &AppSdkSNS::event_type_dnn },
};
const AppSdkSNS::wst_event_t
AppSdkSNS::wst_to_event_ = {
  {pst::sensor::wear_state_t::kWearAttached, event_t::kWearAttached},
  {pst::sensor::wear_state_t::kWearDetached, event_t::kWearDetached}
};
const AppSdkSNS::type_list_t
AppSdkSNS::dnn_list_ =
{
  pst::sensor::sensor_type_t::kDnn1,
  pst::sensor::sensor_type_t::kDnn2,
};

const AppSdkSNS::chk_st_t
AppSdkSNS::chk_status_ = {
  { st_t::kSuccess,
    { "kSuccess",           status_t::kSuccess } },
  { st_t::kInternal,
    { "kInternal",          status_t::kSNS_kInternal } },
  { st_t::kInvalid,
    { "kInvalidParam",      status_t::kInvalidParameter } },
  { st_t::kInvalidState,
    { "kInvalidState",      status_t::kSNS_kInvalidState } },
};

JPVAR(AppSdkSNS::j_type_) =
{
  { pst::sensor::sensor_type_t::kWear, "kWear" },
  { pst::sensor::sensor_type_t::kDnn1, "kDnn1" },
  { pst::sensor::sensor_type_t::kDnn2, "kDnn2" },
};


void
AppSdkSNS::Term()
{
  LOGDEBUG();

  if(mgr_) {
    for(auto m : observer_tbl_) {
      m.second->observer->Deregist();
      delete m.second->observer;
      delete m.second;
    }
  }
}

void
AppSdkSNS::Init(
  EventNotify proc
)
{
  LOGDEBUG();

  mgr_ = &easel::GetManager<pst::sensor::SensorClient>();
  event_proc_ = proc;

  auto rg = [this](pst::sensor::sensor_type_t type, const char* name) {
    auto info = new observer_info_t;
    info->name = name;
    observer_tbl_.emplace(type, info);
    LOGINFO2("Init", "found type:%s(%d), name:%s", JP(j_type_, type), info->name.c_str(), "");
  };

  // Wear
  auto type = pst::sensor::sensor_type_t::kWear;
  rg(type, "");
  if(! regist_observer(type)) {
    LOGWARN("observer register error type:%s(%d)", JP(j_type_, type));
  }

  // DNN
  pst::sensor::SfpCapability param;
  for(auto t : dnn_list_) {
    if(status_t::kSuccess == CHK(mgr_->GetSensorState(t, &param))) {
      rg(t, reinterpret_cast<const char *>(param.model_name));
    } else {
      LOGWARN("not found type:%s(%d)", JP(j_type_, t));
    }
  }
}

bool
AppSdkSNS::regist_observer(
  pst::sensor::sensor_type_t type
)
{
  auto result = false;

  if(observer_tbl_.count(type)) {
    auto ob = observer_tbl_.at(type);
    auto func = type_to_stfunc_.at(type);
    ob->observer = new snsevent_observer_t(mgr_);
    if(status_t::kSuccess == ob->observer->Regist(
      type,
      type_to_size_.at(type),
      [this,func](pst::sensor::sensor_type_t t, pst::sensor::SensorState* d) {
        (this->*func)(t, d);
      })) {
      result = true;
    } else {
      ob->observer->Deregist();
      delete ob->observer;
      ob->observer = nullptr;
      ob->notify_func = nullptr;
    }
  }

  return(result);
}

status_t
AppSdkSNS::RegistSensorDetection(
  const char* name,
  sensor_statecb_t state_cb_f,
  sensor_initcb_t init_cb_f,
  sensor_dnn_config_t* config
)
{
  auto result = status_t::kModelNotFound;

  if(name == nullptr) {
    return(status_t::kInvalidParameter);
  }

  if(state_cb_f == nullptr) {
    result = status_t::kInvalidParameter;
  } else {
    for(auto type : dnn_list_) {
      if(observer_tbl_.count(type) == 0) {
        continue;
      }
      auto info = observer_tbl_.at(type);
      if(info->name.find(name) == std::string::npos) {
        LOGDEBUG("reg skip [%s]", info->name.c_str());
        continue;
      }
      if(info->notify_func) {
        result = status_t::kModelRegistered; 
        break;
      } else {
        info->notify_func = state_cb_f;
      }
      if(init_cb_f) {
        init_cb_f();
      }
      if(config) {
        CHK(mgr_->SetConfig(type, static_cast<const void*>(config)));
      }
      if(regist_observer(type)) {
        LOGDEBUG("register [%s] for %s(%d)", info->name.c_str(), JP(j_type_, type));
        result = status_t::kSuccess;
      } else {
        LOGERR("register error type:%s(%d)", JP(j_type_, type));
        result = status_t::kSNS_kInternal;
      }
      break;
    }
  }

  return(result);
}

status_t
AppSdkSNS::DeregistSensorDetection(
  const char* name,
  sensor_exitcb_t exit_cb_f
)
{
  auto result = status_t::kModelNotFound;

  if(name == nullptr) {
    return(status_t::kInvalidParameter);
  }

  for(auto type : dnn_list_) {
    if(observer_tbl_.count(type) == 0) {
      continue;
    }
    auto info = observer_tbl_.at(type);
    if(info->name.find(name) != std::string::npos) {
      if(info->observer == nullptr){
        result = status_t::kNotRegist;
        break;
      }
      result = info->observer->Deregist();
      if(result == status_t::kSuccess) {
        LOGDEBUG("deregister [%s] for %s(%d)", info->name.c_str(), JP(j_type_, type));
        if(exit_cb_f) {
          exit_cb_f();
        }
        delete info->observer;
        info->observer = nullptr;
        info->notify_func = nullptr;
      } else {
        LOGERR("deregister error type:%s(%d)", JP(j_type_, type));
      }
      break;
    } else {
      LOGDEBUG("dreg skip [%s]", info->name.c_str());
    }
  }

  return(result);
}

void
AppSdkSNS::event_type_wear(
  pst::sensor::sensor_type_t,
  pst::sensor::SensorState* data
)
{
  auto s = &data->sensordata.wear;

  if(wst_to_event_.count(s->state)) {
    event_proc_(wst_to_event_.at(s->state));
  }
}

void
AppSdkSNS::event_type_dnn(
  pst::sensor::sensor_type_t type,
  pst::sensor::SensorState* data
)
{
  if(observer_tbl_.count(type)) {
    auto p = observer_tbl_.at(type);
    if(p->notify_func) {
      auto s = &data->sensordata.dnn;
      auto rc = p->notify_func(s->classification, s->probability);
      if(std::find(rcevent_.begin(), rcevent_.end(), rc) != rcevent_.end()) {
        event_proc_(rc);
      } else {
        if(rc != event_t::kSensorDNNdonot) {
          LOGERR("not supported code:%d", rc);
        }
      }
    }
  }
}

status_t
AppSdkSNS::check_result(
  const char* funcname,
  st_t st
)
{
  auto result = status_t::kInvalidAPI;
  auto mess = "unknown value";

  if(chk_status_.count(st)) {
    mess = chk_status_.at(st).mess;
    result = chk_status_.at(st).result;
  }

  if(result != status_t::kSuccess) {
    LOGWARN2(funcname, "status=%s(%d)", mess, static_cast<int>(st), "");
  }

  return(result);
}

}  // namespace appsdk
}  // namespace shared
}  // namespace apps
