/**
 * @file Geom.hpp
 *
 * This file contains the declaration and implementation of various geometric
 * functions required for game logic, such as collision detection,
 * translation, rotation, and vision area computation.
 */

#pragma once
#include <algorithm>

/**
 * @brief Checks whether a point is inside an obstacle (polygon).
 * * @param p The point to check.
 * @param o The obstacle (polygon).
 * @return true if the point is inside the obstacle, false otherwise.
 */
inline bool is_inside(const Point2D& p, const Obstacle& o)
{
  // TODO: Implement the logic to determine if point 'p' is inside obstacle 'o'.
  bool inside = false;
  for(auto it = o.segments_begin(); it != o.segments_end(); ++it)
  {
      const auto seg = *it;
      // El punto puede estar sobre el segmento
      if(seg.contains_to(p)) {
          return true; 
      }

      const auto p1 = seg.get_src_point();
      const auto p2 = seg.get_tgt_point();
    
      if((p1.get_y() > p.get_y()) != (p2.get_y() > p.get_y())) 
      {
          double intersect_x = p1.get_x() + (p.get_y() - p1.get_y()) * (p2.get_x() - p1.get_x()) / (p2.get_y() - p1.get_y());
        
          if(p.get_x() < intersect_x) 
          {  
              inside = !inside;
          }
      }
  }
  
  return inside;
  
}

/**
 * @brief Checks whether a point is inside any of the obstacles in a list.
 * * @param p The point to check.
 * @param os The list of obstacles.
 * @return true if the point is inside any obstacle, false otherwise.
 */
inline bool is_inside(const Point2D& p, const std::list<Obstacle>& os)
{
  // TODO: Implement the logic to check if point 'p' is inside any obstacle in the list 'os'.
    for(const auto& o : os)
    {
      if(is_inside(p, o)) 
      {
        return true;
      }
    }
    return false;
}

/**
 * @brief Checks whether a segment intersects with an obstacle.
 * * @param s The segment to check.
 * @param o The obstacle.
 * @return true if the segment intersects the obstacle, false otherwise.
 */
inline bool intersects(const Segment& s, const Obstacle& o)
{
  // TODO: Implement the logic to determine if segment 's' intersects obstacle 'o'.
      // Cruzo alguna pared del poligono?
      for (auto it = o.segments_begin(); it != o.segments_end(); ++it)
      {
        if (s.intersects_with(*it)) 
        {
          return true; // Choque
        }
      }

      // Si no tocó ninguna pared, no hay intersección 
      return false;
    
}

/**
 * @brief Checks whether a segment intersects with any obstacle in a list.
 * * @param s The segment to check.
 * @param os The list of obstacles.
 * @return true if the segment intersects any obstacle in the list, false otherwise.
 */
inline bool intersects(const Segment& s, const std::list<Obstacle>& os)
{
  // TODO: Implement the logic to check if segment 's' intersects any obstacle in the list 'os'.
      for (const auto& o : os)
      {
        if (intersects(s, o)) 
        {
          return true; // Choco
        }
      }
      // Si revisó todo el mapa y no tocó nada, la ruta es segura.
      return false;
  
}

/**
 * @brief Translates a vector by dx units horizontally and dy units vertically.
 * * @param v The original vector.
 * @param dx The translation in the x-axis.
 * @param dy The translation in the y-axis.
 * @return The newly translated vector.
 */
inline Vector2D translate(const Vector2D& v, double dx, double dy)
{
  // TODO: Implement vector translation.
  return Vector2D(v.get_x() + dx, v.get_y() + dy);
}

/**
 * @brief Rotates a vector by a given angle (in degrees) with respect to the origin (0, 0).
 * * @param v The vector to rotate.
 * @param a The angle in degrees.
 * @return The rotated vector.
 */
inline Vector2D rotate(const Vector2D& v, double a)
{
  // TODO: Implement vector rotation around the origin using a rotation matrix.
      // Uso la matriz de rotacion
      Mat2D rot_matrix(dcos(a), -dsin(a), dsin(a),  dcos(a));  
      // Multiplico el vector
      return rot_matrix * v;
}

/**
 * @brief Rotates a vector by a given angle (in degrees) with respect to an arbitrary point 'u'.
 * * @param v The vector to rotate.
 * @param a The angle in degrees.
 * @param u The point to rotate around.
 * @return The rotated vector.
 */
inline Vector2D rotate_around(const Vector2D& v, double a, const Vector2D& u)
{
    // TODO: Implement vector rotation around point 'u'.
  return rotate(v - u, a) + u;
}

/**
 * @brief Builds a vision area (circular sector) for a given point and direction.
 * * @param p The position of the observer.
 * @param f The forward direction vector.
 * @param r The radius of vision.
 * @param a The field of view angle (half-angle) in degrees.
 * @return The computed VisionArea tuple.
 */
inline VisionArea build_vision_area(const Vector2D& p, const Vector2D& f, double r, double a)
{
  // TODO: Implement the construction of the VisionArea.
  // VisionArea is typically defined as a tuple: (Position, RightLimit, LeftLimit, Radius)
      // Copiamos el vector frontal y lo volvemos unitario (longitud 1)
      Vector2D dir = f;
      dir.normalize();
      
      // Lo estiramos hasta que alcance el límite del radio de visión
      dir = dir * r;
      
      // Calculamos los puntos extremos rotando el vector y anclándolos a la cámara (p)
      // Límite Derecho: Ángulo negativo (hacia la derecha / manecillas del reloj)
      Vector2D right_limit = p + rotate(dir, -a);
      
      // Límite Izquierdo: Ángulo positivo (hacia la izquierda / antihorario)
      Vector2D left_limit  = p + rotate(dir, a);
      
      // Empaquetamos todo en la tupla que exige Definitions.hpp
      return std::make_tuple(p, right_limit, left_limit, r);
}

/**
 * @brief Checks whether a point is inside a given vision area.
 * * @param p The point to check.
 * @param va The vision area.
 * @return true if the point is inside the vision area, false otherwise.
 */
inline bool is_inside(const Point2D& p, const VisionArea& va)
{
  // TODO: Implement the logic to determine if point 'p' is inside the vision area 'va'.

      // Desempaquetamos la tupla (Posición, Límite Derecho, Límite Izquierdo, Radio)
      const auto& pos   = std::get<0>(va);
      const auto& right = std::get<1>(va);
      const auto& left  = std::get<2>(va);
      const double& r   = std::get<3>(va);

      // CHECKLIST 1: El Filtro de Distancia (Hack del cuadrado)
      // Usamos el método nativo para evitar la raíz cuadrada.
      if (p.square_distance_with(pos) > (r * r)) 
      {
        return false; // Está fuera del alcance de la lente.
      }

      // CHECKLIST 2: El Filtro del Cono (Límites laterales)
      // El límite derecho fue rotado en sentido horario. El interior está a su izquierda.
      if (!p.is_to_left_on_from(pos, right)) 
      {
        return false; // Se salió por la derecha del cono.
      }

      // El límite izquierdo fue rotado en sentido antihorario. El interior está a su derecha.
      if (!p.is_to_right_on_from(pos, left)) 
      {
        return false; // Se salió por la izquierda del cono.
      }

      // Si sobrevivió a todos los filtros, ¡la cámara lo está viendo!
      return true;

}

/**
 * @brief Computes the convex hull of a set of points using the Graham Scan algorithm.
 * @param points The list of points to compute the convex hull for.
 * @return The vertices of the convex hull in counterclockwise order.
 */
inline std::vector<Point2D> convex_hull_graham(std::vector<Point2D> points) {
      // Un polígono necesita al menos 3 vértices.
      if (points.size() < 3) return points;

      // Encontrar el pivote P0 (El más bajo, y más a la izquierda en caso de empate)
      auto pivot_it = std::min_element(points.begin(), points.end(),
        [](const Point2D& a, const Point2D& b) {
          if (a.get_y() == b.get_y()) return a.get_x() < b.get_x();
          return a.get_y() < b.get_y();
        });
        
      // Intercambiamos el pivote para que quede de primero en nuestro arreglo
      std::swap(points[0], *pivot_it);
      Point2D p0 = points[0];

      // Ordenar el resto por Ángulo Polar
      // Usamos un Lambda que compara 'a' y 'b' respecto a nuestro pivote 'p0'
      std::sort(points.begin() + 1, points.end(),
        [&p0](const Point2D& a, const Point2D& b) {
          
          // Caso borde: Si están en la misma línea de visión, el más cercano va primero
          if (b.is_collinear_with(p0, a)) {
            return a.square_distance_with(p0) < b.square_distance_with(p0);
          }
          
          // Magia geométrica: 'a' tiene menor ángulo si 'b' está a la izquierda del vector p0->a
          return b.is_to_left_from(p0, a);
        });

      // Ejecutar el escaneo de la pila (Stack)
      std::vector<Point2D> hull; // Este vector actuará como nuestra pila
      
      for (const auto& p : points) 
      {
        // Mientras haya al menos 2 puntos para formar un ángulo...
        while (hull.size() >= 2) 
        {
          const Point2D& top = hull.back();
          const Point2D& prev = hull[hull.size() - 2];
          
          // Evaluamos el Determinante: ¿El nuevo punto 'p' hace un giro a la izquierda?
          // Si NO hace un giro estricto a la izquierda, significa que es una abolladura (cóncavo)
          // o es un punto colineal inútil en medio de una pared plana.
          if (!p.is_to_left_from(prev, top)) {
            hull.pop_back(); // ¡BEEP! Abolladura detectada, sacamos el vértice del tope.
          } else {
            break; // Tensión correcta, salimos del chequeo.
          }
        }
        
        // Metemos el nuevo punto validado al perímetro
        hull.push_back(p);
  }

  return hull;
  
}

/**
 * @brief Computes the convex hull of a set of points using the Jarvis March (Gift Wrapping) algorithm.
 * @param points The list of points to compute the convex hull for.
 * @return The vertices of the convex hull in counterclockwise order.
 */
inline std::vector<Point2D> convex_hull_jarvis(const std::vector<Point2D>& points) {
      // BLINDAJE: Un polígono necesita al menos 3 vértices
      if (points.size() < 3) return points;

      std::vector<Point2D> hull;

      // Encontrar el Punto Ancla (el más bajo, y más a la izquierda si hay empate)
      Point2D p0 = points[0];
      for (const auto& p : points) {
        if (p.get_y() < p0.get_y() || (p.get_y() == p0.get_y() && p.get_x() < p0.get_x())) {
          p0 = p;
        }
      }

      Point2D p_actual = p0;

      //  La marcha de Jarvis (Envolver el regalo)
      do {
        hull.push_back(p_actual);
        
        // Asignamos un candidato inicial (el primero que sea distinto a p_actual)
        Point2D candidato = points[0];
        if (candidato == p_actual) {
            candidato = points[1]; 
        }

        // El Torneo del Rey de la Colina
        for (const auto& retador : points) 
        {
          if (retador == p_actual || retador == candidato) continue;

          // Si el retador está más "a la derecha", roba el trono.
          // (Es decir, envuelve más hacia afuera en sentido antihorario).
          if (retador.is_to_right_from(p_actual, candidato)) 
          {
              candidato = retador;
          }
          // Desempate Colineal. Gana el que esté más lejos del punto actual.
          else if (retador.is_collinear_with(p_actual, candidato)) 
          {
              if (retador.square_distance_with(p_actual) > candidato.square_distance_with(p_actual)) 
              {
                  candidato = retador;
              }
          }
        }
        
        // El candidato invicto se convierte en el nuevo vértice de la frontera
        p_actual = candidato;

      } while (p_actual != p0); // Terminamos cuando la liga elástica toca de nuevo el ancla

      return hull;
}

//Funcion para calcular el area de un poligono
inline double compute_area(const Obstacle& o) 
{
    double signed_area = 0.0;
    Point2D origin(0.0, 0.0); // Nuestro ancla para los determinantes
    
    // Recorremos las aristas de forma circular
    for (auto it = o.segments_begin(); it != o.segments_end(); ++it) 
    {
        auto seg = *it; 
        // El área del paralelogramo desde el origen equivale a la iteración de Shoelace
        signed_area += origin.area_of_parallelogram(origin, seg.get_src_point(), seg.get_tgt_point()); 
    }
    
    // Retornamos la mitad del valor absoluto
    return std::abs(signed_area) / 2.0;
}


inline bool is_counter_clockwise(const Obstacle& o) 
{
    double signed_area = 0.0;
    Point2D origin(0.0, 0.0);
    
    for (auto it = o.segments_begin(); it != o.segments_end(); ++it) 
    {
        auto seg = *it; 
        signed_area += origin.area_of_parallelogram(origin, seg.get_src_point(), seg.get_tgt_point()); 
    }
    
    // Si el área tiene signo positivo, es Antihorario (Counter-Clockwise)
    return signed_area > 0.0;
}

inline double distance_to_segment(const Point2D& p, const Segment& s) 
{
   // Creamos un vector 'v' que representa la longitud y dirección del láser
    Vector2D v(s.get_tgt_point().get_x() - s.get_src_point().get_x(),
               s.get_tgt_point().get_y() - s.get_src_point().get_y());
               
    // Creamos un vector 'w' desde el inicio del láser hasta nuestro agente
    Vector2D w(p.get_x() - s.get_src_point().get_x(),
               p.get_y() - s.get_src_point().get_y());

    // Proyectamos 'w' sobre 'v' usando Producto Punto
    double c1 = w.dot_product(v); //
    if (c1 <= 0) {
        // Cayó fuera de rango por detrás. Medimos la distancia a la base.
        return p.distance_with(s.get_src_point()); //
    }

    double c2 = v.dot_product(v); // Esto equivale a la magnitud al cuadrado de 'v'
    if (c2 <= c1) {
        // Cayó fuera de rango por delante. Medimos la distancia a la punta.
        return p.distance_with(s.get_tgt_point()); //
    }

    // Cayó justo en medio. Calculamos la coordenada exacta del punto de impacto.
    double b = c1 / c2;
    Point2D projection(s.get_src_point().get_x() + b * v.get_x(),
                       s.get_src_point().get_y() + b * v.get_y());
                       
    return p.distance_with(projection); 
}







