/*
  Authors: Sebastian Ramirez and Gerardo Rosetti
  Inspired by DeSiGNAR by R3mmurd
*/

#pragma once

#include <point2D.hpp>
#include <cmath>

namespace geom_lib
{
  class Vector2D : public Point2D
  {
    using Base = Point2D;
    using Base::Base;

  public:
    static const Vector2D ZERO;

    bool is_to_right_from(const Vector2D &) const;

    bool is_to_right_on_from(const Vector2D &) const;

    bool is_to_left_from(const Vector2D &) const;

    bool is_to_left_on_from(const Vector2D &) const;

    bool is_collinear_with(const Vector2D &) const;

    bool is_normalized() const;

    bool is_unitarian() const;

    double square_magnitude() const;

    double magnitude() const;

    double length() const;

    void normalize();

    void negate();

    void scale(double);

    bool is_opposite(const Vector2D &) const;

    Vector2D get_opposite() const;

    double angle_with(const Vector2D &) const;

    void add_scaled_vector(const Vector2D &, double);

    double dot_product(const Vector2D &) const;

    double scalar_product(const Vector2D &) const;

    double cross_product(const Vector2D &) const;

    double vector_product(const Vector2D &) const;

    Vector2D component_product(const Vector2D &) const;

    /// Performs get_opossite.
    Vector2D operator-() const;

    /// Multiplies this by a scalar value
    Vector2D operator*(double) const;

    /// Accumulates the product of this by a scalar value.
    void operator*=(double);

    /// Multiplies a scalar value by a vector.
    friend Vector2D operator*(double, const Vector2D &);

    /// Performs dot_product.
    double operator*(const Vector2D &) const;

    /// Performs vector addition.
    Vector2D operator+(const Vector2D &) const;

    /// Accumulative vector addition.
    void operator+=(const Vector2D &);

    /// Performs vector substraction.
    Vector2D operator-(const Vector2D &) const;

    /// Accumulative vector substraction.
    void operator-=(const Vector2D &);

    static std::tuple<Vector2D, Vector2D>
    make_orthonormal_basis(const Vector2D &);
  };

} // end namespace geom_lib