//
// Copyright 2024 Sony Semiconductor Solutions Corporation.
//
#ifndef APP_SHARED_SRC_APPSDKDEFS_PRIV_H_
#define APP_SHARED_SRC_APPSDKDEFS_PRIV_H_

#include <cstdint>
#include "base_functional.h"
#include <memory>

namespace apps {
namespace shared {
namespace appsdk {

//
// 非同期実行
//
template <class T>
class
unsync_func_t
{
 public:
  using run_func_t = pst::base::function<void(T*)>;
  unsync_func_t(
    std::unique_ptr<T> data,
    run_func_t func
    ) : data_(std::move(data)), run_(func) {}
  unsync_func_t(unsync_func_t&& e) {
    this->data_ = std::move(e.data_);
    this->run_ = e.run_;
  }
  ~unsync_func_t() = default;

  inline void operator() () {
    run_(data_.get());
  }
 private:
  std::unique_ptr<T> data_;
  run_func_t run_;
};

}  // namespace appsdk
}  // namespace shared
}  // namespace apps

#endif  // APP_SHARED_SRC_APPSDKDEFS_PRIV_H_
