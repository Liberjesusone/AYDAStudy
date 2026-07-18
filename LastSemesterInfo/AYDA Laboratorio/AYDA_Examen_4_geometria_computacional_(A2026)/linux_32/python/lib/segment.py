import math
from .point2d import Point2D, real_equal

class Segment:
    def __init__(self, src_point: Point2D = None, tgt_point: Point2D = None):
        self.src_point = src_point if src_point else Point2D()
        self.tgt_point = tgt_point if tgt_point else Point2D()

    def get_src_point(self) -> Point2D: return self.src_point
    def get_tgt_point(self) -> Point2D: return self.tgt_point
    def set_src_point(self, sp: Point2D): self.src_point = sp
    def set_tgt_point(self, tp: Point2D): self.tgt_point = tp

    def length(self) -> float:
        return self.src_point.distance_with(self.tgt_point)

    def is_to_left_from(self, p: Point2D) -> bool:
        return p.is_to_right_from(self.src_point, self.tgt_point)

    def is_to_left_on_from(self, p: Point2D) -> bool:
        return p.is_to_right_on_from(self.src_point, self.tgt_point)

    def is_to_right_from(self, p: Point2D) -> bool:
        return p.is_to_left_from(self.src_point, self.tgt_point)

    def is_to_right_on_from(self, p: Point2D) -> bool:
        return p.is_to_left_on_from(self.src_point, self.tgt_point)

    def is_collinear_with(self, p: Point2D) -> bool:
        return p.is_collinear_with(self.src_point, self.tgt_point)

    def contains_point(self, p: Point2D) -> bool:
        return p.is_between(self.src_point, self.tgt_point)

    def contains_segment(self, s: 'Segment') -> bool:
        return self.contains_point(s.src_point) and self.contains_point(s.tgt_point)

    def intersects_properly_with(self, s: 'Segment') -> bool:
        if s.is_collinear_with(self.src_point) or s.is_collinear_with(self.tgt_point) or \
           self.is_collinear_with(s.src_point) or self.is_collinear_with(s.tgt_point):
            return False
        return (s.is_to_right_from(self.src_point) != s.is_to_right_from(self.tgt_point)) and \
               (self.is_to_right_from(s.src_point) != self.is_to_right_from(s.tgt_point))

    def intersects_with(self, s: 'Segment') -> bool:
        if self.intersects_properly_with(s): return True
        return self.contains_point(s.src_point) or self.contains_point(s.tgt_point) or \
               s.contains_point(self.src_point) or s.contains_point(self.tgt_point)

    def slope(self) -> float:
        if self.tgt_point.get_x() == self.src_point.get_x():
            if self.src_point.get_y() < self.tgt_point.get_y():
                return float('inf')
            else:
                return float('-inf')
        return (self.tgt_point.get_y() - self.src_point.get_y()) / (self.tgt_point.get_x() - self.src_point.get_x())

    def is_parallel_with(self, s: 'Segment') -> bool:
        return self.slope() == s.slope()

    def is_perpendicular_with(self, s: 'Segment') -> bool:
        ts = self.slope()
        ss = s.slope()
        if real_equal(ts, 0.0):
            return ss == float('inf') or ss == float('-inf')
        if ts == float('inf') or ts == float('-inf'):
            return real_equal(ss, 0.0)
        return real_equal(ts, -1.0 / ss)

    def get_perpendicular(self, p: Point2D) -> 'Segment':
        m1 = self.slope()
        m2 = -1.0 / m1 if m1 != 0 else float('inf')

        x1, y1 = self.src_point.get_x(), self.src_point.get_y()
        x2, y2 = p.get_x(), p.get_y()

        if math.isinf(m1) or math.isinf(-m1):
            x = x1
            y = m2 * (x - x2) + y2 if not (math.isinf(m2) or math.isinf(-m2)) else y2
        elif math.isinf(m2) or math.isinf(-m2):
            x = x2
            y = m1 * (x - x1) + y1 if not (math.isinf(m1) or math.isinf(-m1)) else y1
        else:
            x = (y2 - y1 + m1 * x1 - m2 * x2) / (m1 - m2)
            y = m1 * (x - x1) + y1

        q = Point2D(x, y)
        return Segment(p, q)

    def counterclockwise_angle_with(self, s: 'Segment') -> float:
        dxt = self.tgt_point.get_x() - self.src_point.get_x()
        dyt = self.tgt_point.get_y() - self.src_point.get_y()
        dxs = s.tgt_point.get_x() - s.src_point.get_x()
        dys = s.tgt_point.get_y() - s.src_point.get_y()
        dot = dxt * dxs + dyt * dys
        det = dxt * dys - dyt * dxs
        return math.atan2(det, dot)

    def get_opposite(self) -> 'Segment':
        return Segment(self.tgt_point, self.src_point)

    def intersection_with(self, s: 'Segment') -> Point2D:
        if self.is_parallel_with(s):
            raise ValueError("Segments are parallels")
        x1, y1 = self.src_point.get_x(), self.src_point.get_y()
        x2, y2 = s.src_point.get_x(), s.src_point.get_y()
        m1, m2 = self.slope(), s.slope()

        if math.isinf(m1) or math.isinf(-m1):
            x = x1
            y = m2 * (x - x2) + y2
        elif math.isinf(m2) or math.isinf(-m2):
            x = x2
            y = m1 * (x - x1) + y1
        else:
            x = (y2 - y1 + m1 * x1 - m2 * x2) / (m1 - m2)
            y = m1 * (x - x1) + y1
        return Point2D(x, y)

    def is_null(self) -> bool:
        return self.src_point == self.tgt_point

    def __bool__(self) -> bool:
        return not self.is_null()

    def __eq__(self, s: object) -> bool:
        if not isinstance(s, Segment): return False
        return (self.src_point == s.src_point and self.tgt_point == s.tgt_point) or \
               (self.src_point == s.tgt_point and self.tgt_point == s.src_point)

    def __ne__(self, s: object) -> bool:
        return not self.__eq__(s)

SegmentInt = Segment
