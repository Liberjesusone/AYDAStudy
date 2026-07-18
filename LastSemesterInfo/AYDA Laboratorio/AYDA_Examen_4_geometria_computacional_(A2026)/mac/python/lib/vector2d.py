import math
from typing import Tuple
from .point2d import Point2D, real_equal

class Vector2D(Point2D):
    ZERO = None

    def is_to_right_from(self, v: 'Vector2D') -> bool:
        return super().is_to_right_from(Vector2D.ZERO, v)

    def is_to_right_on_from(self, v: 'Vector2D') -> bool:
        return super().is_to_right_on_from(Vector2D.ZERO, v)

    def is_to_left_from(self, v: 'Vector2D') -> bool:
        return super().is_to_left_from(Vector2D.ZERO, v)

    def is_to_left_on_from(self, v: 'Vector2D') -> bool:
        return super().is_to_left_on_from(Vector2D.ZERO, v)

    def is_collinear_with(self, v: 'Vector2D') -> bool:
        return super().is_collinear_with(Vector2D.ZERO, v)

    def is_normalized(self) -> bool:
        return real_equal(self.square_magnitude(), 1.0)

    def is_unitarian(self) -> bool:
        return self.is_normalized()

    def square_magnitude(self) -> float:
        return self.square_distance_to_origin()

    def magnitude(self) -> float:
        return self.distance_to_origin()

    def length(self) -> float:
        return self.magnitude()

    def negate(self):
        self.x *= -1.0
        self.y *= -1.0

    def scale(self, r: float):
        self.x *= r
        self.y *= r

    def normalize(self):
        length = self.magnitude()
        if real_equal(length, 0.0) or real_equal(length, 1.0):
            return
        self.x *= (1.0 / length)
        self.y *= (1.0 / length)

    def is_opposite(self, v: 'Vector2D') -> bool:
        a = Vector2D(self.x, self.y)
        b = Vector2D(v.x, v.y)
        a.normalize()
        b.normalize()
        return real_equal(a.x, -b.x) and real_equal(a.y, -b.y)

    def get_opposite(self) -> 'Vector2D':
        return Vector2D(-self.x, -self.y)

    def angle_with(self, v: 'Vector2D') -> float:
        return math.atan2(self.cross_product(v), self.dot_product(v))

    def add_scaled_vector(self, v: 'Vector2D', scale: float):
        self.x += v.x * scale
        self.y += v.y * scale

    def component_product(self, v: 'Vector2D') -> 'Vector2D':
        return Vector2D(self.x * v.x, self.y * v.y)

    def dot_product(self, v: 'Vector2D') -> float:
        return self.x * v.x + self.y * v.y

    def scalar_product(self, v: 'Vector2D') -> float:
        return self.dot_product(v)

    def cross_product(self, v: 'Vector2D') -> float:
        return self.x * v.y - self.y * v.x

    def vector_product(self, v: 'Vector2D') -> float:
        return self.cross_product(v)

    def __neg__(self) -> 'Vector2D':
        return self.get_opposite()

    def __mul__(self, other) -> 'Vector2D':
        if isinstance(other, (int, float)):
            return Vector2D(self.x * other, self.y * other)
        elif isinstance(other, Vector2D):
            return self.dot_product(other)
        return NotImplemented

    def __rmul__(self, scalar: float) -> 'Vector2D':
        return Vector2D(self.x * scalar, self.y * scalar)

    def __imul__(self, scalar: float):
        self.x *= scalar
        self.y *= scalar
        return self

    def __add__(self, v: 'Vector2D') -> 'Vector2D':
        return Vector2D(self.x + v.x, self.y + v.y)

    def __iadd__(self, v: 'Vector2D'):
        self.x += v.x
        self.y += v.y
        return self

    def __sub__(self, v: 'Vector2D') -> 'Vector2D':
        return Vector2D(self.x - v.x, self.y - v.y)

    def __isub__(self, v: 'Vector2D'):
        self.x -= v.x
        self.y -= v.y
        return self

    @staticmethod
    def make_orthonormal_basis(v: 'Vector2D') -> Tuple['Vector2D', 'Vector2D']:
        a = Vector2D(v.x, v.y)
        a.normalize()
        b = Vector2D(a.get_y(), -a.get_x())
        return (a, b)

Vector2D.ZERO = Vector2D(0.0, 0.0)
