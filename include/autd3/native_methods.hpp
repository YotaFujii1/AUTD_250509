#pragma once

#ifdef _MSC_VER
#define AUTD3_API
#elif defined __GNUC__ && defined COVERAGE
#define AUTD3_API __attribute__((__used__))
#else
#define AUTD3_API
#endif

#include "autd3/native_methods/autd3-driver.h"
#include "autd3/native_methods/def.hpp"
// def.hpp must be included first
#include "autd3/native_methods/autd3capi-driver.h"
// then autd3capi-driver.h
#include "autd3/native_methods/autd3capi-gain-holo.h"
#include "autd3/native_methods/autd3capi-link-simulator.h"
#include "autd3/native_methods/autd3capi-link-twincat.h"
#include "autd3/native_methods/autd3capi-modulation-audio-file.h"
#include "autd3/native_methods/autd3capi.h"
