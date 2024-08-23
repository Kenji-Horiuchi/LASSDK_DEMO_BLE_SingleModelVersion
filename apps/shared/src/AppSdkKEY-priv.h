//
// Copyright 2023 Sony Semiconductor Solutions Corporation.
//
#ifndef APP_SHARED_INCLUDE_APPSDKKEY_PRIV_H_
#define APP_SHARED_INCLUDE_APPSDKKEY_PRIV_H_

#include <cstdint>
#include <map>

#include "AppSdkType.h"
#include "KeyCode.h"
#include "AppSdkLOG-priv.h"

namespace apps {
namespace shared {
namespace appsdk {

class
AppSdkKEY
{
 public:
  AppSdkKEY(EventNotify);
  ~AppSdkKEY() = default;

  bool CheckEvent(pst::key::keycode_t);

 private:
  enum
  lr_t
  {
    kLeft,
    kRight
  };

  bool get_position(void);

  std::unique_ptr<lr_t> ch_ = nullptr;
  EventNotify event_proc_;

  using kc_t = pst::key::keycode_t;
  using kmap_t = std::map<pst::key::keycode_t, event_t>;
  const static kmap_t key_to_event_;

  using kmap_lr_t = std::map<lr_t, kmap_t>;
  const static kmap_lr_t key_to_eventLR_;

  JPDEF(kc_t, j_keycode_);
};

}  // namespace appsdk
}  // namespace shared
}  // namespace apps

#endif  // APP_SHARED_INCLUDE_APPSDKKEY_PRIV_H_
