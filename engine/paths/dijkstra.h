#pragma once

#include <iostream>
#include <limits>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <algorithm>
#include <iterator>
#include <string>
#include "../utils/vector2.h"
namespace paths
{

    const double maximumWeight = std::numeric_limits<double>::infinity(); // Konstante für das maximale Gewicht

    // Datentyp, der die Nachbarknoten eines Knotens definiert
    struct Neighbor
    {
        int targetIndex; // Index des Zielknotens
        utils::Vector2 position;
        double weight; // Gewicht der Kante
        Neighbor(int _target, const utils::Vector2 &position, double _weight) : targetIndex(_target), position(position), weight(_weight) {}
    };

    typedef std::vector<std::vector<Neighbor>> Graph;

    void ComputeShortestPathsByDijkstra(int startIndex, const Graph &adjacencyList, std::vector<double> &minimumDistances, std::vector<int> &previousVertices, std::map<int, utils::Vector2> &vertexPositions);

    // Gibt einen kürzesten Weg für den Knoten mit index zurück
    std::list<utils::Vector2> GetShortestPathTo(int index, std::vector<int> &previousVertices, std::map<int, utils::Vector2> &vertexPositions);

} // namespace paths
