//
// Copyright 2023 Sony Semiconductor Solutions Corporation.
//

#ifndef BASE_APP_SRC_BOOTAPP_PRIV_H_
#define BASE_APP_SRC_BOOTAPP_PRIV_H_

#include "AppSdk.h"

namespace apps {
namespace boot {

class BootApp :
  public apps::shared::appsdk::AppSdk
{
 public:
  BootApp();
  virtual ~BootApp();

  void Boot(void) override;
  void Run(void) override;
  void Term(void) override;

 private:
};

}  // namespace boot
}  // namespace apps

#endif  // BASE_APP_SRC_BOOTAPP_PRIV_H_
