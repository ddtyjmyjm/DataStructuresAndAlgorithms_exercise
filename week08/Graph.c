// Graph.c ... implementation of Graph ADT
// Written by John Shepherd, May 2013

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Graph.h"
#include "Queue.h"

//helper function
int *makeList(int length);

// graph representation (adjacency matrix)
typedef struct GraphRep
{
	int nV;		 // #vertices
	int nE;		 // #edges
	int **edges; // matrix of weights (0 == no edge)
} GraphRep;

// check validity of Vertex
int validV(Graph g, Vertex v)
{
	return (g != NULL && v >= 0 && v < g->nV);
}

// make an edge
static Edge mkEdge(Graph g, Vertex v, Vertex w)
{
	assert(g != NULL && validV(g, v) && validV(g, w));
	Edge new = {v, w}; // struct assignment
	return new;
}

// insert an Edge
// - sets (v,w) and (w,v)
void insertEdge(Graph g, Vertex v, Vertex w, int wt)
{
	assert(g != NULL && validV(g, v) && validV(g, w));

	if (g->edges[v][w] != 0 && g->edges[w][v] != 0)
		// an edge already exists; do nothing.
		return;

	g->edges[v][w] = wt;
	g->edges[w][v] = wt;
	g->nE++;
}

// remove an Edge
// - unsets (v,w) and (w,v)
void removeEdge(Graph g, Vertex v, Vertex w)
{
	assert(g != NULL && validV(g, v) && validV(g, w));
	if (g->edges[v][w] == 0 && g->edges[w][v] == 0)
		// an edge doesn't exist; do nothing.
		return;

	g->edges[v][w] = 0;
	g->edges[w][v] = 0;
	g->nE--;
}

// create an empty graph
Graph newGraph(int nV)
{
	assert(nV > 0);

	GraphRep *new = malloc(sizeof *new);
	assert(new != NULL);
	*new = (GraphRep){.nV = nV, .nE = 0};

	new->edges = calloc((size_t)nV, sizeof(int *));
	assert(new->edges != NULL);
	for (int v = 0; v < nV; v++)
	{
		new->edges[v] = calloc((size_t)nV, sizeof(int));
		assert(new->edges[v] != 0);
	}

	return new;
}

// free memory associated with graph
void dropGraph(Graph g)
{
	assert(g != NULL);
	for (int v = 0; v < g->nV; v++)
		free(g->edges[v]);
	free(g->edges);
	free(g);
}

// display graph, using names for vertices
void showGraph(Graph g, char **names)
{
	assert(g != NULL);
	printf("#vertices=%d, #edges=%d\n\n", g->nV, g->nE);
	int v, w;
	for (v = 0; v < g->nV; v++)
	{
		printf("%d %s\n", v, names[v]);
		for (w = 0; w < g->nV; w++)
		{
			if (g->edges[v][w])
			{
				printf("\t%s (%d)\n", names[w], g->edges[v][w]);
			}
		}
		printf("\n");
	}
}

//helper function
int *makeList(int length)
{
	int *list = (int *)malloc(sizeof(int) * length);
	for (int i = 0; i < length; i++)
	{
		list[i] = -1;
	}
	return list;
}

// find a path between two vertices using breadth-first traversal
// only allow edges whose weight is less than "max"
int findPath(Graph g, Vertex src, Vertex dest, int max, int *path)
{
	assert(g != NULL);

	// the same vertex
	if(src == dest){
		path[0] = dest;
		return 1;
	}

	int *visited = makeList(g->nV);
	int *revPath = makeList(g->nV);
	int found = 0;
	Queue queue = newQueue();
	visited[src] = src;
	QueueJoin(queue, src);

	

	// find the path
	while (!found && !QueueIsEmpty(queue))
	{
		int v = QueueLeave(queue);
		if (v == dest)
			found = 1;
		else
		{
			// beacuse every vertex is connected to each other, it searches every vertexs
			// w is the Vertex which is not visited and (v,w) less than "max"
			for (int w = 0; w < g->nV; w++)
			{
				if (visited[w] == -1 && g->edges[v][w] <= max)
				{
					visited[w] = v;
					QueueJoin(queue, w);
				}
			}
		}
	}
	dropQueue(queue);

	int length = 0;
	if (found)
	{
		// create a reverse list of finding path
		int point = dest;

		for (int i = 0; i < g->nV; i++)
		{
			revPath[i] = point;
			point = visited[point];
			if (point == src)
			{
				revPath[i + 1] = point;
				length = i + 2;
				break;
			}
		}

		// create the list pf path
		for (int i = 0; i < length; i++)
		{
			path[i] = revPath[length - i - 1];
		}
	}
	free(visited);
	free(revPath);
	return length;
	
}
