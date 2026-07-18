/**
 * @file template.cpp
 * @brief Examen Práctico 4: Emergencia en La Hechicera (Geometría Computacional)
 * Facultad de Ingeniería - ULA, Mérida.
 */

#include <vector>
#include <stdexcept>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <list>
#include <tuple>
#include "Definitions.hpp"
#include "Geom.hpp"

using namespace std;
using namespace geom_lib;

// ========================================================================
// EJERCICIO 1: Línea de Contención
// ========================================================================
/**
 * @brief Construye la frontera óptima alrededor del fuego y verifica que las
 * instalaciones de la ULA estén a salvo.
 * @throws std::runtime_error("LA_HECHICERA_COMPROMETIDA") si una estación está en peligro.
 * * HINT: Recuerda que la frontera más eficiente para encerrar un conjunto de puntos
 * sin concavidades redundantes tiene un nombre específico en geometría.
 */
Polygon build_secure_perimeter(const vector<Point2D>& fire_points, const vector<Point2D>& stations) {
    // TODO: Implementar la construcción del perímetro y la validación de seguridad.
    throw std::logic_error("NOT_IMPLEMENTED");
}

// ========================================================================
// EJERCICIO 2: Huella de Destrucción por Viento
// ========================================================================
/**
 * @brief Calcula el área de la superficie total amenazada considerando el viento.
 * * HINT: Un polígono no es más que una secuencia ordenada de vértices. Si conoces
 * las coordenadas de los vértices del nuevo macro-polígono, existe un teorema
 * algebraico para hallar su área exacta sin importar los polígonos que lo formen.
 */
double calculate_buffer_zone_area(const Polygon& p_fire, const Vector2D& wind_vector) {
    // TODO: Implementar el cálculo de la huella combinada y su área.
    throw std::logic_error("NOT_IMPLEMENTED");
}

// ========================================================================
// EJERCICIO 3: Escuadrón de Drones Estáticos
// ========================================================================
/**
 * @brief Encuentra la combinación de orientaciones de drones que maximiza
 * la vigilancia de focos únicos de incendio.
 * * HINT: Dado que el número de drones es pequeño (N <= 8) y el espacio de
 * decisiones es discreto (4 opciones por dron), evalúa cómo estructurar
 * tu algoritmo para explorar el árbol de posibilidades sin omitir ninguna.
 */
vector<Vector2D> optimize_camera_angles(
    const vector<Point2D>& fire_points,
    const vector<Point2D>& camera_positions,
    double radius,
    double alpha
) {
    // TODO: Implementar la búsqueda de la configuración óptima.
    throw std::logic_error("NOT_IMPLEMENTED");
}


// ========================================================================
// I/O BOILERPLATE (NO MODIFICAR ESTA SECCIÓN)
// Canal de comunicación automatizado con el Grader.
// ========================================================================

int main(int argc, char* argv[]) {

    if (argc < 5) return 1;

    string exercise = argv[1];
    ofstream sol(argv[4]);

    try {
        if (exercise == "ex1") {
            ifstream infile(argv[2]);
            int num_fires; infile >> num_fires;
            vector<Point2D> fire_points;
            for (int i = 0; i < num_fires; ++i) {
                double x, y; infile >> x >> y;
                fire_points.push_back(Point2D(x, y));
            }
            int num_stations; infile >> num_stations;
            vector<Point2D> stations;
            for (int i = 0; i < num_stations; ++i) {
                double x, y; infile >> x >> y;
                stations.push_back(Point2D(x, y));
            }
            infile.close();

            Polygon perimeter = build_secure_perimeter(fire_points, stations);
            perimeter.for_each_vertex([&](const Point2D& v) {
                sol << fixed << setprecision(4) << v.get_x() << " " << v.get_y() << " ";
            });
            sol << "\n";
        }
        else if (exercise == "ex2") {
            ifstream infile(argv[2]);
            int num_vertices; infile >> num_vertices;
            Polygon p_fire;
            for (int i = 0; i < num_vertices; ++i) {
                double x, y; infile >> x >> y;
                p_fire.add_vertex(Point2D(x, y));
            }
            infile.close();

            ifstream params(argv[3]);
            double wx, wy; params >> wx >> wy;
            params.close();

            double area = calculate_buffer_zone_area(p_fire, Vector2D(wx, wy));
            sol << fixed << setprecision(6) << area << "\n";
        }
        else if (exercise == "ex3") {
            ifstream infile(argv[2]);
            int num_fires; infile >> num_fires;
            vector<Point2D> fire_points;
            for (int i = 0; i < num_fires; ++i) {
                double x, y; infile >> x >> y;
                fire_points.push_back(Point2D(x, y));
            }
            int num_cameras; infile >> num_cameras;
            vector<Point2D> camera_positions;
            for (int i = 0; i < num_cameras; ++i) {
                double x, y; infile >> x >> y;
                camera_positions.push_back(Point2D(x, y));
            }
            infile.close();

            ifstream params(argv[3]);
            double radius, alpha; params >> radius >> alpha;
            params.close();

            vector<Vector2D> dirs = optimize_camera_angles(fire_points, camera_positions, radius, alpha);
            for (const auto& d : dirs) sol << fixed << setprecision(1) << d.get_x() << " " << d.get_y() << " ";
            sol << "\n";
        }
    }
    catch (const std::logic_error& e) { sol << "NOT_IMPLEMENTED\n"; }
    catch (const std::runtime_error& e) { sol << e.what() << "\n"; }
    catch (...) { sol << "ERROR\n"; }

    sol.close();
    return EXIT_SUCCESS;
}