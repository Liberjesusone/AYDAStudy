/*
  Authors: Sebastian Ramirez and Gerardo Rosetti
  Inspired by DeSiGNAR by R3mmurd
*/

#pragma once

// #include <math.hpp>
#include <type_traits>
#include <utility>
#include <string>
#include <sstream>
#include <cmath>
#include <cstdint>

namespace geom_lib
{
  template <typename NumT>
  class GenPoint2D
  {
    static_assert(std::is_arithmetic<NumT>::value,
                  "Template argument must be an arithmetic type");

  protected:
    NumT x;
    NumT y;

  public:
    using ComponentType = NumT;
    using NumberType = NumT;
    using ValueType = NumT;

    GenPoint2D()
        : x(0), y(0)
    {
      // empty
    }

    GenPoint2D(const NumT &_x, const NumT &_y)
        : x(_x), y(_y)
    {
      // empty
    }

    GenPoint2D(const NumT &_x, NumT &&_y)
        : x(_x), y(std::forward<NumT>(_y))
    {
      // empty
    }

    GenPoint2D(NumT &&_x, const NumT &_y)
        : x(std::forward<NumT>(_x)), y(_y)
    {
      // empty
    }

    GenPoint2D(NumT &&_x, NumT &&_y)
        : x(std::forward<NumT>(_x)), y(std::forward<NumT>(_y))
    {
      // empty
    }

    GenPoint2D(const GenPoint2D &p)
        : x(p.x), y(p.y)
    {
      // empty
    }

    GenPoint2D(GenPoint2D &&p)
        : GenPoint2D()
    {
      swap(p);
    }

    GenPoint2D &operator=(const GenPoint2D &p)
    {
      if (this == &p)
        return *this;

      x = p.x;
      y = p.y;

      return *this;
    }

    GenPoint2D &operator=(GenPoint2D &&p)
    {
      swap(p);
      return *this;
    }

    void swap(GenPoint2D &p)
    {
      std::swap(x, p.x);
      std::swap(y, p.y);
    }

    const NumT &get_x() const
    {
      return x;
    }

    const NumT &get_y() const
    {
      return y;
    }

    void set_x(const NumT &_x)
    {
      x = _x;
    }

    void set_x(NumT &&_x)
    {
      x = std::move(_x);
    }

    void set_y(const NumT &_y)
    {
      y = _y;
    }

    void set_y(NumT &&_y)
    {
      y = std::move(_y);
    }

    void nullify()
    {
      x = y = NumT(0);
    }

    bool is_null() const
    {
      return x == NumT(0) && y == NumT(0);
    }

    bool is_zero() const
    {
      return is_null();
    }

    NumT square_distance_with(const GenPoint2D &p) const
    {
      NumT dx = p.x - x;
      NumT dy = p.y - y;
      return dx * dx + dy * dy;
    }

    double distance_with(const GenPoint2D &p) const
    {
      return std::sqrt(square_distance_with(p));
    }

    NumT square_distance_to_origin() const
    {
      return x * x + y * y;
    }

    NumT distance_to_origin() const
    {
      return std::sqrt(square_distance_to_origin());
    }

    double area_of_parallelogram(const GenPoint2D<NumT> &a,
                                 const GenPoint2D<NumT> &b,
                                 const GenPoint2D<NumT> &c) const
    {
      return (b.get_x() - a.get_x()) * (c.get_y() - a.get_y()) -
             (c.get_x() - a.get_x()) * (b.get_y() - a.get_y());
    }


    bool is_to_right_from(const GenPoint2D &p, const GenPoint2D &q) const
    {
      return area_of_parallelogram(p, q, *this) < NumT(0);
    }

    bool is_to_right_on_from(const GenPoint2D &p, const GenPoint2D &q) const
    {
      return area_of_parallelogram(p, q, *this) <= NumT(0);
    }

    bool is_to_left_from(const GenPoint2D &p, const GenPoint2D &q) const
    {
      return area_of_parallelogram(p, q, *this) > NumT(0);
    }

    bool is_to_left_on_from(const GenPoint2D &p, const GenPoint2D &q) const
    {
      return area_of_parallelogram(p, q, *this) >= NumT(0);
    }

    bool is_collinear_with(const GenPoint2D &p, const GenPoint2D &q) const
    {
      return area_of_parallelogram(p, q, *this) == NumT(0);
    }

    bool is_between(const GenPoint2D &p, const GenPoint2D &q) const
    {
      if (!is_collinear_with(p, q))
        return false;

      if (p.get_x() != q.get_x())
        return ((p.get_x() <= this->get_x()) && (this->get_x() <= q.get_x())) || ((p.get_x() >= this->get_x()) && (this->get_x() >= q.get_x()));
      else
        return ((p.get_y() <= this->get_y()) && (this->get_y() <= q.get_y())) || ((p.get_y() >= this->get_y()) && (this->get_y() >= q.get_y()));
    }

    explicit operator bool() const
    {
      return !is_null();
    }

    bool operator==(const GenPoint2D &p) const
    {
      return std::abs(static_cast<double>(x - p.x)) <= 1e-6 &&
             std::abs(static_cast<double>(y - p.y)) <= 1e-6;
    }

    bool operator!=(const GenPoint2D &p) const
    {
      return !(*this == p);
    }

    std::string to_string() const
    {
      std::stringstream s;
      s << '(' << x << ',' << y << ')';
      return s.str();
    }
  };

  class PointInt2D : public GenPoint2D<int64_t>
  {
    using Base = GenPoint2D<int64_t>;
    using Base::Base;
  };

  class Point2D : public GenPoint2D<double>
  {
    using Base = GenPoint2D<double>;
    using Base::Base;
  };

} // end namespace geom_lib