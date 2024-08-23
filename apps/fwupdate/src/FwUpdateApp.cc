//
// Copyright 2020,2021,2022 Sony Semiconductor Solutions Corporation.
//
#include "FwUpdateApp-priv.h"

#include <cstdio>

#include "AppSharedLog-priv.h"
#include "base_memory.h"
#include "FwUpdateEvents.h"
#include "FwUpdateManager.h"
#include "GetManager.h"
#include "PowerManager.h"
#include "Utility.h"

#define APPNAME "FwUpdateApp"

namespace apps {
namespace fw_update {

namespace {
constexpr auto kTransferConfirmResponse = pst::fwupdate::response_t::kAccept;

constexpr const uint32_t kNumOfWorkerThreads = 0;
constexpr const size_t kWorkerThreadsStackSize = 0;
constexpr const easel::SystemConfig kSystemConfig = {
  kNumOfWorkerThreads,
  kWorkerThreadsStackSize
};

int localMain(int argc, char *argv[]) {
  auto app = pst::base::make_unique<FwUpdateApp>();
  app->SetSystemConfig(kSystemConfig);
  app->Main("FwUpdateApp", argc, argv);
  return 0;
}
}  // namespace

FwUpdateApp::FwUpdateApp() = default;

FwUpdateApp::~FwUpdateApp() = default;

void FwUpdateApp::RegisterEvent() {
  // TODO: Execute shutdown request in observer function.
#define REG_EVENT(ev_id, mode) \
    handles_.push_back(easel::RegisterEventObserver( \
        ev_id, \
        [this](const pst::core::Event&) { \
            TWSS_RINFO("FwUpdateApp %s received mode=%d", #ev_id, mode); \
            auto& fm = easel::GetManager<pst::fwupdate::FwUpdateManager>(); \
            fm.SetBootMode(mode); \
            auto& pm = easel::GetManager<pst::power::PowerManager>(); \
            pm.Reboot(); \
            Exit(); \
        }))

  REG_EVENT(pst::fwupdate::kBroadcastEventFwUpdate,
            pst::fwupdate::boot_mode_t::kFwUpdate);
  REG_EVENT(pst::fwupdate::kBroadcastEventFwErase,
            pst::fwupdate::boot_mode_t::kFwErase);
#undef REG_EVENT
}

void FwUpdateApp::DeregisterEvent() {
  handles_.clear();
}

void FwUpdateApp::ToInitialize() {
  TWSS_RINFO("FwUpdateApp::%s", __func__);
  easel::GetManager<pst::power::PowerManager>().WakeLock(APPNAME);
  RegisterEvent();
}

void FwUpdateApp::ToActivate() {
  TWSS_RINFO("FwUpdateApp::%s", __func__);

  if (!HasStartParameter<pst::core::event_id_t>() ||
      GetStartParameter<pst::core::event_id_t>() !=
          pst::fwupdate::kBroadcastEventFwTransferConfirm) {
    TWSS_RINFO("Invalid start parameter.");
    Exit();
    return;
  }

  auto& fm = easel::GetManager<pst::fwupdate::FwUpdateManager>();
  const auto result = fm.TransferStartResponse(kTransferConfirmResponse);
  if (kTransferConfirmResponse != pst::fwupdate::response_t::kAccept) {
    TWSS_RINFO("Reject transfer confirm.");
    Exit();
    return;
  }
  if (result != pst::fwupdate::status_t::kSuccess) {
    TWSS_RERR("FwUpdateManager::TransferStartResponse() error:%d",
        pst::base::AsInteger(result));
    Exit();
  }
}

void FwUpdateApp::ToWfe() {
  TWSS_RINFO("FwUpdateApp::%s", __func__);
}

void FwUpdateApp::ToRun() {
  TWSS_RINFO("FwUpdateApp::%s", __func__);
}

bool FwUpdateApp::ToInactivate() {
  TWSS_RINFO("FwUpdateApp::%s", __func__);
  return true;
}

void FwUpdateApp::ToFinalize() {
  TWSS_RINFO("FwUpdateApp::%s", __func__);
  easel::GetManager<pst::power::PowerManager>().WakeUnLock(APPNAME);
  DeregisterEvent();
}

}  // namespace fw_update
}  // namespace apps

extern "C" int APP_MAIN(int argc, char* argv[]);
extern "C" int APP_MAIN(int argc, char* argv[]) {
  apps::fw_update::localMain(argc, argv);
  return 0;
}
