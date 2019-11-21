// Dijkstra API implement
// author: Jingming Yang ( //-1sizesizeof()distsizesizeof()vSet////

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Graph.h"
#include "Dijkstra.h"
#include "PQ.h"

typedef struct PredNode *Pred;

static bool validVertex(Graph g, Vertex v);

static Pred newPredNode(Vertex v);

static Pred predNodeInsert(Pred header, Vertex v);

static void freePredNode(Pred pred);

//////////////////////////////////////////////////////

ShortestPaths dijkstra(Graph g, Vertex src)
{
    assert(g != NULL);
    assert(validVertex(g, src));
    // initialazion ShortestPaths
    ShortestPaths path;
    path.numNodes = GraphNumVertices(g);
    path.src = src;
    path.dist = malloc(sizeof(int) * path.numNodes);
    memset(path.dist, 0, sizeof(int) * path.numNodes);
    path.pred = malloc(sizeof(struct PredNode *) * path.numNodes);
    for (int i = 0; i < path.numNodes; i++)
    {
        path.pred[i] = NULL;
    }

    PQ pq = PQNew();
    ItemPQ item = {src, 0};
    PQAdd(pq, item);
    while (!PQIsEmpty(pq))
    {
        Vertex vFind = PQDequeue(pq).key;

        // relaxation
        for (AdjList list = GraphOutIncident(g, vFind); list != NULL; list = list->next)
        {

            Vertex w = list->v;
            int weight = list->weight;

            // w found for the first time?
            if (w != src && path.dist[w] == 0)
            {
                path.dist[w] = weight + path.dist[vFind];
                path.pred[w] = predNodeInsert(path.pred[w], vFind);

                // first in
                item.key = w;
                item.value = path.dist[w];
                PQAdd(pq, item);
            }else if (path.dist[vFind] + weight < path.dist[w])
            {
                path.dist[w] = weight + path.dist[vFind];
                freePredNode(path.pred[w]);
                path.pred[w] = NULL;
                path.pred[w] = predNodeInsert(path.pred[w], vFind);
                // update
                item.key = w;
                item.value = path.dist[w];
                PQUpdate(pq, item);
            } else if (path.dist[vFind] + weight == path.dist[w])
            {
                path.pred[w] = predNodeInsert(path.pred[w], vFind);

            }
        }
    }
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

