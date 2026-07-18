import math

def real_equal(a: float, b: float) -> bool:
    return abs(a - b) <= 1e-6

class Point2D:
    def __init__(self, x: float = 0.0, y: float = 0.0):
        self.x = float(x)
        self.y = float(y)

    def get_x(self) -> float: return self.x
    def get_y(self) -> float: return self.y
    def set_x(self, x: float): self.x = float(x)
    def set_y(self, y: float): self.y = float(y)

    def nullify(self):
        self.x = 0.0
        self.y = 0.0

    def is_null(self) -> bool:
        return self.x == 0.0 and self.y == 0.0

    def is_zero(self) -> bool:
        return self.is_null()

    def square_distance_with(self, p: 'Point2D') -> float:
        dx = p.x - self.x
        dy = p.y - self.y
        return dx * dx + dy * dy

    def distance_with(self, p: 'Point2D') -> float:
        return math.sqrt(self.square_distance_with(p))

    def square_distance_to_origin(self) -> float:
        return self.x * self.x + self.y * self.y

    def distance_to_origin(self) -> float:
        return math.sqrt(self.square_distance_to_origin())

    def area_of_parallelogram(self, a: 'Point2D', b: 'Point2D', c: 'Point2D') -> float:
        return (b.get_x() - a.get_x()) * (c.get_y() - a.get_y()) - \
               (c.get_x() - a.get_x()) * (b.get_y() - a.get_y())

    def is_to_right_from(self, p: 'Point2D', q: 'Point2D') -> bool:
        return self.area_of_parallelogram(p, q, self) < 0.0

    def is_to_right_on_from(self, p: 'Point2D', q: 'Point2D') -> bool:
        return self.area_of_parallelogram(p, q, self) <= 0.0

    def is_to_left_from(self, p: 'Point2D', q: 'Point2D') -> bool:
        return self.area_of_parallelogram(p, q, self) > 0.0

    def is_to_left_on_from(self, p: 'Point2D', q: 'Point2D') -> bool:
        return self.area_of_parallelogram(p, q, self) >= 0.0

    def is_collinear_with(self, p: 'Point2D', q: 'Point2D') -> bool:
        return self.area_of_parallelogram(p, q, self) == 0.0

    def is_between(self, p: 'Point2D', q: 'Point2D') -> bool:
        if not self.is_collinear_with(p, q):
            return False
        if p.get_x() != q.get_x():
            return min(p.get_x(), q.get_x()) <= self.get_x() <= max(p.get_x(), q.get_x())
        else:
            return min(p.get_y(), q.get_y()) <= self.get_y() <= max(p.get_y(), q.get_y())

    def __bool__(self) -> bool:
        return not self.is_null()

    def __eq__(self, p: object) -> bool:
        if not isinstance(p, Point2D): return False
        return real_equal(self.x, p.x) and real_equal(self.y, p.y)

    def __ne__(self, p: object) -> bool:
        return not self.__eq__(p)

    def __str__(self) -> str:
        return f"({self.x},{self.y})"

    def __repr__(self) -> str:
        return self.__str__()

PointInt2D = Point2D
