//
// Copyright 2022 Sony Semiconductor Solutions Corporation.
//
#ifndef FIRMWARE_SRC_MIDDLEWARE_FRAMEWORK_INCLUDE_CRADLE_DEFAULT_FD_APPHANDLECRADLEFDCOMMAND_H_
#define FIRMWARE_SRC_MIDDLEWARE_FRAMEWORK_INCLUDE_CRADLE_DEFAULT_FD_APPHANDLECRADLEFDCOMMAND_H_

#include <cstdint>

namespace pst {
namespace cradle {

/// @ingroup Cradle
///
/// Cradle FD Command definition
///
/// @defgroup CradleFdCommand Cradle Client FD Command Default constant definition
/// @{

/// FD App Echo
constexpr const char *kFdAppEcho = "AppEcho";
/// FD App Reboot
constexpr const char *kFdAppReboot = "AppReboot";

/// FD command definition that the app receives
PST_UNUSED static const char* kAppHandleFdCommands[] = {
  kFdAppEcho,
  kFdAppReboot,
};
/// @}

}  // namespace cradle
}  // namespace pst

#endif  // FIRMWARE_SRC_MIDDLEWARE_FRAMEWORK_INCLUDE_CRADLE_DEFAULT_FD_APPHANDLECRADLEFDCOMMAND_H_
