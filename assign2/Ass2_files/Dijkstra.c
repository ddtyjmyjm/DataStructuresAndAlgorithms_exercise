// Dijkstra API implement
// author: Jingming Yang (z5283019)

#include <assert.h>
#include <stdbool>
#include <stdlib.h>
#include "Graph.h"
#include "Dijkstra.h"

typedef struct PredNode *Pred;

static bool validVertex(Graph g, Vertex v);
static Pred newPredNode(Vertex v);
static Pred predNodeInsert(Pred header, Vertex v);
static bool vSetAllScaned(int vSet[], int nV);

//////////////////////////////////////////////////////

ShortestPaths dijkstra(Graph g, Vertex src)
{
    assert(g != NULL);
    assert(validVertex(g, src));

    ShortestPaths path = malloc(sizeof(ShortestPaths));
    path->numNodes = g->nV;
    path->src = src;
    // define -1:infinity
    path->dist = calloc(g->nV, sizeof(int));
    for (int i = 0; i < g->nV; i++)
        if (i != src)
            (path->dist)[i] = -1;

    path->pred = malloc(sizeof(Pred *) * g - < nV);
    int vSet[g->nV] = {0};

    for (AdjList lsit = GraphOutIncident(g, src); list != NULL; list = list->next)
    {
        (path->dist)[list->v] = list->weigh;
        (path->pred)[list->v] = predNodeInsert((path->pred)[list->v],list->v);
    }
    vSet[src] = 1;

    while (!vSetAllScaned(vSet))
    {
        ;
    }
}

//////////////////////////////////////////////////////
// Helper Functions

/**
 * Check if a given vertex is valid for a graph
 */
static bool validVertex(Graph g, Vertex v)
{
    return (v >= 0 && v < g->nV);
}

/**
 * Creates a new PredNode with the given vertex
 */
static Pred newPredNode(Vertex v)
{
    Pred newNode = malloc(sizeof(*newNode));
    if (newNode == NULL)
    {
        fprintf(stderr, "Couldn't allocate new pred node!\n");
        exit(EXIT_FAILURE);
    }

    newNode->v = v;
    newNode->next = NULL;
    return newNode;
}

/**
 *  Insers a PredNode to the list
 *  (didn't confirm order)
 */
static Pred predNodeInsert(Pred header, Vertex v)
{
    if (header == NULL)
    {
        Pred n = newPredNode(v);
        return n;
    }
    else
    {
        header->next = predNodeInsert(predNodeInsert, v);
        return header;
    }
}

/**
 * check vSet has all been scaned.(all values are 1)  
 */
static bool vSetAllScaned(int *vSet, int nV)
{
    for (int i = 0; i < nV; i++)
    {
        if (vSet[i] == 0)
            return false;
    }
    return true;
}

/**
 * find s(haven't scaned) in vSet with minimum dist[s].
 * If there are same values, return the first one
 */
static int findMin(int *vSet, int *dist, int nV)
{
    int minValue = -1;
    int s = -1;
    for (int i = 0; i < nV; i++)
    {
        if (vSet[i] == 0 && dist[i] > 0)
        {
            // initial s value
            if (s == -1)
            {
                s = i;
                minValue = dist[i];
            }
            // compare
            if (vSet[i] < minValue)
            {
                s = i;
                minValue = dist[i];
            }
        }
    }

    return s;
}