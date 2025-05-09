#pragma once

#include <chrono>
#include <optional>

#include "autd3/exception.hpp"
#include "autd3/native_methods.hpp"

#ifdef AUTD3_ASYNC_API
#include <coro/coro.hpp>
#endif

#define AUTD3_DEF_PROP(PARAM_T, PARAM_NAME)                                               \
  [[nodiscard]] PARAM_T PARAM_NAME() const { return _##PARAM_NAME; } /* LCOV_EXCL_LINE */ \
                                                                                          \
 protected:                                                                               \
  PARAM_T _##PARAM_NAME;                                                                  \
                                                                                          \
 public:

#define AUTD3_DEF_PARAM(T, PARAM_T, PARAM_NAME)                                                     \
  void with_##PARAM_NAME(PARAM_T value)& { _##PARAM_NAME = std::move(value); } /* LCOV_EXCL_LINE */ \
  [[nodiscard]] T&& with_##PARAM_NAME(PARAM_T value)&& {                       /* LCOV_EXCL_LINE */ \
    _##PARAM_NAME = std::move(value);                                          /* LCOV_EXCL_LINE */ \
    return std::move(*static_cast<T*>(this));                                  /* LCOV_EXCL_LINE */ \
  }                                                                                                 \
  [[nodiscard]] PARAM_T PARAM_NAME() const { return _##PARAM_NAME; } /* LCOV_EXCL_LINE */           \
                                                                                                    \
 protected:                                                                                         \
  PARAM_T _##PARAM_NAME;                                                                            \
                                                                                                    \
 public:

#define AUTD3_DEF_PARAM_INT(T, PARAM_T, PARAM_NAME)                                       \
  template <std::integral I>                                                              \
  void with_##PARAM_NAME(I value)& {                                                      \
    _##PARAM_NAME = static_cast<PARAM_T>(value); /* LCOV_EXCL_LINE */                     \
  }                                                                                       \
  template <std::integral I>                                                              \
  [[nodiscard]] T&& with_##PARAM_NAME(I value)&& {                                        \
    _##PARAM_NAME = static_cast<PARAM_T>(value); /* LCOV_EXCL_LINE */                     \
    return std::move(*static_cast<T*>(this));    /* LCOV_EXCL_LINE */                     \
  }                                                                                       \
  [[nodiscard]] PARAM_T PARAM_NAME() const { return _##PARAM_NAME; } /* LCOV_EXCL_LINE */ \
 protected:                                                                               \
  PARAM_T _##PARAM_NAME;                                                                  \
                                                                                          \
 public:

#define AUTD3_DEF_PARAM_INTENSITY(T, PARAM_NAME)                                                               \
  template <std::integral I>                                                                                   \
  void with_##PARAM_NAME(I value)& {                                                                           \
    _##PARAM_NAME = autd3::driver::EmitIntensity(value); /* LCOV_EXCL_LINE */                                  \
  }                                                                                                            \
  template <std::integral I>                                                                                   \
  [[nodiscard]] T&& with_##PARAM_NAME(I value)&& {                                                             \
    _##PARAM_NAME = autd3::driver::EmitIntensity(value); /* LCOV_EXCL_LINE */                                  \
    return std::move(*static_cast<T*>(this));            /* LCOV_EXCL_LINE */                                  \
  }                                                                                                            \
  void with_##PARAM_NAME(autd3::driver::EmitIntensity value)& { _##PARAM_NAME = value; } /* LCOV_EXCL_LINE */  \
  [[nodiscard]] T&& with_##PARAM_NAME(autd3::driver::EmitIntensity value)&& {                                  \
    _##PARAM_NAME = value;                    /* LCOV_EXCL_LINE */                                             \
    return std::move(*static_cast<T*>(this)); /* LCOV_EXCL_LINE */                                             \
  }                                                                                                            \
  [[nodiscard]] autd3::driver::EmitIntensity PARAM_NAME() const { return _##PARAM_NAME; } /* LCOV_EXCL_LINE */ \
 protected:                                                                                                    \
  autd3::driver::EmitIntensity _##PARAM_NAME;                                                                  \
                                                                                                               \
 public:

#define AUTD3_DEF_PARAM_PHASE(T, PARAM_NAME)                                                           \
  template <std::integral I>                                                                           \
  void with_##PARAM_NAME(I value)& {                                                                   \
    _##PARAM_NAME = autd3::driver::Phase(value); /* LCOV_EXCL_LINE */                                  \
  }                                                                                                    \
  template <std::integral I>                                                                           \
  [[nodiscard]] T&& with_##PARAM_NAME(I value)&& {                                                     \
    _##PARAM_NAME = autd3::driver::Phase(value); /* LCOV_EXCL_LINE */                                  \
    return std::move(*static_cast<T*>(this));    /* LCOV_EXCL_LINE */                                  \
  }                                                                                                    \
  void with_##PARAM_NAME(autd3::driver::Phase value)& { _##PARAM_NAME = value; } /* LCOV_EXCL_LINE */  \
  [[nodiscard]] T&& with_##PARAM_NAME(autd3::driver::Phase value)&& {                                  \
    _##PARAM_NAME = value;                    /* LCOV_EXCL_LINE */                                     \
    return std::move(*static_cast<T*>(this)); /* LCOV_EXCL_LINE */                                     \
  }                                                                                                    \
  [[nodiscard]] autd3::driver::Phase PARAM_NAME() const { return _##PARAM_NAME; } /* LCOV_EXCL_LINE */ \
 protected:                                                                                            \
  autd3::driver::Phase _##PARAM_NAME;                                                                  \
                                                                                                       \
 public:

#define AUTD3_DEF_PARAM_CHRONO(T, PARAM_NAME)                                                              \
  template <typename Rep, typename Period>                                                                 \
  void with_##PARAM_NAME(const std::chrono::duration<Rep, Period> value)& {                                \
    _##PARAM_NAME = std::chrono::duration_cast<std::chrono::nanoseconds>(value); /* LCOV_EXCL_LINE */      \
  }                                                                                                        \
  template <typename Rep, typename Period>                                                                 \
  [[nodiscard]] T&& with_##PARAM_NAME(const std::chrono::duration<Rep, Period> value)&& {                  \
    _##PARAM_NAME = std::chrono::duration_cast<std::chrono::nanoseconds>(value); /* LCOV_EXCL_LINE */      \
    return std::move(*static_cast<T*>(this));                                    /* LCOV_EXCL_LINE */      \
  }                                                                                                        \
  [[nodiscard]] std::chrono::nanoseconds PARAM_NAME() const { return _##PARAM_NAME; } /* LCOV_EXCL_LINE */ \
 protected:                                                                                                \
  std::chrono::nanoseconds _##PARAM_NAME;                                                                  \
                                                                                                           \
 public:

namespace autd3::native_methods {

template <typename T>
constexpr auto validate(T res) {
  if (res.err_len != 0) {
    const std::string err(res.err_len, ' ');
    AUTDGetErr(res.err, const_cast<char*>(err.c_str()));
    throw AUTDException(err);
  }
  return res.result;
}

template <typename T = AUTDStatus>
constexpr T validate(ResultStatus res) {
  const auto [result, err_len, err] = res;
  if (result == AUTDStatus::AUTDErr) {
    const std::string err_str(err_len, ' ');
    AUTDGetErr(err, const_cast<char*>(err_str.c_str()));
    throw AUTDException(err_str);
  }
  return static_cast<T>(result);
}

template <typename Rep, typename Period>
inline Duration to_duration(const std::chrono::duration<Rep, Period>& d) {
  return Duration{static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(d).count())};
}

inline std::chrono::nanoseconds from_duration(const Duration& d) { return std::chrono::nanoseconds(d.nanos); }

template <typename Rep, typename Period>
inline OptionDuration to_option_duration(const std::optional<std::chrono::duration<Rep, Period>>& d) {
  return OptionDuration{d.has_value(), to_duration(d.value_or(std::chrono::nanoseconds(0)))};
}

inline std::optional<std::chrono::nanoseconds> from_option_duration(const OptionDuration& d) {
  return d.has_value ? std::make_optional(from_duration(d.value)) : std::nullopt;
}

#ifdef AUTD3_ASYNC_API
inline coro::task<ResultStatus> wait_future(const HandlePtr handle, FfiFutureResultStatus future) {
  co_return AUTDWaitResultStatus(handle, std::move(future));
}

inline coro::task<ResultStatus> wait_future(const HandlePtr handle, LocalFfiFutureResultStatus future) {
  co_return AUTDWaitLocalResultStatus(handle, std::move(future));
}

inline coro::task<ResultFPGAStateList> wait_future(const HandlePtr handle, FfiFutureResultFPGAStateList future) {
  co_return AUTDWaitResultFPGAStateList(handle, std::move(future));
}

inline coro::task<ResultFirmwareVersionList> wait_future(const HandlePtr handle, FfiFutureResultFirmwareVersionList future) {
  co_return AUTDWaitResultFirmwareVersionList(handle, std::move(future));
}

inline coro::task<ResultController> wait_future(const HandlePtr handle, FfiFutureResultController future) {
  co_return AUTDWaitResultController(handle, std::move(future));
}

#endif

}  // namespace autd3::native_methods
