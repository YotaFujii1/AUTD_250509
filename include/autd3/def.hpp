#pragma once

#if _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4127)
#pragma warning(disable : 5054)
#pragma warning(disable : 26495)
#endif
#include <Eigen/Geometry>
#if _MSC_VER
#pragma warning(pop)
#endif

#include <numbers>

namespace autd3::driver {

using Vector3 = Eigen::Matrix<float, 3, 1>;
using Vector4 = Eigen::Matrix<float, 4, 1>;
using Matrix4X4 = Eigen::Matrix<float, 4, 4>;
using Matrix3X3 = Eigen::Matrix<float, 3, 3>;
using Quaternion = Eigen::Quaternion<float>;
using Affine3 = Eigen::Transform<float, 3, Eigen::Affine>;

constexpr float pi = std::numbers::pi_v<float>;

}  // namespace autd3::driver
