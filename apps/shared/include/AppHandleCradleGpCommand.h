//
// Copyright 2022 Sony Semiconductor Solutions Corporation.
//
#ifndef FIRMWARE_SRC_MIDDLEWARE_FRAMEWORK_INCLUDE_CRADLE_DEFAULT_GP_APPHANDLECRADLEGPCOMMAND_H_
#define FIRMWARE_SRC_MIDDLEWARE_FRAMEWORK_INCLUDE_CRADLE_DEFAULT_GP_APPHANDLECRADLEGPCOMMAND_H_

#include <cstdint>

namespace pst {
namespace cradle {

/// @ingroup Cradle
///
/// Cradle GP Command definition
///
/// @defgroup CradleGpCommand Cradle Client GP Command Default constant definition
/// @{

/// GP Shutdown
constexpr const uint8_t kGpShutdown = 0x82;

/// GP Case control
constexpr const uint8_t kGpCaseControl = 0x89;
/// GP Case status
constexpr const uint8_t kGpCaseStatus = 0x8A;

/// GP command definition that the app receives
PST_UNUSED static const uint8_t kAppHandleGpCommands[] = {
  kGpShutdown,
  kGpCaseControl,
  kGpCaseStatus,
};

/// @}

}  // namespace cradle
}  // namespace pst

#endif  // FIRMWARE_SRC_MIDDLEWARE_FRAMEWORK_INCLUDE_CRADLE_DEFAULT_GP_APPHANDLECRADLEGPCOMMAND_H_
