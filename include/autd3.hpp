#pragma once

#include <string>

#include "autd3/controller/builder.hpp"
#include "autd3/controller/controller.hpp"
#include "autd3/def.hpp"
#include "autd3/driver/autd3_device.hpp"
#include "autd3/driver/datagram/clear.hpp"
#include "autd3/driver/datagram/datagram.hpp"
#include "autd3/driver/datagram/debug.hpp"
#include "autd3/driver/datagram/force_fan.hpp"
#include "autd3/driver/datagram/phase_corr.hpp"
#include "autd3/driver/datagram/pulse_width_encoder.hpp"
#include "autd3/driver/datagram/reads_fpga_state.hpp"
#include "autd3/driver/datagram/segment.hpp"
#include "autd3/driver/datagram/silencer.hpp"
#include "autd3/driver/datagram/stm/foci.hpp"
#include "autd3/driver/datagram/stm/gain.hpp"
#include "autd3/driver/datagram/synchronize.hpp"
#include "autd3/driver/defined/angle.hpp"
#include "autd3/driver/defined/freq.hpp"
#include "autd3/driver/firmware/fpga/drive.hpp"
#include "autd3/driver/firmware/fpga/emit_intensity.hpp"
#include "autd3/driver/firmware/fpga/phase.hpp"
#include "autd3/driver/firmware/fpga/sampling_config.hpp"
#include "autd3/driver/firmware/fpga/transition_mode.hpp"
#include "autd3/driver/geometry/device.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/driver/geometry/rotation.hpp"
#include "autd3/driver/geometry/transducer.hpp"
#include "autd3/gain/bessel.hpp"
#include "autd3/gain/focus.hpp"
#include "autd3/gain/group.hpp"
#include "autd3/gain/null.hpp"
#include "autd3/gain/plane.hpp"
#include "autd3/gain/uniform.hpp"
#include "autd3/modulation/sine.hpp"
#include "autd3/modulation/square.hpp"
#include "autd3/modulation/static.hpp"

namespace autd3 {

constexpr float pi = driver::pi;

static inline std::string version = "29.0.0-rc.11";

using autd3::driver::deg;
using autd3::driver::Hz;
using autd3::driver::kHz;
using autd3::driver::rad;

using driver::AUTD3;
using driver::geometry::Device;
using driver::geometry::EulerAngles;
using driver::geometry::Geometry;
using driver::geometry::Transducer;

using driver::DcSysTime;
using driver::DebugType;
using driver::Drive;
using driver::EmitIntensity;
using driver::LoopBehavior;
using driver::Phase;
using driver::SamplingConfig;
using driver::TransitionMode;
using native_methods::GPIOIn;
using native_methods::GPIOOut;
using native_methods::Segment;
using native_methods::SilencerTarget;

using driver::Clear;
using driver::ControlPoint;
using driver::ControlPoints;
using driver::DebugSettings;
using driver::DebugType;
using driver::FixedCompletionTime;
using driver::FixedUpdateRate;
using driver::FociSTM;
using driver::ForceFan;
using driver::GainSTM;
using driver::PhaseCorrection;
using driver::PulseWidthEncoder;
using driver::ReadsFPGAState;
using driver::Silencer;
using driver::SwapSegment;
using driver::Synchronize;

using native_methods::GainSTMMode;

using gain::Bessel;
using gain::Focus;
using gain::Group;
using gain::Null;
using gain::Plane;
using gain::Uniform;

using modulation::Sine;
using modulation::Square;
using modulation::Static;

using driver::FirmwareVersion;
using driver::FPGAState;

using driver::Quaternion;
using driver::Vector3;

using controller::Controller;
using controller::ControllerBuilder;

#ifndef AUTD3_EXT_TRACING
inline void tracing_init() { native_methods::AUTDTracingInit(); }
#endif

}  // namespace autd3
