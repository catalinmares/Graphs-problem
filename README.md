# Graphs problem

The program I wrote treats 2 separated cases for the given problem:
Given a graph with N cities and M roads and the cost of a school and a road,
there are 2 cases:
1. The school is cheaper than the road ---> In this case, I build a school in
each city, so there will be N schools.
2. The road is cheaper than the school ---> In this case, I must parse the graph
using the DFS algorith, counting the number of connected components. For this
case, I found that the minimum cost is given by building a school in each
component and N-1 roads in each component, where N is the number of cities in the
component. So, according to the mathematical induction, if a connected component
needs N-1 roads to be connected, then nrComp need N-nrComp roads to be connected.
This way, the minimum cost is:

	minCost = nrComp * sCost + rCost * (nrCities - nrCompConexe)
