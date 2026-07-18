"""
@file template.py
@brief Examen Práctico 4: Emergencia en La Hechicera (Geometría Computacional)
Facultad de Ingeniería - ULA, Mérida.

INSTRUCCIONES:
- Completa las funciones marcadas con "TODO".
- NO modifiques las firmas de las funciones existentes.
- NO modifiques el bloque de comunicación final (Boilerplate).
"""

import sys
from typing import List
from lib import Point2D, Vector2D, Polygon
import geom

# ========================================================================
# EJERCICIO 1: Línea de Contención
# ========================================================================
def build_secure_perimeter(fire_points: List[Point2D], stations: List[Point2D]) -> Polygon:
    """
    Construye la frontera óptima alrededor del fuego y verifica que las
    instalaciones de la ULA estén a salvo.

    @throws RuntimeError("LA_HECHICERA_COMPROMETIDA") si una estación está en peligro.
    * HINT: Recuerda que la frontera más eficiente para encerrar un conjunto de puntos
      sin concavidades redundantes tiene un nombre específico en geometría.
    """
    # TODO: Implementar la construcción del perímetro y la validación de seguridad.
    raise NotImplementedError("NOT_IMPLEMENTED")

# ========================================================================
# EJERCICIO 2: Huella de Destrucción por Viento
# ========================================================================
def calculate_buffer_zone_area(p_fire: Polygon, wind_vector: Vector2D) -> float:
    """
    Calcula el área de la superficie total amenazada considerando el viento.

    * HINT: Un polígono no es más que una secuencia ordenada de vértices. Si conoces
      las coordenadas de los vértices del nuevo macro-polígono, existe un teorema
      algebraico para hallar su área exacta sin importar los polígonos que lo formen.
    """
    # TODO: Implementar el cálculo de la huella combinada y su área.
    raise NotImplementedError("NOT_IMPLEMENTED")

# ========================================================================
# EJERCICIO 3: Escuadrón de Drones Estáticos
# ========================================================================
def optimize_camera_angles(
    fire_points: List[Point2D],
    camera_positions: List[Point2D],
    radius: float,
    alpha: float
) -> List[Vector2D]:
    """
    Encuentra la combinación de orientaciones de drones que maximiza
    la vigilancia de focos únicos de incendio.

    * HINT: Dado que el número de drones es pequeño (N <= 8) y el espacio de
      decisiones es discreto (4 opciones por dron), evalúa cómo estructurar
      tu algoritmo para explorar el árbol de posibilidades sin omitir ninguna.
    """
    # TODO: Implementar la búsqueda de la configuración óptima mediante exploración recursiva.
    raise NotImplementedError("NOT_IMPLEMENTED")

# ========================================================================
# I/O BOILERPLATE (NO MODIFICAR ESTA SECCIÓN)
# Canal de comunicación automatizado con el Grader.
# ========================================================================
if __name__ == "__main__":
    if len(sys.argv) < 5: sys.exit(1)
    exercise, graph_file, params_file, sol_file = sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4]

    with open(sol_file, "w") as sol:
        try:
            if exercise == "ex1":
                with open(graph_file, "r") as f: tokens = f.read().split()
                if not tokens: raise ValueError()
                idx = 0
                num_fires = int(tokens[idx]); idx += 1
                fire_points = [Point2D(float(tokens[idx+i*2]), float(tokens[idx+i*2+1])) for i in range(num_fires)]; idx += num_fires*2
                num_stations = int(tokens[idx]); idx += 1
                stations = [Point2D(float(tokens[idx+i*2]), float(tokens[idx+i*2+1])) for i in range(num_stations)]

                perimeter = build_secure_perimeter(fire_points, stations)
                perimeter.for_each_vertex(lambda v: sol.write(f"{v.get_x():.4f} {v.get_y():.4f} "))
                sol.write("\n")

            elif exercise == "ex2":
                with open(graph_file, "r") as f: tokens = f.read().split()
                p_fire = Polygon()
                for i in range(int(tokens[0])): p_fire.add_vertex(Point2D(float(tokens[1+i*2]), float(tokens[2+i*2])))
                with open(params_file, "r") as p: ptokens = p.read().split()

                area = calculate_buffer_zone_area(p_fire, Vector2D(float(ptokens[0]), float(ptokens[1])))
                sol.write(f"{area:.6f}\n")

            elif exercise == "ex3":
                with open(graph_file, "r") as f: tokens = f.read().split()
                idx = 0
                num_fires = int(tokens[idx]); idx += 1
                fire_points = [Point2D(float(tokens[idx+i*2]), float(tokens[idx+i*2+1])) for i in range(num_fires)]; idx += num_fires*2
                num_cameras = int(tokens[idx]); idx += 1
                cameras = [Point2D(float(tokens[idx+i*2]), float(tokens[idx+i*2+1])) for i in range(num_cameras)]
                with open(params_file, "r") as p: ptokens = p.read().split()

                dirs = optimize_camera_angles(fire_points, cameras, float(ptokens[0]), float(ptokens[1]))
                for d in dirs: sol.write(f"{d.get_x():.1f} {d.get_y():.1f} ")
                sol.write("\n")

        except NotImplementedError: sol.write("NOT_IMPLEMENTED\n")
        except RuntimeError as e: sol.write(f"{str(e)}\n")
        except Exception: sol.write("ERROR\n")