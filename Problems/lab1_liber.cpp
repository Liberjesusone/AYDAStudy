// By Liber Jesus Avendaño Puccini V031622543

#include <iostream>
#include <vector>
#include <algorithm>

// FUNCIÓN PRINCIPAL A IMPLEMENTAR
int resolverMaximoSubarreglo(const std::vector<int>& registros, int inicio, int fin) {
    // CASO BASE: Si el rango se reduce a un solo elemento
    if (inicio == fin) {
        return registros[inicio];
    }
    if (fin < 0) return registros[inicio];
    
    int lenght = fin - inicio + 1;

    if (lenght == 1) {
        return registros[inicio];
    }
    if (lenght == 2) {
        auto sum = registros[inicio] + registros[fin];
        auto _max_de_dos = std::max(registros[inicio], registros[fin]);
        if (_max_de_dos < 0) return std::max(sum, registros[inicio]);
        if (inicio != 0) return sum; 
        return std::max(sum, _max_de_dos);
    }

    int medio = inicio + (fin - inicio) / 2;

    // 1. Mitad Izquierda (Recursivo)
    // int maxIzquierda = ...
    int max_izq = resolverMaximoSubarreglo(registros, inicio, medio-1);

    // 2. Mitad Derecha (Recursivo)
    // int maxDerecha = ...
    int max_der = resolverMaximoSubarreglo(registros, medio+1, fin);

    // 3. Cruzando el Centro
    // int maxCruzado = ...
    int izq_mas_centro = max_izq + registros[medio];
    int centro_mas_der = max_der + registros[medio];
    
    int max_extremo = std::max(max_izq, max_der);
    int max_unido = std::max(izq_mas_centro, centro_mas_der);
    int all = max_der + registros[medio] + max_izq;
    
    return std::max(max_extremo, 
           std::max(max_unido, all));

    // Retornar el mayor de los tres escenarios
    return 0; // Cambiar por la lógica correcta
}

int main() {
    int n;
    if (!(std::cin >> n)) return 0;
    
    std::vector<int> registros(n);
    for (int i = 0; i < n; i++) {
        std::cin >> registros[i];
    }

    int resultado = resolverMaximoSubarreglo(registros, 0, n - 1);
    std::cout << resultado << std::endl;

    return 0;
}

/*
7
10 20 -50 -30 1 3 7

6
-5 10 3 -2 4 -9

4 
-2 -3 -1 -5

5
1 2 3 4 5

7 
2 -4 6 8 -1 5 -3

15
10 20 30 -15 -70 -8 -60 -2 -40 -3 -46 -70 -10 -6 -7
*/