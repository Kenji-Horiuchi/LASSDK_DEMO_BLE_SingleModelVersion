//
// Copyright 2023 Sony Semiconductor Solutions Corporation.
//

#ifndef BASE_APP_SRC_TWSBASEAPP_PRIV_H_
#define BASE_APP_SRC_TWSBASEAPP_PRIV_H_

#include "AppSdk.h"

namespace apps {
namespace tws {

class TwsBaseApp :
  public apps::shared::appsdk::AppSdk
{
 public:
  TwsBaseApp();
  virtual ~TwsBaseApp();

  void Boot(void) override;
  void Run(void) override;
  void Term(void) override;

 private:
  void tap_base(void);
  void tap_stop(void);
  void tap_info(void); 
  void tap_net_init(void);
  bool loop = false;
  int tag = 0;
  using tv_t = struct {shared::appsdk::effect_target_t t; const char* m;};
  using ttbl_t = std::map<int,tv_t>;
  const static ttbl_t ttbl_;
  };
}  // namespace tws
}  // namespace apps

#endif  // BASE_APP_SRC_TWSBASEAPP_PRIV_H_
