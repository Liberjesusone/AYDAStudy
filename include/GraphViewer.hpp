#pragma once

#include <string>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <cstdio>
#include <cstdlib>

using ull = unsigned long long;
using ll = long long;

/** Exports every graph to the interactive viewer graph_viewer.html
 *
 * The complete flow is:
 *
 *   1. to_json(...)          Constructs the JSON text of the graph
 *   2. to_html(json, ...)    inyects that JSON into a copy of the viewer
 *   3. open_in_browser(...)  opens the resulting HTML in the default browser
 */
namespace gviz
{

/** @brief Escapes a string so that it is a valid JSON string.
 *
 * JSON does not allow raw quotes, backslashes, or control characters inside a 
 * string. If a label contains a newline or a quote and we put it in as-is, 
 * the JSON will be broken and the viewer won't be able to parse it. Control 
 * characters that don't have a shortcut (\n, \t, ...) are emitted in the form 
 * \uXXXX, which is what the standard requires.

 * The bytes >= 0x80 are copied as-is: if the string was UTF-8, it remains 
 * valid UTF-8, which is what the browser expects.
 *
 * @returns the string ready to be placed between quotes in the JSON
 */
inline std::string escape(const std::string& _str) noexcept
{
    std::string out;
    out.reserve(_str.size() + 8);

    for (unsigned char c : _str)
    {
        switch (c)
        {
            case '"' : out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\b': out += "\\b" ; break;
            case '\f': out += "\\f" ; break;
            case '\n': out += "\\n" ; break;
            case '\r': out += "\\r" ; break;
            case '\t': out += "\\t" ; break;
            default:
                if (c < 0x20)   // Control without own shortcut -> \u00XX
                {
                    char buf[8];
                    std::snprintf(buf, sizeof(buf), "\\u%04x", c);
                    out += buf;
                }
                else out += static_cast<char>(c);
        }
    }

    return out;
}


/** @brief Acumulator that builds the JSON text of the graph piece by piece.
 *
 * Instead of manually concatenating commas and braces in the middle of the 
 * traversal (where syntax errors are prone to occur), you call add_node() 
 * and add_edge() in the order you want, and at the end str() closes everything.
 *
 * The produced format is what the viewer expects:
 *
 *   {
 *     "directed": false,
 *     "weighted": false,
 *     "nodes": [ { "id":0, "label":"A" } ],
 *     "edges": [ { "from":0, "to":1, "weight":1 } ]
 *   }
 *
 * We don't emit "x"/"y": without positions, the viewer places the nodes in a 
 * circle and starts only the force-directed layout, which is what we want most of the time.
 */
class Json
{
    private:
        std::string nodes{};
        std::string edges{};
        ull n_count{0};
        ull e_count{0};

    public:
        /** @brief Adds a node. @param _id must be unique and is the one 
         * that edges will reference later (naturally, the index in adj[]).
         */
        void add_node(ull _id, const std::string& _label) noexcept
        {
            if (n_count++) nodes += ',';
            nodes += "{\"id\":" + std::to_string(_id)
                   + ",\"label\":\"" + escape(_label) + "\"}";
        }

        /** @brief Adds an edge between two nodes that have already been added.
         *
         * In an undirected graph, this function should be called only once per edge.
         */
        void add_edge(ull _from, ull _to, ll _weight = 1) noexcept
        {
            if (e_count++) edges += ',';
            edges += "{\"from\":" + std::to_string(_from)
                   + ",\"to\":"   + std::to_string(_to)
                   + ",\"weight\":" + std::to_string(_weight) + "}";
        }

        ull nodes_added() const noexcept { return n_count; }
        ull edges_added() const noexcept { return e_count; }

        /** @returns The complete JSON string, ready for the to_html() */
        std::string str(bool _directed = false, bool _weighted = false) const noexcept
        {
            return std::string("{\"directed\":") + (_directed ? "true" : "false")
                 + ",\"weighted\":" + (_weighted ? "true" : "false")
                 + ",\"nodes\":[" + nodes
                 + "],\"edges\":[" + edges + "]}";
        }
};


/** @brief Convierte un grafo cualquiera en el JSON que entiende el viewer.
 *
 * La idea es que esta función no sepa nada de cómo está hecho el grafo: sólo
 * le pide un recorrido y una forma de etiquetar. Así sirve igual para LGraph,
 * para el dirigido que hagas después, o para cualquier otro que exponga el
 * mismo recorrido.
 *
 * @param _graph  el grafo. Debe ofrecer un recorrido que visite TODOS los
 *                nodos (recorrer adj[] directamente, no un BFS desde adj[0]:
 *                un BFS se salta las componentes desconectadas) y que le pase
 *                al lambda el índice del nodo, su valor y sus vecinos.
 *
 * @param _label  lambda que recibe el valor del nodo y devuelve el string que
 *                se dibuja debajo del círculo. Ej. para un grafo de usuarios:
 *                    [](const User& u) { return u.get_name().substr(0, 14); }
 *
 * Dos cosas a resolver dentro:
 *
 *   - Los nodos van primero: una arista que apunte a un id que todavía no
 *     existe la descarta el viewer. Recorre agregando nodos, y en la misma
 *     pasada (o en otra) agrega las aristas.
 *
 *   - Si el grafo NO es dirigido, cada arista aparece dos veces en las listas
 *     de adyacencia (una desde cada extremo). Si las agregas las dos, el
 *     viewer dibuja dos líneas encimadas. Piensa qué condición sencilla sobre
 *     los dos índices deja pasar sólo una de las dos.
 *
 * @returns el texto JSON (usa Json::str() al final)
 */
template <class Graph, class LabelFn>
std::string to_json(const Graph& _graph, LabelFn _label,
                    bool _directed = false, bool _weighted = false) noexcept
{
    Json json;

    // 1. We have to add all nodes (id, name)
    auto add_node = [&json, &_label](const auto& _node) -> void 
    {
        json.add_node(_node.get_id(), _label(_node.get_value()));
    };
    _graph.for_each(add_node);

    // 2. We add all the edges 
    auto add_edge = [&json, &_label](ull _from, ull _to, ll _weight) -> void 
    {
        json.add_edge(_from, _to, _weight);
    };
    _graph.for_each_edge(add_edge);

    return json.str(_directed, _weighted);
}


/** @brief Injects the JSON into a copy of the viewer and writes it to disk.
 *
 * The browser blocks an HTML page opened with file:// from reading another
 * file on disk, so the viewer cannot load a .json on its own. The way out is
 * to leave the graph embedded inside the HTML itself: the template carries
 * the token/variable
 *
 *     const EMBEDDED = [marker]null;
 *
 * and here we substitute that string for the JSON text. The viewer detects
 * that EMBEDDED is no longer null and starts with your graph instead of the
 * demo.
 *
 * About file handling, which is what you may not have used before:
 *
 *   std::ifstream in(path)   opens for reading; if (!in) detects the failure.
 *   The std::string constructor taking two istreambuf_iterators reads the
 *   whole file in one go (the extra pair of parentheses around the first
 *   argument avoids the "most vexing parse", where the compiler would read
 *   the line as a function declaration instead of a variable).
 *   std::ofstream out(path)  opens for writing, truncating whatever was there.
 *   Both close themselves when leaving the scope (RAII), no need to call close().
 *
 * We open in binary on purpose: in text mode Windows converts every \n into
 * \r\n when writing, and since the template already carries \r\n we would end
 * up with \r\r\n.
 *
 * @param _json           the JSON produced by to_json()
 * @param _template_path  the template; it is NOT modified
 * @param _output_path    file to generate. It must differ from the template:
 *                        if you overwrite it you lose the marker and the next
 *                        export will no longer find where to inject.
 * @throws std::runtime_error if the files cannot be opened or the marker is missing
 */
inline void to_html(const std::string& _json,
                    const std::string& _template_path = "graph_viewer.html",
                    const std::string& _output_path   = "output.html")
{
    if (_template_path == _output_path)
        throw std::runtime_error("to_html: la salida no puede ser la plantilla misma");

    // -- Read the whole template into a string
    std::ifstream in(_template_path, std::ios::binary);
    if (!in)
        throw std::runtime_error("to_html: couldn't be opened '" + _template_path + "'");

    std::string html((std::istreambuf_iterator<char>(in)),
                      std::istreambuf_iterator<char>());
    in.close();

    // -- Locate the mark. Should be exactly once: if it has been repeated, we would 
    //    replace just the first one, and the file would be broken
    const std::string mark = "/*GRAPH_DATA*/null";

    const auto pos = html.find(mark);
    if (pos == std::string::npos)
        throw std::runtime_error("to_html: the mark was not found in the template "
                                 "(¿Was it overwriten/modified before?)");

    if (html.find(mark, pos + mark.size()) != std::string::npos)
        throw std::runtime_error("to_html: the mark appears more than once");

    html.replace(pos, mark.size(), _json);

    // -- Write the result 
    std::ofstream out(_output_path, std::ios::binary);
    if (!out)
        throw std::runtime_error("to_html: it couldn't be written '" + _output_path + "'");

    out << html;
}

 /** @brief Opens a file with the default system app  
  * 
  * In Windows the command is `start`, but it has a trap: interpretes the first 
  * argument in quotation marks as the TITLE of the window, not as the route. 
  * That's why we use "" empty before the route; without him, the route with spaces
  * is taken as the title and nothing is open
  * 
  * @param _path file route to open.
  */
inline void open_in_browser(const std::string& _path) noexcept
{
#if defined(_WIN32)
    std::system(("start \"\" \"" + _path + "\"").c_str());
#elif defined(__APPLE__)
    std::system(("open \"" + _path + "\"").c_str());
#else
    std::system(("xdg-open \"" + _path + "\"").c_str());
#endif
}


/** @brief Shortcut: graph -> JSON -> HTML -> browser, in one call.
 */
template <class Graph, class LabelFn>
void show(const Graph& _graph, LabelFn _label,
          bool _directed = false, bool _weighted = false,
          const std::string& _template_path = "graph_viewer.html",
          const std::string& _output_path   = "output.html")
{
    to_html(to_json(_graph, _label, _directed, _weighted),
            _template_path, _output_path);
    open_in_browser(_output_path);
}

} // namespace gviz
