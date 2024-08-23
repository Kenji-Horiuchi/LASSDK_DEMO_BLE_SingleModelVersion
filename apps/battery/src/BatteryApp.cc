//
// Copyright 2020,2021,2022 Sony Semiconductor Solutions Corporation.
//
#include "BatteryApp-priv.h"

#include <cstdio>

#include "AppSharedLog-priv.h"
#include "base_memory.h"
#include "BatteryTypes.h"
#include "BatteryEvents.h"
#include "BatteryManager.h"
#include "IEventManager.h"
#include "GetManager.h"
#include "PowerManager.h"
#include "Utility.h"

#define APPNAME "BatteryApp"
namespace {
const char* ConvState(const pst::battery::battery_state_t& state) {
  const char* s = nullptr;
  switch (state) {
#define STATE_STR(state) case pst::battery::battery_state_t::state: s = #state; break
    STATE_STR(kIdle);
    STATE_STR(kCharging);
    STATE_STR(kFull);
    STATE_STR(kHighTemperatureError);
    STATE_STR(kLowTemperatureError);
    STATE_STR(kTimeoutError);
    STATE_STR(kNoDeviceError);
    STATE_STR(kUnknownError);
  }
#undef STATE_STR
  return s;
}
const char* ConvBool(const bool& val) {
  const char* s = nullptr;
  
  if(val == false) {
    s = "FALSE";
  } else {
    s = "TRUE";
  }
  return s;
}
const char* ConvRet(const pst::battery::status_t& ret) {
  const char* s = nullptr;
  switch (ret) {
    case pst::battery::kSuccess:
      s = "Success";
      break;
    case pst::battery::kInternalError:
      s = "Internal Error or Not Supported";
      break;
    default:
      s = "NG DATA";
      break;
  }
  return s;
}
}  // namespace

namespace apps {
namespace battery {

namespace {
//constexpr auto kTransferConfirmResponse = pst::fwupdate::response_t::kAccept;

constexpr const uint32_t kNumOfWorkerThreads = 0;
constexpr const size_t kWorkerThreadsStackSize = 0;
constexpr const easel::SystemConfig kSystemConfig = {
  kNumOfWorkerThreads,
  kWorkerThreadsStackSize
};

int localMain(int argc, char *argv[]) {
  auto app = pst::base::make_unique<BatteryApp>();
  app->SetSystemConfig(kSystemConfig);
  app->Main("BatteryApp", argc, argv);
  return 0;
}
}  // namespace

BatteryApp::BatteryApp()
  :observer_(std::make_shared<MyBroadcastObserver>(this)) {
}

BatteryApp::~BatteryApp() = default;

void BatteryApp::RegisterEvent() {
  TWSS_RINFO("BatteryApp::%s", __func__);
  auto& em = easel::GetManager<pst::core::EventManager>();
  em.RegisterObserver(pst::battery::kEventBatteryStateChangeBroadcast, observer_);
  em.RegisterObserver(pst::battery::kEventChargeLevelChangeBroadcast, observer_);
  em.RegisterObserver(pst::battery::kEventLowBatteryVoltageBroadcast, observer_);
}

void BatteryApp::DeregisterEvent() {
  TWSS_RINFO("BatteryApp::%s", __func__);
  auto& em = easel::GetManager<pst::core::EventManager>();
  em.DeregisterObserver(pst::battery::kEventBatteryStateChangeBroadcast, *observer_);
  em.DeregisterObserver(pst::battery::kEventChargeLevelChangeBroadcast, *observer_);
  em.DeregisterObserver(pst::battery::kEventLowBatteryVoltageBroadcast, *observer_);
}

void BatteryApp::ToInitialize() {
  TWSS_RINFO("BatteryApp::%s", __func__);
  easel::GetManager<pst::power::PowerManager>().WakeLock(APPNAME);
  RegisterEvent();
}

void BatteryApp::ToActivate() {
  TWSS_RINFO("BatteryApp::%s", __func__);
}

void BatteryApp::ToWfe() {
  TWSS_RINFO("BatteryApp::%s", __func__);
}

void BatteryApp::ToRun() {
  TWSS_RINFO("BatteryApp::%s", __func__);
}

bool BatteryApp::ToInactivate() {
  TWSS_RINFO("BatteryApp::%s", __func__);
  return true;
}

void BatteryApp::ToFinalize() {
  TWSS_RINFO("BatteryApp::%s", __func__);
  easel::GetManager<pst::power::PowerManager>().WakeUnLock(APPNAME);
  DeregisterEvent();
}

void BatteryApp::OnNotify(const pst::core::Event& event) {
  const auto evid = event.GetEventId();
  auto& battery = easel::GetManager<pst::battery::BatteryManager>();
  pst::battery::status_t ret;

  if (evid == pst::battery::kEventBatteryStateChangeBroadcast) {
    pst::battery::battery_state_t state;
    state = event.GetExtra<pst::battery::battery_state_t>();
    std::printf("### change state [%s]\n", ConvState(state));
    ret = battery.GetBatteryState(state);
    std::printf("$ get state[%s] (%s)\n", ConvState(state), ConvRet(ret));
    uint32_t voltage_mv;
    ret = battery.GetBatteryVoltage(voltage_mv);
    std::printf("$ get voltage_mv[%d] (%s)\n", voltage_mv, ConvRet(ret));
    int32_t temperature_dc;
    ret = battery.GetThermistorTemperature(temperature_dc);
    std::printf("$ get temperature_dc[%d] (%s)\n", temperature_dc, ConvRet(ret));
  }

  if (evid == pst::battery::kEventChargeLevelChangeBroadcast) {
    std::printf("### change level:[%d]\n", event.GetExtra<uint32_t>());
    uint32_t level;
    ret = battery.GetBatteryLevel(level);
    std::printf("$ get level[%d] (%s)\n", level, ConvRet(ret));
  }

  if (evid == pst::battery::kEventLowBatteryVoltageBroadcast) {
    std::printf("### Low Battery Voltage Recived.\n");
    bool is_low_battery;
    ret = battery.GetLowBatteryVoltage(is_low_battery);
    std::printf("$ Low battery gotten info is %s. (%s)\n", ConvBool(is_low_battery), ConvRet(ret));
  }

  std::fflush(stdout);
}

}  // namespace battery
}  // namespace apps

extern "C" int APP_MAIN(int argc, char* argv[]);
extern "C" int APP_MAIN(int argc, char* argv[]) {
  apps::battery::localMain(argc, argv);
  return 0;
}
