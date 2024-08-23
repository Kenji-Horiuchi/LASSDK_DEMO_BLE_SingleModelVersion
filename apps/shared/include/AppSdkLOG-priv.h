//
// Copyright 2023,2024 Sony Semiconductor Solutions Corporation.
//
#ifndef APP_SHARED_INCLUDE_APPSDKLOG_PRIV_H_
#define APP_SHARED_INCLUDE_APPSDKLOG_PRIV_H_

#include <cstdint>
#include <map>

#define JPDEF(t,v) using v##logtype_t = std::map<t,const char*>; const static v##logtype_t v

#endif  // APP_SHARED_INCLUDE_APPSDKLOG_PRIV_H_

#if defined(kCNAME) && !defined(LOGINFO)

#include "Log.h"

constexpr const pst::log::LogModule kASDK("ALIB");

namespace apps {
namespace shared {
namespace appsdk {

#define LOGINFO(...) LOGINFO2(__func__,__VA_ARGS__,"")
#define LOGINFO2(func,fmt,...) PST_RINFO(&kASDK,kCNAME "::%s " fmt "%s",func,__VA_ARGS__)

#define LOGDEBUG(...) LOGDEBUG2(__func__,__VA_ARGS__,"")
#define LOGDEBUG2(func,fmt,...) PST_DEBUG(&kASDK,"[D]" kCNAME "::%s " fmt "%s",func,__VA_ARGS__)

#define LOGWARN(...) LOGWARN2(__func__,__VA_ARGS__,"")
#define LOGWARN2(func,fmt,...) PST_RWARN(&kASDK,"[WARN]" kCNAME "::%s " fmt "%s",func,__VA_ARGS__)

#define LOGERR(...) LOGERR2(__func__,__VA_ARGS__,"")
#define LOGERR2(func,fmt,...) PST_RERR(&kASDK,"[ERR]" kCNAME "::%s " fmt "%s",func,__VA_ARGS__)

#define CHK(f) check_result(__func__,f)

#define JP(v,i) v.count(i) ? v.at(i) : "unknown",static_cast<int>(i)
#define JPVAR(v) const v##logtype_t v

}  // namespace appsdk
}  // namespace shared
}  // namespace apps

#endif  // kCNAME
