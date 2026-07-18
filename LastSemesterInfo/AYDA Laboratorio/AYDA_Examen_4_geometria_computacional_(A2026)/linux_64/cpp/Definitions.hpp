#pragma once

#ifdef DEBUG
#define LOG(...) printf(__VA_ARGS__); printf("\n");
#else
#define LOG(...)
#endif

#define UNUSED(name) (void)name;

# include <tuple>

using namespace std;

# include <vector2D.hpp>
# include <polygon.hpp>

using namespace geom_lib;

// Alias for an obstacle.
using Obstacle = Polygon;

// Trigonometry of notable angles.
constexpr double sin0{0.};
constexpr double sin30{0.5};
constexpr double sin45{0.707106781};
constexpr double sin60{0.866025404};
constexpr double sin90{1.};
constexpr double cos0{1.};
constexpr double cos30{0.866025404};
constexpr double cos45{0.707106781};
constexpr double cos60{0.5};
constexpr double cos90{0.};

// Convert degrees to radians.
inline double degree_to_radian(double a)
{
  const double PI = 3.14159265358979323846;
  return a * PI / 180.;
}

// Compute sin of an angle in degrees.
inline double dsin(double a)
{
  if (real_equal(a, 0.))
    return sin0;

  if (real_equal(a, 30.))
    return sin30;

  if (real_equal(a, 45.))
    return sin45;

  if (real_equal(a, 60.))
    return sin60;

  if (real_equal(a, 90.))
    return sin90;

  return std::sin(degree_to_radian(a));
}

// Compute cos of an angle in degrees.
inline double dcos(double a)
{
  if (real_equal(a, 0.))
    return cos0;

  if (real_equal(a, 30.))
    return cos30;

  if (real_equal(a, 45.))
    return cos45;

  if (real_equal(a, 60.))
    return cos60;

  if (real_equal(a, 90.))
    return cos90;

  return std::cos(degree_to_radian(a));
}

// 2x2 transformation matrix.
struct Mat2D
{
  double m11;
  double m12;
  double m21;
  double m22;

  Mat2D(double _m11, double _m12, double _m21, double _m22)
    : m11{_m11}, m12{_m12}, m21{_m21}, m22{_m22} {}

  // Product with a column vector.
  Vector2D operator * (const Vector2D &v) const
  {
    return Vector2D(m11 * v.get_x() + m12 * v.get_y(),
		    m21 * v.get_x() + m22 * v.get_y());
  }
};

/** Alias for the vision area of a camera.
 *
 * It is defined by three counterclockwise points.
 *
 * Each point is defined as follow:
 *  - The first element represents the camera position.
 *  - The second element is the farthest point to the right of the area.
 *  - The third element is the farthest point to the left of the area.
 *  - The fourth element is the vision length (vision radius).
 */
using VisionArea = tuple<Vector2D, Vector2D, Vector2D, double>;