**UNIVERSIDAD DE LOS ANDES M   E   R   I   D   A   -   V   E   N   E   Z   U   E   L   A** Escuela de Ingeniería de Sistemas **Asignatura:** Diseño y Análisis de Algoritmos **Código:** ISFDAA **Semestre:** 6to 

# Guía 0: Fundamentos y Hoja de Ruta 

Sean bienvenidos a la asignatura de Diseño y Análisis de Algoritmos. Este documento, que he denominado "Guía 0", no debe ser confundido con el programa formal del curso. Considéralo, más bien, una declaración de principios y una exposición rigurosa de la mentalidad fundamental que se exige para tener éxito, no solo en esta materia, sino en la disciplina de la Ingeniería de Sistemas en su totalidad. 

## I. El Manifiesto del Ingeniero 

Es de importancia estratégica, desde este primer momento, establecer la distinción fundamental entre la **programación y la ingeniería** , la primera es un oficio, una habilidad valiosa para traducir lógica a un lenguaje de máquina y la segunda es una ciencia, una disciplina intelectual dedicada a resolver problemas bajo restricciones del mundo real. 

Este curso se fundamenta en una filosofía central que debe guiar cada uno de sus esfuerzos a partir de hoy. El axioma es el siguiente: **Cualquiera puede escribir código que funcione para 10 datos; un ingeniero diseña sistemas que funcionen para 10 mil millones** . La diferencia entre ambas magnitudes no es cuantitativa, sino cualitativa. Representa el abismo que separa al artesano del arquitecto. 

El código del primero puede ser funcional en un entorno aislado y controlado, pero frágil, ineficiente e inescalable ante la complejidad del mundo real. El sistema del segundo está concebido desde sus cimientos para ser eficiente, robusto y predecible, sin importar la escala. Este curso es, por tanto, el puente que le permitirá cruzar desde la orilla del programador a la del ingeniero. Para construir dicho puente, nos apoyaremos en dos pilares intelectuales que deben permanecer siempre separados en su mente: **el análisis y el diseño.** 

II. Análisis vs. Diseño: Los Dos Pilares de la Disciplina 

La práctica profesional de la ingeniería exige la capacidad de separar con claridad el acto de analizar un problema del acto de diseñar su solución. Son dos fases intelectuales 

interconectadas pero distintas, confundirlas es el error más común y costoso en el desarrollo de sistemas complejos. 

La maestría en esta separación es el primer indicio de madurez profesional. 

**Análisis** : Es la aplicación de un aparato matemático formal para predecir, con precisión, el comportamiento y el consumo de recursos (tiempo y memoria) de un sistema _antes_ de su implementación. No se trata de ejecutar código y medir con un cronómetro; eso es experimentación, no análisis. Hablamos de utilizar herramientas como la inducción matemática, los límites y las sumatorias para obtener una función que modele el rendimiento de un algoritmo en función del tamaño de la entrada. Analizar es el acto de conocer el futuro computacional de una idea. 

**Diseño:** Es la arquitectura del pensamiento racional que ocurre _antes_ de tocar el teclado. Diseñar no es escribir código; es el proceso metódico y estratégico de estructurar una solución para que sea inherentemente eficiente, mantenible y elegante. Es la selección deliberada de una estructura de datos sobre otra, la elección de un paradigma algorítmico específico y la justificación formal de por qué esa arquitectura es superior a sus alternativas. El código es simplemente el material con el que se construye el edificio, pero el diseño es el plano. El dominio de estos dos pilares se fundamenta, ineludiblemente, en el conocimiento técnico que usted ya debe poseer. 

III. Inventario Técnico de Supervivencia (Prerrequisitos) 

El éxito en esta asignatura está supeditado a un dominio no negociable del conocimiento previo. Las prelaciones oficiales son **'Programación 3'** y **'Matemáticas Discretas'** , y se asume una competencia absoluta en sus áreas fundamentales. ⚠ No habrá concesiones ni repasos; se espera que usted llegue preparado para construir sobre cimientos sólidos. A continuación, se detalla el inventario mínimo de competencias que debe manejar con total fluidez: 

## **Matemáticas Discretas:** 

- Dominio absoluto de la Inducción Matemática. 

- Resolución de Sumatorias y Progresiones. 

- Fundamentos de Teoría de Grafos. 

- Lógica de Predicados y Cuantificadores. 

## **Programación:** 

- Gestión explícita de Memoria (punteros y referencias). 

- Implementación y operación de Estructuras de Datos Fundamentales (pilas, colas, listas, árboles). 

- Dominio profundo de la Recursividad. 

Si no domina a cabalidad estos temas, dispone de 7 días para nivelarse de forma autónoma. El curso no se detendrá para realizar repasos. Su preparación será puesta a prueba inmediatamente. 

Además debe contar con los siguiente Prerrequisitos: 

- Estar en el canal de <u>Discord de AYDA. Puede acceder por medio de este</u> enlace <u>https://discord.gg/4HUD8vddv</u> 

- Tener conocimiento de al menos un lenguaje de programación orientado a objetos (Sugerencias: Python, Java, C++, C#, JavaScript, Ruby, Swift y Rust) 

- Tener una cuenta en la plataforma de Retos de <u>HackerRank. Si no la tiene,</u> Registrese y vaya familiarizando con la plataforma. 

- Haber firmado los Acuerdos de Convivencia, Derechos y Deberes esto es de suma importancia. 

- En el salón de clases tener un cuaderno/hojas y lápiz/algo para escribir, hojas de exámen. 

- Antes de cada clase haber leído las Guías especialmente redactadas para usted que estarán en el Channel de **Blog.** 

El siguiente problema está diseñado para evaluar su capacidad de razonamiento analítico, la habilidad central que cultivaremos. 

IV. El Problema de Reflexión: Una Cuestión de Magnitud 

El núcleo del análisis de algoritmos reside en comprender el impacto práctico y económico de la eficiencia teórica. No es un ejercicio académico abstracto; es la diferencia entre un producto viable y un fracaso multimillonario. Considere el siguiente escenario como el primer desafío intelectual del semestre. 

Se le encarga procesar un conjunto de datos con **n = 1.000.000** de registros en un procesador estándar de **3GHz** . Dispone de dos algoritmos para resolver el problema: 

- **Algoritmo A:** Complejidad de **O(n²)** . 

- **Algoritmo B:** Complejidad de **O(n log n).** 

**Su tarea:** Sin ejecutar una sola línea de código, reflexione y estime la diferencia abismal en el tiempo de ejecución entre ambas soluciones. ¿Hablamos de segundos, minutos, horas o años? ¿Por qué esta distinción es el corazón de la Ingeniería de Sistemas?La capacidad de responder a esta pregunta con un orden de magnitud correcto, y de justificar formalmente esa respuesta, es precisamente lo que este curso busca forjar en usted. Esta habilidad de razonamiento es la única herramienta que no puede ser sustituida. 

- V. Política de Integridad Académica y el Uso de Inteligencia Artificial 

Reconocemos la existencia y el poder de las herramientas modernas, incluyendo los modelos de lenguaje y los asistentes de codificación basados en Inteligencia Artificial. La política de esta cátedra sobre su uso es clara y estricta, y se alinea con el objetivo de formar ingenieros, no operadores. 

El uso de estas herramientas de IA se define como un **"Copiloto"** . Es aceptable para la optimización de sintaxis, la depuración de errores triviales o la exploración de implementaciones alternativas de una idea que usted ya comprende a cabalidad. Sin embargo, su uso para generar la lógica fundamental de una solución o para resolver el núcleo de un problema está terminantemente prohibido y será considerado una violación a la integridad académica. Se establece la siguiente regla, la cual será aplicada sin excepción: **Cualquier solución, algoritmo o fragmento de código generado por IA que el estudiante no sea capaz de defender, explicar, analizar y optimizar manualmente en una evaluación oral o escrita, será calificado con 0 y tratado como una falta grave a la integridad académica.** 

El objetivo de esta asignatura no es entregar programas que funcionen, sino forjar ingenieros que entiendan por qué funcionan, cómo funcionan y cuán bien funcionan. Nuestra meta es desarrollar su intelecto, su rigor analítico y su capacidad para crear soluciones robustas y eficientes desde los primeros principios. Espero de cada uno de ustedes el máximo compromiso intelectual y profesional. Comencemos!󰙥 

