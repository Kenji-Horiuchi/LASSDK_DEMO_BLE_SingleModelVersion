//
// Copyright 2023 Sony Semiconductor Solutions Corporation.
//
#include <cstdio>
#include "TwsBaseApp-priv.h"
#include "AppSharedLog-priv.h"

#include "BroadcastEvent.h"
#include "GetManager.h"

#define APPNAME "TwsBaseApp"

namespace apps {
namespace tws {

TwsBaseApp::ttbl_t
const TwsBaseApp::ttbl_ =
{
  { 0, { shared::appsdk::effect_target_t::kMyself, "Myself" } },
  { 1, { shared::appsdk::effect_target_t::kSync, "Left and Right" } },
};

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
  auto app = pst::base::make_unique<TwsBaseApp>();
  app->SetSystemConfig(kSystemConfig);
  app->Main(APPNAME, argc, argv);
  return 0;
}

}  // namespace

TwsBaseApp::TwsBaseApp() = default;
TwsBaseApp::~TwsBaseApp() = default;

void TwsBaseApp::Boot() {
  TWSS_RINFO(APPNAME"::%s", __func__);

  SetEventToAction(
    shared::appsdk::event_t::kKeySingleR, shared::appsdk::action_t::kAudioUpSpeaker);
  SetEventToAction(
    shared::appsdk::event_t::kKeySingleL, shared::appsdk::action_t::kAudioDownSpeaker);

  SetEventToCallback(
    shared::appsdk::event_t::kFinGuidance,
    [this](shared::appsdk::event_t) {
      TWSS_RINFO(APPNAME" fin guidance");
      this->tap_base();
    }
    );

  SetEventToCallback(
    shared::appsdk::event_t::kStartGuidance,
    [this](shared::appsdk::event_t) {
      TWSS_RINFO(APPNAME" start guidance");
      UnSetEventToCallback(shared::appsdk::event_t::kKeyLong);
      this->tap_stop();
    }
    );

  SetEventToCallback(
    shared::appsdk::event_t::kChangeVolumeSpeaker,
    [this](shared::appsdk::event_t) {
      TWSS_RINFO(APPNAME" change volume");
    }
    );

  SetEventToCallback(
    shared::appsdk::event_t::kKeyDoubleR,
    [this](shared::appsdk::event_t) {
      this->ChangeSpeakerVolume(shared::appsdk::volume_ctrl_t::kToggle);
    }
    );

  SetEventToCallback(
    shared::appsdk::event_t::kKeySingleLongL,
    [this](shared::appsdk::event_t) {
      tag++;
      if(tag > 1) {
        tag = 0;
      }
      tap_info();
    }
    );

  SetEventToCallback(
    shared::appsdk::event_t::kKeyDoubleLongL,
    [this](shared::appsdk::event_t) {
      loop = loop ? false : true;
      tap_info();
    }
    );

  tap_base();
}

void TwsBaseApp::Run() {
  TWSS_RINFO(APPNAME"::%s", __func__);
  tap_net_init();

  tap_info();
}

void TwsBaseApp::Term() {
  TWSS_RINFO(APPNAME"::%s", __func__);
}

void TwsBaseApp::tap_info() {
  TWSS_RINFO("Left Tap                 ... reduce the volume");
  TWSS_RINFO("Right Tap                ... increase the volume");
  TWSS_RINFO("---------------------------------");
  TWSS_RINFO("Left Double Tap          ... plays the sound effect");
  TWSS_RINFO("Right Double Tap         ... toggle mute");
  TWSS_RINFO("---------------------------------");
  TWSS_RINFO("Left Single Long Tap     ... change play earbuds:%s", ttbl_.at(tag).m);
  TWSS_RINFO("Left Double Tap          ... change loop settings:%s", loop ? "on" : "off");
  TWSS_RINFO("---------------------------------");
  TWSS_RINFO("Left Double Tap while playing ... stop playing");
  TWSS_RINFO("---------------------------------");
  TWSS_RINFO("Triple Long Tap ... network pairing");
}

void TwsBaseApp::tap_base() {

  SetEventToCallback(
    shared::appsdk::event_t::kKeyLong,
    [this](shared::appsdk::event_t) {
      this->StopGuidance();
      easel::StartApp("SensorApp");
    }
    );

  SetEventToCallback(
    shared::appsdk::event_t::kKeyDoubleL,
    [this](shared::appsdk::event_t) {
      TWSS_RINFO(APPNAME" %s %s playing", ttbl_.at(tag).m, loop ? "loop" : "single");
      this->PlayGuidance(
        loop ? shared::appsdk::effect_id_t::kAmbientSound : shared::appsdk::effect_id_t::kAmbientSoundControlOff,
        ttbl_.at(tag).t,
        loop ? true : false);
    }
    );
}

void TwsBaseApp::tap_stop() {

  SetEventToCallback(
    shared::appsdk::event_t::kKeyDoubleL,
    [this](shared::appsdk::event_t) {
      TWSS_RINFO(APPNAME" stop playing");
      this->StopGuidance();
    }
    );
}

void TwsBaseApp::tap_net_init(void) {
  shared::appsdk::status_t ret;
  ret = this->ControlNetwork(shared::appsdk::net_ctrl_t::kEnablePairedDevice);
  if(ret!=shared::appsdk::status_t::kSuccess){
    TWSS_RINFO(APPNAME" bluetooth enable paired device failure(%d)",ret);
  };

  SetEventToCallback(
    shared::appsdk::event_t::kKeyTripleLong,
    [this](shared::appsdk::event_t) {
      if(this->ControlNetwork(shared::appsdk::net_ctrl_t::kPairing)==shared::appsdk::status_t::kSuccess){
        TWSS_RINFO(APPNAME" Bluetooth pairing");
        this->PlayGuidance(
          shared::appsdk::effect_id_t::kBluetoothPairing,
          shared::appsdk::effect_target_t::kMyself,
          false);
      }
    }
    );

  SetEventToCallback(
    shared::appsdk::event_t::kNetConnect,
    [this](shared::appsdk::event_t) {
      TWSS_RINFO(APPNAME" Bluetooth device connected");
      this->PlayGuidance(
        shared::appsdk::effect_id_t::kBluetoothDevice1Connected,
        shared::appsdk::effect_target_t::kMyself,
        false);
    }
    );

  SetEventToCallback(
    shared::appsdk::event_t::kNetDisconnect,
    [this](shared::appsdk::event_t) {
      TWSS_RINFO(APPNAME" Bluetooth device disconnected");
      this->PlayGuidance(
        shared::appsdk::effect_id_t::kBluetoothDevice1Disconnected,
        shared::appsdk::effect_target_t::kMyself,
        false);
    }
    );
}
}  // namespace tws
}  // namespace apps

extern "C" int APP_MAIN(int argc, char* argv[]);
extern "C" int APP_MAIN(int argc, char* argv[]) {
  apps::tws::localMain(argc, argv);
  return 0;
}
