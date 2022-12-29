#include "dijkstra.h"

namespace paths
{

    // Berechnet die kürzesten Wege für den Knoten mit startIndex. Der gerichtete Graph wird als Adjazenzliste übergeben.
    void ComputeShortestPathsByDijkstra(int startIndex, const Graph &adjacencyList, std::vector<double> &minimumDistances, std::vector<int> &previousVertices, std::map<int, utils::Vector2> &vertexPositions)
    {

        int numberOfVertices = adjacencyList.size(); // Anzahl der Knoten
                                                     // Initialisiert den Vektor für die kleinsten Abstände
        minimumDistances.clear();
        minimumDistances.resize(numberOfVertices, maximumWeight);
        minimumDistances[startIndex] = 0;
        // Initialisiert den Vektor für die Vorgängerknoten
        previousVertices.clear();
        previousVertices.resize(numberOfVertices, -1);
        std::set<std::pair<double, int>> vertexQueue;
        vertexQueue.insert(std::make_pair(minimumDistances[startIndex], startIndex));    // Warteschlange, die die Knoten des kürzesten Wegs enthält. Fügt den Startknoten hinzu.
        vertexPositions.insert(std::make_pair(startIndex, vertexPositions[startIndex])); // Zuordnungstabelle, die die Knoten des kürzesten Wegs enthält. Fügt den Startknoten hinzu.
        while (!vertexQueue.empty())                                                     // Solange die Warteschlange nicht leer ist
        {
            double distance = vertexQueue.begin()->first; // Abstand
            int index = vertexQueue.begin()->second;
            vertexQueue.erase(vertexQueue.begin()); // Entfernt den ersten Knoten der Warteschlange
            const std::vector<paths::Neighbor> &neighbors = adjacencyList[index];
            // Diese for-Schleife durchläuft alle Nachbarn des Knoten mit index
            if (!neighbors.empty())
            {
                for (std::vector<paths::Neighbor>::const_iterator neighborIterator = neighbors.begin(); neighborIterator != neighbors.end(); neighborIterator++)
                {
                    int targetIndex = neighborIterator->targetIndex;         // Index des Nachbarknotens
                    const utils::Vector2 &name = neighborIterator->position; // Name des Nachbarknotens
                    double weight = neighborIterator->weight;                // Abstand zum Nachbarknoten
                    double currentDistance = distance + weight;              // Abstand vom Startknoten zum Knoten mit index
                    if (currentDistance < minimumDistances[targetIndex])     // Wenn der Abstand zum Nachbarknoten kleiner als die Länge des bisher kürzesten Wegs ist
                    {
                        vertexQueue.erase(std::make_pair(minimumDistances[targetIndex], targetIndex));  // Entfernt den Knoten aus der Warteschlange
                        vertexPositions.erase(targetIndex);                                             // Entfernt den Namen des Knotens aus der Zuordnungstabelle
                        minimumDistances[targetIndex] = currentDistance;                                // Speichert den Abstand vom Startknoten
                        previousVertices[targetIndex] = index;                                          // Speichert den Index des Vorgängerknotens
                        vertexQueue.insert(std::make_pair(minimumDistances[targetIndex], targetIndex)); // Fügt den Knoten der Warteschlange hinzu
                        vertexPositions.insert(std::make_pair(targetIndex, name));                      // Fügt den Namen des Knotens der Zuordnungstabelle hinzu
                    }
                }
            }
        }
    }

    std::list<utils::Vector2> GetShortestPathTo(int index, std::vector<int> &previousVertices, std::map<int, utils::Vector2> &vertexPositions)
    {
        std::list<utils::Vector2> path;
        for (; index != -1; index = previousVertices[index]) // Diese for-Schleife durchläuft den Weg
        {
            path.push_front(vertexPositions[index]); // Fügt den Namen des Vorgängerknotens hinzu
        }
        return path;
    }

};