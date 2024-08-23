//
// Copyright 2022 Sony Semiconductor Solutions Corporation.
//
#ifndef APP_SHARED_INCLUDE_APPSHAREDLOG_PRIV_H_
#define APP_SHARED_INCLUDE_APPSHAREDLOG_PRIV_H_

#include "Log.h"

constexpr const pst::log::LogModule kAppSharedLog("TWSS");

#define TWSS_EMERG(...)  PST_EMERG(&kAppSharedLog, ##__VA_ARGS__)
#define TWSS_ALERT(...)  PST_ALERT(&kAppSharedLog, ##__VA_ARGS__)
#define TWSS_CRIT(...)   PST_CRIT(&kAppSharedLog, ##__VA_ARGS__)
#define TWSS_ERR(...)    PST_ERR(&kAppSharedLog, ##__VA_ARGS__)
#define TWSS_WARN(...)   PST_WARN(&kAppSharedLog, ##__VA_ARGS__)
#define TWSS_NOTICE(...) PST_NOTICE(&kAppSharedLog, ##__VA_ARGS__)
#define TWSS_INFO(...)   PST_INFO(&kAppSharedLog, ##__VA_ARGS__)
#define TWSS_DEBUG(...)  PST_DEBUG(&kAppSharedLog, ##__VA_ARGS__)
#define TWSS_TRACE(...)  PST_TRACE(&kAppSharedLog, ##__VA_ARGS__)
#define TWSS_TRACE0()    PST_TRACE(&kAppSharedLog)
#define TWSS_FTRACE0()   PST_FTRACE(&kAppSharedLog)

#define TWSS_REMERG(...)  PST_REMERG(&kAppSharedLog, ##__VA_ARGS__)
#define TWSS_RALERT(...)  PST_RALERT(&kAppSharedLog, ##__VA_ARGS__)
#define TWSS_RCRIT(...)   PST_RCRIT(&kAppSharedLog, ##__VA_ARGS__)
#define TWSS_RERR(...)    PST_RERR(&kAppSharedLog, ##__VA_ARGS__)
#define TWSS_RWARN(...)   PST_RWARN(&kAppSharedLog, ##__VA_ARGS__)
#define TWSS_RNOTICE(...) PST_RNOTICE(&kAppSharedLog, ##__VA_ARGS__)
#define TWSS_RINFO(...)   PST_RINFO(&kAppSharedLog, ##__VA_ARGS__)
#define TWSS_RDEBUG(...)  PST_RDEBUG(&kAppSharedLog, ##__VA_ARGS__)

#endif  // APP_SHARED_INCLUDE_APPSHAREDLOG_PRIV_H_
