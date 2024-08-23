//
// Copyright 2023 Sony Semiconductor Solutions Corporation.
//
#include <cstdio>
#include "BootApp-priv.h"
#include "AppSharedLog-priv.h"

#include "BroadcastEvent.h"
#include "GetManager.h"

#define APPNAME "BootApp"

namespace apps {
namespace boot {

namespace {
constexpr uint32_t kNumOfWorkerThreads = 0;
constexpr size_t kWorkerThreadsStackSize = 1024;
constexpr easel::SystemConfig kSystemConfig = {
  kNumOfWorkerThreads,
  kWorkerThreadsStackSize
};

int
localMain(
  int argc,
  char** argv
)
{
  auto app = pst::base::make_unique<BootApp>();
  app->SetSystemConfig(kSystemConfig);
  app->Main(APPNAME, argc, argv);
  return 0;
}

}  // namespace

BootApp::BootApp() = default;
BootApp::~BootApp() = default;

void BootApp::Boot() {
  TWSS_RINFO(APPNAME"::%s", __func__);

  SetEventToCallback(
    shared::appsdk::event_t::kFinGuidance,
    [this](shared::appsdk::event_t) {
      Exit();
    }
    );
}

void BootApp::Run() {
  TWSS_RINFO(APPNAME"::%s", __func__);

  PlayGuidance(shared::appsdk::effect_id_t::kPowerOn);
}

void BootApp::Term() {
  TWSS_RINFO(APPNAME"::%s", __func__);
}

}  // namespace boot
}  // namespace apps

extern "C" int APP_MAIN(int argc, char* argv[]);
extern "C" int APP_MAIN(int argc, char* argv[]) {
  apps::boot::localMain(argc, argv);
  return 0;
}
