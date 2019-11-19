// Dijkstra API implement
// author: Jingming Yang ( //-1sizesizeof()distsizesizeof()vSet////

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Graph.h"
#include "Dijkstra.h"

typedef struct PredNode *Pred;

static bool validVertex(Graph g, Vertex v);

static Pred newPredNode(Vertex v);

static Pred predNodeInsert(Pred header, Vertex v);

static void freePredNode(Pred pred);

static bool vSetAllScaned(int vSet[], int nV);

static int findMin(int *vSet, int *dist, int nV);

static void noConnectedDist(int *set, int *dist, int nV);
//////////////////////////////////////////////////////

ShortestPaths dijkstra(Graph g, Vertex src)
{
    assert(g != NULL);
    assert(validVertex(g, src));
    /////----initialazion ShortestPaths----/////
    // define dist -1:infinity | 0:itself
    ShortestPaths path;
    path.numNodes = GraphNumVertices(g);
    path.src = src;
    path.dist = malloc(sizeof(int) * path.numNodes);
    memset(path.dist, -1, sizeof(int) * path.numNodes);
    path.dist[src] = 0;
    path.pred = malloc(sizeof(struct PredNode *) * path.numNodes);
    for (int i = 0; i < path.numNodes; i++)
    {
        path.pred[i] = NULL;
    }

    ////----initialazion vSet----////
    int vSet[path.numNodes];
    memset(vSet, 0, sizeof(vSet));

    while (!vSetAllScaned(vSet, path.numNodes))
    {
        Vertex vFind;
        if (vSet[src] == 0)
        {
            vFind = src;
            vSet[src] = 1;
        } else
        {
            vFind = findMin(vSet, path.dist, path.numNodes);
            if (vFind == -1)
                break;
        }
        // relaxation
        for (AdjList list = GraphOutIncident(g, vFind); list != NULL; list = list->next)
        {
            Vertex w = list->v;
            int weight = list->weight;
            if (path.dist[w] == -1 || path.dist[vFind] + weight < path.dist[w])
            {
                path.dist[w] = weight + path.dist[vFind];
                freePredNode(path.pred[w]);
                path.pred[w] = NULL;
                path.pred[w] = predNodeInsert(path.pred[w], vFind);
            } else if (path.dist[vFind] + weight == path.dist[w])
            {
                path.pred[w] = predNodeInsert(path.pred[w], vFind);
            }
        }
        vSet[vFind] = 1;
    }
    noConnectedDist(vSet, path.dist, path.numNodes);
    return path;
}

void showShortestPaths(ShortestPaths sps)
{
    printf("----showShortestPaths----:\n");

    printf("src: %d\n", sps.src);
    printf("dist:\n");
    for (int i = 0; i < sps.numNodes; i++)
    {
        printf("dist[%d] : %d", i, sps.dist[i]);
    }

    printf("pred:\n");
    for (int i = 0; i < sps.numNodes; i++)
    {
        printf("pred[%d] : ", i);
        for (Pred p = sps.pred[i]; p != NULL; p = p->next)
        {
            printf("%d ", p->v);
        }
    }
}

void freeShortestPaths(ShortestPaths sps)
{
    free(sps.dist);
    for (int i = 0; i < sps.numNodes; i++)
    {
        freePredNode(sps.pred[i]);
    }
    free(sps.pred);

}

//////////////////////////////////////////////////////
// Helper Functions

/**
 * Check if a given vertex is valid for a graph
 */
static bool validVertex(Graph g, Vertex v)
{
    return (v >= 0 && v < GraphNumVertices(g));
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
    } else
    {
        header->next = predNodeInsert(header->next, v);
        return header;
    }
}

/**
 * Free PredNode
 */
static void freePredNode(Pred pred)
{
    if (pred != NULL)
    {
        freePredNode(pred->next);
        free(pred);
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

/*
 *  set no connected nodes' dist
 */
static void noConnectedDist(int *vSet, int *dist, int nV)
{
    for (int i = 0; i < nV; i++)
    {
        if (vSet[i] == 0)
        {
            vSet[i] = 1;
            dist[i] = 0;
        }
    }
}