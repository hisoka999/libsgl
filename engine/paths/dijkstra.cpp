#include "dijkstra.h"

namespace paths
{

    // Berechnet die kürzesten Wege für den Knoten mit startIndex. Der gerichtete Graph wird als Adjazenzliste übergeben.
    void ComputeShortestPathsByDijkstra(int startIndex, const Graph &adjacencyList, std::map<int, double> &minimumDistances, std::map<int, int> &previousVertices, std::map<int, utils::Vector2> &vertexPositions)
    {

        minimumDistances.clear();
        // minimumDistances.resize(numberOfVertices, maximumWeight);
        for (auto &adj : adjacencyList)
        {
            minimumDistances[adj.first] = maximumWeight;
            previousVertices[adj.first] = -1;
        }
        minimumDistances[startIndex] = 0;
        // Initialisiert den Vektor für die Vorgängerknoten

        std::set<std::pair<double, int>> vertexQueue;
        vertexQueue.insert(std::make_pair(minimumDistances[startIndex], startIndex));    // Warteschlange, die die Knoten des kürzesten Wegs enthält. Fügt den Startknoten hinzu.
        vertexPositions.insert(std::make_pair(startIndex, vertexPositions[startIndex])); // Zuordnungstabelle, die die Knoten des kürzesten Wegs enthält. Fügt den Startknoten hinzu.
        while (!vertexQueue.empty())                                                     // Solange die Warteschlange nicht leer ist
        {
            double distance = vertexQueue.begin()->first; // Abstand
            int index = vertexQueue.begin()->second;
            vertexQueue.erase(vertexQueue.begin()); // Entfernt den ersten Knoten der Warteschlange
            const std::vector<Neighbor> &neighbors = adjacencyList.at(index);
            // Diese for-Schleife durchläuft alle Nachbarn des Knoten mit index
            for (auto &neighbor : neighbors)
            {
                int targetIndex = neighbor.targetIndex;              // Index des Nachbarknotens
                auto &position = neighbor.position;                  // Name des Nachbarknotens
                double weight = neighbor.weight;                     // Abstand zum Nachbarknoten
                double currentDistance = distance + weight;          // Abstand vom Startknoten zum Knoten mit index
                if (currentDistance < minimumDistances[targetIndex]) // Wenn der Abstand zum Nachbarknoten kleiner als die Länge des bisher kürzesten Wegs ist
                {
                    vertexQueue.erase(std::make_pair(minimumDistances[targetIndex], targetIndex));  // Entfernt den Knoten aus der Warteschlange
                    vertexPositions.erase(targetIndex);                                             // Entfernt den Namen des Knotens aus der Zuordnungstabelle
                    minimumDistances[targetIndex] = currentDistance;                                // Speichert den Abstand vom Startknoten
                    previousVertices[targetIndex] = index;                                          // Speichert den Index des Vorgängerknotens
                    vertexQueue.insert(std::make_pair(minimumDistances[targetIndex], targetIndex)); // Fügt den Knoten der Warteschlange hinzu
                    vertexPositions.insert(std::make_pair(targetIndex, position));                  // Fügt den Namen des Knotens der Zuordnungstabelle hinzu
                }
            }
        }
    }

    std::list<utils::Vector2> GetShortestPathTo(int index, std::map<int, int> &previousVertices, std::map<int, utils::Vector2> &vertexPositions)
    {
        std::list<utils::Vector2> path;
        if (vertexPositions.count(index) > 0)
        {
            for (; index != -1; index = previousVertices[index]) // Diese for-Schleife durchläuft den Weg
            {
                path.push_front(vertexPositions.at(index)); // Fügt den Namen des Vorgängerknotens hinzu
            }
        }
        return path;
    }

};