"""
@file geom.py

This file contains the declaration and implementation of various geometric
functions required for game logic, such as collision detection,
translation, rotation, and vision area computation.
"""

from typing import List, Tuple
from definitions import *
from lib import Point2D, Vector2D, Segment, Polygon

def is_inside(p: Point2D, o) -> bool:
    """
    Checks whether point p is inside an obstacle, a list of obstacles, or a VisionArea.

    The behavior depends on the type of o:
        - If o is a list, it checks if p is inside any obstacle in the list.
        - If o is a tuple (VisionArea), it checks if p is inside the circular sector.
        - If o is an Obstacle (Polygon), it checks if p is inside the polygon.

    Args:
        p (Point2D): The point to check.
        o (Obstacle | List[Obstacle] | VisionArea): The geometric entity.

    Returns:
        bool: True if the point is inside, False otherwise.
    """
    pass

def intersects(s: Segment, o) -> bool:
    """
    Checks whether segment s intersects with an obstacle or a list of obstacles.

    Args:
        s (Segment): The segment to check.
        o (Obstacle | List[Obstacle]): The obstacle or list of obstacles.

    Returns:
        bool: True if the segment intersects any obstacle, False otherwise.
    """
    pass

def translate(v: Vector2D, dx: float, dy: float) -> Vector2D:
    """
    Translates a vector by dx units horizontally and dy units vertically.

    Args:
        v (Vector2D): The original vector.
        dx (float): Translation in the x-axis.
        dy (float): Translation in the y-axis.

    Returns:
        Vector2D: The newly translated vector.
    """
    pass

def rotate(v: Vector2D, a: float) -> Vector2D:
    """
    Rotates a vector by a given angle (in degrees) with respect to the origin (0, 0).

    Args:
        v (Vector2D): The vector to rotate.
        a (float): The angle in degrees.

    Returns:
        Vector2D: The rotated vector.
    """
    pass

def rotate_around(v: Vector2D, a: float, u: Vector2D) -> Vector2D:
    """
    Rotates a vector by a given angle (in degrees) with respect to an arbitrary point u.

    Args:
        v (Vector2D): The vector to rotate.
        a (float): The angle in degrees.
        u (Vector2D): The point to rotate around.

    Returns:
        Vector2D: The rotated vector.
    """
    pass

def build_vision_area(p: Vector2D, f: Vector2D, r: float, a: float) -> VisionArea:
    """
    Builds a vision area (circular sector) for a given point and direction.

    The returned VisionArea is a tuple (Position, RightLimit, LeftLimit, Radius).

    Args:
        p (Vector2D): The position of the observer.
        f (Vector2D): The forward direction vector.
        r (float): The radius of vision.
        a (float): The field of view angle (half-angle) in degrees.

    Returns:
        VisionArea: Tuple containing (position, right limit point, left limit point, radius).
    """
    pass

# --- Graham Scan Algorithm ---

def convex_hull_graham(points: List[Point2D]) -> List[Point2D]:
    """
    Computes the convex hull of a set of points using the Graham Scan algorithm.
    Args:
        points (List[Point2D]): The list of points to compute the convex hull for.
    Returns:
        List[Point2D]: The vertices of the convex hull in counterclockwise order.
    """
    pass

# --- Jarvis March (Gift Wrapping) ---

def convex_hull_jarvis(points: List[Point2D]) -> List[Point2D]:
    """
    Computes the convex hull of a set of points using the Jarvis March (Gift Wrapping) algorithm.
    Args:
        points (List[Point2D]): The list of points to compute the convex hull for.
    Returns:
        List[Point2D]: The vertices of the convex hull in counterclockwise order.
    """
    pass