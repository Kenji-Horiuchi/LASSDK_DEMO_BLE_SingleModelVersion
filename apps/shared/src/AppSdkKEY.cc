//
// Copyright 2023 Sony Semiconductor Solutions Corporation.
//
#include "PstSystemProperty.h"
#define kCNAME "KEY"
#include "AppSdkKEY-priv.h"

namespace apps {
namespace shared {
namespace appsdk {

const AppSdkKEY::kmap_t
AppSdkKEY::key_to_event_ = {
  { pst::key::keycode_t::kSingle,     event_t::kKeySingle },
  { pst::key::keycode_t::kDouble,     event_t::kKeyDouble },
  { pst::key::keycode_t::kTriple,     event_t::kKeyTriple },
  { pst::key::keycode_t::kLong,       event_t::kKeyLong },
  { pst::key::keycode_t::kSingleLong, event_t::kKeySingleLong },
  { pst::key::keycode_t::kDoubleLong, event_t::kKeyDoubleLong },
  { pst::key::keycode_t::kTripleLong, event_t::kKeyTripleLong },
};

const AppSdkKEY::kmap_lr_t
AppSdkKEY::key_to_eventLR_ = {
  { kLeft,
    {
      { pst::key::keycode_t::kSingle,     event_t::kKeySingleL },
      { pst::key::keycode_t::kDouble,     event_t::kKeyDoubleL },
      { pst::key::keycode_t::kTriple,     event_t::kKeyTripleL },
      { pst::key::keycode_t::kLong,       event_t::kKeyLongL },
      { pst::key::keycode_t::kSingleLong, event_t::kKeySingleLongL },
      { pst::key::keycode_t::kDoubleLong, event_t::kKeyDoubleLongL },
      { pst::key::keycode_t::kTripleLong, event_t::kKeyTripleLongL },
    }
  },
  { kRight,
    {
      { pst::key::keycode_t::kSingle,     event_t::kKeySingleR },
      { pst::key::keycode_t::kDouble,     event_t::kKeyDoubleR },
      { pst::key::keycode_t::kTriple,     event_t::kKeyTripleR },
      { pst::key::keycode_t::kLong,       event_t::kKeyLongR },
      { pst::key::keycode_t::kSingleLong, event_t::kKeySingleLongR },
      { pst::key::keycode_t::kDoubleLong, event_t::kKeyDoubleLongR },
      { pst::key::keycode_t::kTripleLong, event_t::kKeyTripleLongR },
    }
  }
};

JPVAR(AppSdkKEY::j_keycode_) =
{
  { pst::key::keycode_t::kSingle, "kSingle" },
  { pst::key::keycode_t::kDouble, "kDouble" },
  { pst::key::keycode_t::kTriple, "kTriple" },
  { pst::key::keycode_t::kLong, "kLong" },
  { pst::key::keycode_t::kSingleLong, "kSingleLong" },
  { pst::key::keycode_t::kDoubleLong, "kDoubleLong" },
  { pst::key::keycode_t::kTripleLong, "kTripleLong" },
};

AppSdkKEY::AppSdkKEY(
  EventNotify proc
)
{
  LOGDEBUG();

  event_proc_ = proc;

  if(! get_position()) {
    LOGWARN("read position fail");
  }
}

bool
AppSdkKEY::CheckEvent(
  pst::key::keycode_t keycode
)
{
  LOGINFO("keycode:%s(%d)", JP(j_keycode_, keycode));

  auto result = false;

  if(key_to_event_.count(keycode)) {
    event_proc_(key_to_event_.at(keycode));

    if(ch_ && (key_to_eventLR_.at(*ch_)).count(keycode)) {
      event_proc_((key_to_eventLR_.at(*ch_)).at(keycode));
    } else {
      LOGWARN("no left and right information");
    }

    result = true;
  }

  return(result);
}

bool
AppSdkKEY::get_position(
  void
)
{
  constexpr char key[] = "pst.tws_position";
  constexpr char kl[] = "left";
  constexpr char kr[] = "right";
  auto result = false;

  if(ch_ == nullptr) {
    char buffer[sizeof(kr)];
    lr_t lr;

    if(PstGetSystemProperty(key, buffer, sizeof(buffer)) > 0) {
      if(0 == std::strcmp(kl, buffer)) {
        lr = kLeft;
        result = true;
      }
      else
      if(0 == std::strcmp(kr, buffer)) {
        lr = kRight;
        result = true;
      }
    }
    if(result) {
      ch_.reset(new lr_t);
      *ch_ = lr;
    }
  } else {
    result = true;
  }

  return(result);
}

}  // namespace appsdk
}  // namespace shared
}  // namespace apps
