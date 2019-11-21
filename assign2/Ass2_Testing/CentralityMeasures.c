// Centrality Measures ADT implementation
// COMP2521 Assignment 2

#include <stdio.h>
#include <stdlib.h>

#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "PQ.h"

NodeValues closenessCentrality(Graph g)
{
    NodeValues nvs;
    nvs.numNodes = GraphNumVertices(g);
    nvs.values = malloc(sizeof(double) * nvs.numNodes);
    for (int i = 0; i < nvs.numNodes; i++)
    {
        int n = 1;
        double sum = 0;
        ShortestPaths paths = dijkstra(g, i);
        for (int j = 0; j < nvs.numNodes; j++)
        {
            if (paths.dist[j] != 0 && j != i)
            {
                n += 1;
                sum += paths.dist[j];
            }
        }
        if (sum == 0)
            nvs.values[i] = 0;
        else
            nvs.values[i] = (n - 1) * (n - 1) / ((nvs.numNodes - 1) * sum);
    }

    return nvs;
}

NodeValues betweennessCentrality(Graph g)
{
    NodeValues nvs = {0};
    return nvs;
}

NodeValues betweennessCentralityNormalised(Graph g)
{
    NodeValues nvs = {0};
    return nvs;
}

void showNodeValues(NodeValues nvs)
{
    for (int i = 0; i < nvs.numNodes; i++)
    {
        printf("%d: %f\n", i, nvs.values[i]);
    }
}

void freeNodeValues(NodeValues nvs)
{
}
