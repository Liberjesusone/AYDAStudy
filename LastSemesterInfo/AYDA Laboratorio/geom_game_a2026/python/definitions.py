import math
from typing import Tuple
from lib import Point2D, Vector2D, Polygon

Obstacle = Polygon

sin0 = 0.0
sin30 = 0.5
sin45 = 0.707106781
sin60 = 0.866025404
sin90 = 1.0
cos0 = 1.0
cos30 = 0.866025404
cos45 = 0.707106781
cos60 = 0.5
cos90 = 0.0

def degree_to_radian(a: float) -> float:
    PI = 3.14159265358979323846
    return a * PI / 180.0

def real_equal(a: float, b: float) -> bool:
    return abs(a - b) <= 1e-6

def dsin(a: float) -> float:
    if a < 0.0: return -dsin(-a)
    if real_equal(a, 0.0): return sin0
    if real_equal(a, 30.0): return sin30
    if real_equal(a, 45.0): return sin45
    if real_equal(a, 60.0): return sin60
    if real_equal(a, 90.0): return sin90
    return math.sin(degree_to_radian(a))

def dcos(a: float) -> float:
    if a < 0.0: return dcos(-a)
    if real_equal(a, 0.0): return cos0
    if real_equal(a, 30.0): return cos30
    if real_equal(a, 45.0): return cos45
    if real_equal(a, 60.0): return cos60
    if real_equal(a, 90.0): return cos90
    return math.cos(degree_to_radian(a))

class Mat2D:
    def __init__(self, m11: float, m12: float, m21: float, m22: float):
        self.m11 = m11; self.m12 = m12
        self.m21 = m21; self.m22 = m22

    def __mul__(self, v: Vector2D) -> Vector2D:
        return Vector2D(
            self.m11 * v.get_x() + self.m12 * v.get_y(),
            self.m21 * v.get_x() + self.m22 * v.get_y()
        )

# VisionArea (pos, right, left, radius)
VisionArea = Tuple[Vector2D, Vector2D, Vector2D, float]
