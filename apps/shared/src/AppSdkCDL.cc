//
// Copyright 2023 Sony Semiconductor Solutions Corporation.
//
#include <algorithm>
#include "GetManager.h"
#define kCNAME "CDL"
#include "AppSdkCDL.h"

namespace apps {
namespace shared {
namespace appsdk {

const auto key_boot =
    pst::volatilesetting::key_id_t(PST_STATIC_ID("appsdk.boot"));

const AppSdkCDL::rcevent_t
AppSdkCDL::rcevent_ =
{
  event_t::kCradleEvent1,
  event_t::kCradleEvent2,
  event_t::kCradleEvent3,
  event_t::kCradleEvent4,
  event_t::kCradleEvent5,
  event_t::kCradleEvent6,
  event_t::kCradleEvent7,
  event_t::kCradleEvent8,
  event_t::kCradleEvent9,
};

const AppSdkCDL::cdl_event_t
AppSdkCDL::cdl_to_event_ = {
  { pst::cradle::cradle_state_t::kCradleConnected, event_t::kCradleConnect },
  { pst::cradle::cradle_state_t::kCradleDisconnected, event_t::kCradleDisconnect },
};

const AppSdkCDL::chk_st_t
AppSdkCDL::chk_status_ = {
  { st_t::kSuccess,
    { "kSuccess",status_t::kSuccess } },
  { st_t::kInternal,
    { "kInternal",status_t::kCDL_kInternal } },
  { st_t::kInvalid,
    { "kInvalid",status_t::kCDL_kInvalid } },
  { st_t::kInvalidState,
    { "kInvalidState",status_t::kCDL_kInvalidState } },
};

AppSdkCDL::AppSdkCDL(void) :
  notify_func_gp_(nullptr),notify_func_fd_(nullptr)
{

}

bool
AppSdkCDL::Init(
  EventNotify proc
)
{
  LOGINFO();

#define REG_EVENT(ev_id) \
    handles_.push_back(easel::RegisterEventObserver( \
        ev_id, \
        [this](const pst::core::Event& event) { \
            LOGDEBUG("%s received", #ev_id); \
            on_notify_event(event); \
        }))
  REG_EVENT(pst::cradle::kCradleConnected);
  REG_EVENT(pst::cradle::kCradleDisconnected);
#undef REG_EVENT

  event_proc_ = proc;

  mgr_ = &easel::GetManager<pst::cradle::CradleClient>();

  bool b = false;
  vol_mgr_ = &easel::GetManager<pst::volatilesetting::VolatileSettingsManager>();
  vol_mgr_->Get(key_boot, &b);

  status_t ret = status_t::kSuccess;
  if(!b){
    LOGINFO("exec EnableCradle");
    ret = CHK(mgr_->EnableCradle());
  }

  if(ret==status_t::kSuccess){
    LOGDEBUG("exec RegisterObserver");
    CHK(mgr_->RegisterObserver(this));
  }

  return(true);
}

void
AppSdkCDL::Term(
  void
)
{
  LOGINFO();
  CHK(mgr_->DeregisterObserver(this));
}

void AppSdkCDL::OnNotifyGpRequest(pst::cradle::GpCommandParameter *command) {
  LOGINFO();

  event_type_gp(command);
  mgr_->RequestGpResponse();
}

void AppSdkCDL::OnNotifyFdRequest(
    pst::cradle::FdCommandParameter *command) {
  LOGINFO();

  int ret = event_type_fd(command);
  mgr_->RequestFdResponse(ret);

  return;
}

status_t
AppSdkCDL::RegisterCradleGpCommand(
  cdl_gp_statecb_t state_cb_f
)
{
  auto result = status_t::kInvalidAPI;
  if(state_cb_f == nullptr) {
    result = status_t::kInvalidParameter;
  } else {
      if(notify_func_gp_!=nullptr) {
        result = status_t::kCradleRegistered; 
      } else {
        notify_func_gp_ = state_cb_f;
        result = status_t::kSuccess;
      }
  }

  return(result);
}

status_t
AppSdkCDL::DeregisterCradleGpCommand(
  void
)
{
  auto result = status_t::kInvalidAPI;

  if(!notify_func_gp_) {
    result = status_t::kNotRegist; 
  } else {
    notify_func_gp_ = nullptr;
    result = status_t::kSuccess;
  }

  return(result);
}

status_t
AppSdkCDL::RegisterFdCommand(
  cdl_fd_statecb_t state_cb_f
)
{
  auto result = status_t::kInvalidAPI;
  if(state_cb_f == nullptr) {
    result = status_t::kInvalidParameter;
  } else {
      if(notify_func_fd_!=nullptr) {
        result = status_t::kCradleRegistered; 
      } else {
        notify_func_fd_ = state_cb_f;
        result = status_t::kSuccess;
      }
  }

  return(result);
}

status_t
AppSdkCDL::DeregisterFdCommand(
  void
)
{
  auto result = status_t::kInvalidAPI;

  if(!notify_func_fd_) {
    result = status_t::kNotRegist; 
  } else {
    notify_func_fd_ = nullptr;
    result = status_t::kSuccess;
  }

  return(result);
}

void
AppSdkCDL::SendFdData(
  fd_write_param_t sendparam
)
{
  mgr_->SendFdData(std::move(sendparam));
}

int
AppSdkCDL::event_type_fd(
  pst::cradle::FdCommandParameter *command
)
{
  int ret = -1;
  if(notify_func_fd_) {
    auto rc = notify_func_fd_(command);
    if(rc!=event_t::kCradleInvalidFdCommand){
      LOGDEBUG("recieved code:%d", rc);
      event_proc_(rc);
      ret = 0;
    }else{
      ret = -1;
      LOGDEBUG("recieved invalid fd command");
    }
  }
  return ret;
}

void
AppSdkCDL::event_type_gp(
  pst::cradle::GpCommandParameter *command
)
{
  if(notify_func_gp_) {
    auto rc = notify_func_gp_(command);
    if(std::find(rcevent_.begin(), rcevent_.end(), rc) != rcevent_.end()) {
      event_proc_(rc);
    } else {
      if(rc != event_t::kCradleEventdonot) {
        LOGERR("not supported code:%d", rc);
      } else {
        LOGDEBUG("cradle event do not");
      }
    }
  }else{
    LOGDEBUG("no processing function registered");
    command->parameter[0] = 0xFF;
    command->parameter[1] = 0x00;
    command->parameter[2] = 0x00;
    command->datasize = 0;
  }
}

void AppSdkCDL::on_notify_event(const pst::core::Event& event) {
  LOGINFO();
  const auto event_id = event.GetEventId();
  pst::cradle::cradle_state_t st;
  if (event_id == pst::cradle::kCradleConnected) {
    st = pst::cradle::cradle_state_t::kCradleConnected;
  }else if (event_id == pst::cradle::kCradleDisconnected) {
    st = pst::cradle::cradle_state_t::kCradleDisconnected;
  }

  if(cdl_to_event_.count(st)) {
    event_proc_(cdl_to_event_.at(st));
  } else {
    LOGINFO("not notify");
  }
}

status_t
AppSdkCDL::check_result(
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
