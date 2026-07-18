from .point2d import Point2D
from .segment import Segment

class Polygon:
    def __init__(self, points=None):
        self.points = list(points) if points else []

    def clear(self): self.points.clear()
    def add_vertex(self, p: Point2D): self.points.append(p)
    def get_first_vertex(self) -> Point2D: return self.points[0]
    def get_last_vertex(self) -> Point2D: return self.points[-1]

    def get_first_segment(self) -> Segment:
        if len(self.points) < 2: raise ValueError("Polygon has no segments")
        return Segment(self.points[0], self.points[1])

    def get_last_segment(self) -> Segment:
        if len(self.points) < 2: raise ValueError("Polygon has no segments")
        return Segment(self.points[-1], self.points[0])

    def is_empty(self) -> bool: return len(self.points) == 0
    def size(self) -> int: return len(self.points)

    def for_each_vertex(self, op):
        for p in self.points: op(p)

    def all_vertex(self, pred) -> bool:
        return all(pred(p) for p in self.points)

    def exists_vertex(self, pred) -> bool:
        return any(pred(p) for p in self.points)

    def none_vertex(self, pred) -> bool:
        return not any(pred(p) for p in self.points)

    def for_each_segment(self, op):
        n = len(self.points)
        if n < 2: return
        for i in range(n):
            op(Segment(self.points[i], self.points[(i+1)%n]))

    def all_segment(self, pred) -> bool:
        n = len(self.points)
        if n < 2: return True
        return all(pred(Segment(self.points[i], self.points[(i+1)%n])) for i in range(n))

    def exists_segment(self, pred) -> bool:
        n = len(self.points)
        if n < 2: return False
        return any(pred(Segment(self.points[i], self.points[(i+1)%n])) for i in range(n))

    def none_segment(self, pred) -> bool:
        n = len(self.points)
        if n < 2: return True
        return not any(pred(Segment(self.points[i], self.points[(i+1)%n])) for i in range(n))

PolygonInt = Polygon
