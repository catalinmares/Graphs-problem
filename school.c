/* Mares Catalin-Constantin
			312 CD			*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
	int key;
	struct node* next;
} TNode;

typedef struct
{
	int N;
	TNode** adList;
} TGraph;

TGraph* initGraph(int numberOfNodes)
{
	/* Allocates memory for a graph structure and an adiacency list. */

	TGraph* graph = (TGraph*) calloc(1,sizeof(TGraph));
	graph->N = numberOfNodes;
	graph->adList = (TNode**) calloc(numberOfNodes + 1,sizeof(TNode*));

	return graph;
}

void addEdge(TGraph* graph, int v1, int v2)
{
	/* Allocates memory for a new edge and inserts it
	into the list bidirectionally. */

	TNode* t = (TNode*) calloc(1,sizeof(TNode));
	t->key = v2;
	t->next = graph->adList[v1];
	graph->adList[v1] = t;

	TNode* y =(TNode*) calloc(1,sizeof(TNode));
	y->key = v1;
	y->next = graph->adList[v2];
	graph->adList[v2] = y;
}

TGraph* readGraph(FILE* f, int nrCities, int nrRoads)
{
	int i;
	int v1, v2;

	// Graph initialisation.
	TGraph* graph = initGraph(nrCities);

	// Edge reading and insertion in the list.
	for (i = 1; i <= nrRoads; i++)
	{
		fscanf(f, "%d ", &v1);
		fscanf(f, "%d", &v2);

		addEdge(graph, v1, v2);
	}

	return graph;
}

void visit(TGraph* graph, int* visited, int city)
{
	TNode* t;
	// Set the city to visited.
	visited[city] = 1;

	// Parse the neighbours of the city and visit them recursively.
	for (t = graph->adList[city]; t != NULL; t = t->next)
	{
		// Visit only the cities that are not visited yet.
		if (visited[t->key] == 0)
		{
			visit(graph, visited, t->key);
		}
	}
}

int DFS(TGraph* graph)
{
	/* Parses the graph counting the connected componenents
	and saving in nrIsolatedCities the number of cities in
	each componenent. */
	int i;

	// Allocate N+1 positions for visited because the indexes start at 1.
	int* visited = (int*) calloc(graph->N + 1,sizeof(int));

	int nrCompConexe = 0;

	for (i = 1; i <= graph->N; i++)
	{
		if (visited[i] == 0)
		{
			// Visit each city and its neighbours recursively.
			visit(graph, visited, i);
			nrCompConexe++;
		}
	}

	free(visited);
	visited = NULL;

	return nrCompConexe;
}

int getMinCost(TGraph* graph, int rC, int sC, int nrCities, int nrRoads)
{
	/* Calculates the minimum cost for a given graph. */

	// Get the number of connected components and the number of cities for
	// each component.
	int nrCompConexe = DFS(graph);
	printf("%d\n", nrCompConexe);

	// A school is cheaper than a road.
	if (sC <= rC)
	{	
		// Build N schools, where N is the number of cities.
		return sC * nrCities;
	}


	// A road is cheaper than a school.

	// Build a school in each connected component and m-1 roads for each
	// connected components, where m is the number of cities in the given
	// component.
	int minCost = nrCompConexe * sC + rC * (nrCities - nrCompConexe);

	return minCost;
}

void freeGraph(TGraph* graph)
{
	int i;

	// For each node, free its adiacency list.
	for (i = 0; i <= graph->N; i++)
	{
		TNode* t = graph->adList[i];

		while (t != NULL)
		{
			TNode* current = t;

			t = t->next;
			free(current);
			current = NULL;
		}
	}

	// free the adiacency list and the graph.
	free(graph->adList);
	free(graph);
	graph = NULL;
}

int main()
{
	FILE* fin = fopen("school.in", "r");

	if (fin == NULL) return -1;
	FILE* fout = fopen("school.out", "w");
	
	TGraph* graph;
	int i, k;
	int minCost;
	int nrCities, nrRoads;
	int roadCost, schoolCost;
	
	// Read the number of entries.
	fscanf(fin, "%d\n", &k);

	for (i = 1; i <= k; i++)
	{
		// Read graph components and costs.
		fscanf(fin, "%d ", &nrCities);
		fscanf(fin, "%d ", &nrRoads);
		fscanf(fin, "%d ", &roadCost);
		fscanf(fin, "%d\n", &schoolCost);
		
		// Read edges and build graph.
		graph = readGraph(fin, nrCities, nrRoads);

		// Calculate the minimum cost.
		minCost = getMinCost(graph, roadCost, schoolCost, nrCities, nrRoads);

		// Print the cost in the given file.
		fprintf(fout, "%d\n", minCost);

		freeGraph(graph);
	}

	fclose(fin);
	fclose(fout);
	return 0;
}