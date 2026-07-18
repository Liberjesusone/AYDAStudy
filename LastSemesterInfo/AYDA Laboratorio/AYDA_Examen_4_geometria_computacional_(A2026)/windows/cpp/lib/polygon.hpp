/*
  Authors: Sebastian Ramirez and Gerardo Rosetti
  Inspired by DeSiGNAR by R3mmurd
*/

#pragma once

#include <list>
#include <initializer_list>
#include <stdexcept>
#include <algorithm>
#include <segment.hpp>
# include <cstdint>

namespace geom_lib
{
  template <typename PointT>
  class GenPolygon
  {
    std::list<PointT> points;

    using ListIt = typename std::list<PointT>::iterator;
    using ConstListIt = typename std::list<PointT>::const_iterator;

  public:
    using PointType = PointT;
    using SegmentType = GenSegment<PointT>;

    GenPolygon() = default;

    GenPolygon(const std::initializer_list<PointT> &l) : points(l) {}

    GenPolygon(const GenPolygon &p) : points(p.points) {}

    GenPolygon(GenPolygon &&p) noexcept : points(std::move(p.points)) {}

    ~GenPolygon() = default;

    GenPolygon &operator=(const GenPolygon &p)
    {
      if (this != &p) points = p.points;
      return *this;
    }

    GenPolygon &operator=(GenPolygon &&p) noexcept
    {
      points = std::move(p.points);
      return *this;
    }

    void swap(GenPolygon &p) { points.swap(p.points); }

    void clear() { points.clear(); }

    void add_vertex(const PointT &p) { points.push_back(p); }

    void add_vertex(PointT &&p) { points.push_back(std::move(p)); }

    PointT &get_first_vertex()
    {
      if (points.empty()) throw std::length_error("Polygon has no vertices");
      return points.front();
    }

    const PointT &get_first_vertex() const
    {
      if (points.empty()) throw std::length_error("Polygon has no vertices");
      return points.front();
    }

    PointT &get_last_vertex()
    {
      if (points.empty()) throw std::length_error("Polygon has no vertices");
      return points.back();
    }

    const PointT &get_last_vertex() const
    {
      if (points.empty()) throw std::length_error("Polygon has no vertices");
      return points.back();
    }

    SegmentType get_first_segment() const
    {
      if (points.size() < 2) throw std::length_error("Polygon has no segments");
      auto it = points.begin();
      const PointT &s = *it;
      const PointT &t = *(++it);
      return SegmentType(s, t);
    }

    SegmentType get_last_segment() const
    {
      if (points.size() < 2) throw std::length_error("Polygon has no segments");
      // The final segment closes the polygon
      return SegmentType(points.back(), points.front());
    }

    bool is_empty() const { return points.empty(); }

    uint64_t size() const { return static_cast<uint64_t>(points.size()); }

    // --- Internal Algorithms for Iterators ---
    template <typename Iterator, typename Op>
    static void for_each_it(Iterator begin, Iterator end, Op &&op)
    {
      for (; begin != end; begin.next())
        op(begin.get_current());
    }

    template <typename Iterator, typename Pred>
    static bool all_it(Iterator begin, Iterator end, Pred &&pred)
    {
      for (; begin != end; begin.next())
        if (!pred(begin.get_current())) return false;
      return true;
    }

    template <typename Iterator, typename Pred>
    static bool exists_it(Iterator begin, Iterator end, Pred &&pred)
    {
      for (; begin != end; begin.next())
        if (pred(begin.get_current())) return true;
      return false;
    }

    template <typename Iterator, typename Pred>
    static bool none_it(Iterator begin, Iterator end, Pred &&pred)
    {
      for (; begin != end; begin.next())
        if (pred(begin.get_current())) return false;
      return true;
    }

    // --- Vertex Iterator ---
    class VertexIterator
    {
      GenPolygon *p_ptr;
      ListIt curr_it;

    public:
      using iterator_category = std::bidirectional_iterator_tag;
      using value_type = PointT;
      using difference_type = std::ptrdiff_t;
      using pointer = PointT*;
      using reference = PointT&;

      VertexIterator() : p_ptr(nullptr) {}
      VertexIterator(const GenPolygon &p, ListIt it)
          : p_ptr(const_cast<GenPolygon*>(&p)), curr_it(it) {}

      PointT &get_current() { return *curr_it; }
      const PointT &get_current() const { return *curr_it; }

      // Legacy
      void next() { ++curr_it; }
      void prev() { --curr_it; }

      PointT& operator*() { return get_current(); }
      const PointT& operator*() const { return get_current(); }
      PointT* operator->() { return &(*curr_it); }

      VertexIterator& operator++() { next(); return *this; }
      VertexIterator operator++(int) { VertexIterator tmp = *this; next(); return tmp; }
      VertexIterator& operator--() { prev(); return *this; }
      VertexIterator operator--(int) { VertexIterator tmp = *this; prev(); return tmp; }

      bool operator==(const VertexIterator& other) const { return curr_it == other.curr_it; }
      bool operator!=(const VertexIterator& other) const { return curr_it != other.curr_it; }

      void del()
      {
        if (curr_it == p_ptr->points.end()) throw std::overflow_error("No current element");
        curr_it = p_ptr->points.erase(curr_it);
      }
    };

    // --- Segment Iterator ---
    class SegmentIterator
    {
      GenPolygon *p_ptr;
      ListIt curr_it;

    public:
      using iterator_category = std::bidirectional_iterator_tag;
      using value_type = SegmentType;
      using difference_type = std::ptrdiff_t;
      using pointer = SegmentType*;
      using reference = SegmentType;

      SegmentIterator() : p_ptr(nullptr) {}
      SegmentIterator(const GenPolygon &p, ListIt it)
          : p_ptr(const_cast<GenPolygon*>(&p)), curr_it(it) {}

      bool has_current() const { return curr_it != p_ptr->points.end(); }

      SegmentType get_current() const
      {
        if (!has_current()) throw std::overflow_error("No current element");
        auto next_it = std::next(curr_it);

        // Circular behavior of the polygon
        if (next_it == p_ptr->points.end()) next_it = p_ptr->points.begin();
        return SegmentType(*curr_it, *next_it);
      }

      // Legacy
      void next() { if (has_current()) ++curr_it; }
      void prev() { if (curr_it != p_ptr->points.begin()) --curr_it; }

      SegmentType operator*() const { return get_current(); }

      SegmentIterator& operator++() { next(); return *this; }
      SegmentIterator operator++(int) { SegmentIterator tmp = *this; next(); return tmp; }
      SegmentIterator& operator--() { prev(); return *this; }
      SegmentIterator operator--(int) { SegmentIterator tmp = *this; prev(); return tmp; }

      bool operator==(const SegmentIterator& other) const { return curr_it == other.curr_it; }
      bool operator!=(const SegmentIterator& other) const { return curr_it != other.curr_it; }

      void del()
      {
          if (!has_current()) throw std::overflow_error("No current element");

          auto next_it = std::next(curr_it);
          if (next_it == p_ptr->points.end()) next_it = p_ptr->points.begin();

          if (curr_it == next_it) {
              p_ptr->points.erase(curr_it);
          } else {
              p_ptr->points.erase(curr_it);
              p_ptr->points.erase(next_it);
          }

          curr_it = p_ptr->points.begin();
      }
    };

    // --- Iterator Factories ---

    VertexIterator vertices_begin() { return VertexIterator(*this, points.begin()); }
    VertexIterator vertices_begin() const { return VertexIterator(*this, const_cast<std::list<PointT>&>(points).begin()); }
    VertexIterator vertices_end() { return VertexIterator(*this, points.end()); }
    VertexIterator vertices_end() const { return VertexIterator(*this, const_cast<std::list<PointT>&>(points).end()); }

    SegmentIterator segments_begin() { return SegmentIterator(*this, points.begin()); }
    SegmentIterator segments_begin() const { return SegmentIterator(*this, const_cast<std::list<PointT>&>(points).begin()); }
    SegmentIterator segments_end() { return SegmentIterator(*this, points.end()); }
    SegmentIterator segments_end() const { return SegmentIterator(*this, const_cast<std::list<PointT>&>(points).end()); }

    // --- Algoritmos (Wrappers) ---

    template <class Op> void for_each_vertex(Op &&op = Op()) const {
        for_each_it(vertices_begin(), vertices_end(), std::forward<Op>(op));
    }
    template <class Pred> bool all_vertex(Pred &&pred = Pred()) const {
        return all_it(vertices_begin(), vertices_end(), std::forward<Pred>(pred));
    }
    template <class Pred> bool exists_vertex(Pred &&pred = Pred()) const {
        return exists_it(vertices_begin(), vertices_end(), std::forward<Pred>(pred));
    }
    template <class Pred> bool none_vertex(Pred &&pred = Pred()) const {
        return none_it(vertices_begin(), vertices_end(), std::forward<Pred>(pred));
    }

    template <class Op> void for_each_segment(Op &&op = Op()) const {
        for_each_it(segments_begin(), segments_end(), std::forward<Op>(op));
    }
    template <class Pred> bool all_segment(Pred &&pred = Pred()) const {
        return all_it(segments_begin(), segments_end(), std::forward<Pred>(pred));
    }
    template <class Pred> bool exists_segment(Pred &&pred = Pred()) const {
        return exists_it(segments_begin(), segments_end(), std::forward<Pred>(pred));
    }
    template <class Pred> bool none_segment(Pred &&pred = Pred()) const {
        return none_it(segments_begin(), segments_end(), std::forward<Pred>(pred));
    }
  };

  class PolygonInt : public GenPolygon<PointInt2D>
  {
    using Base = GenPolygon<PointInt2D>;
    using Base::Base;
  };

  class Polygon : public GenPolygon<Point2D>
  {
    using Base = GenPolygon<Point2D>;
    using Base::Base;
  };

} // end namespace geom_lib