/********************************************************************************************** 
** Project Name: Task_SPT 
** Program Name: My_Dijkstry.h 
** 
** Purpose:  Dijksra's alhorithm - for search min pathes from the source st_node to each other vertex 
** 
** Dependencies: STL library 
** 
** Author:  Alesya Samylova 
** 
** Date: 21 JUL 2016 
** Update Date: 22 JUL 2016 
** 
***********************************************************************************************/ 
#include <iostream> 

// Dijksra's alhorithm - for search min pathes from the source st_node to each other node 
// we can use size_t type instead of unsigned int, as it is alias for it 

void Dijkstra (int **WeightMatrix, size_t num_nodes, size_t st_node, int *distances) 
{ 

    bool *visited = new bool[num_nodes]; 
    unsigned int count, i, u; 

    //set each node/vertex to unvisited with INT_MAX distance(by INT_MAX I represent infinitive) 
    for (i=0; i < num_nodes; i++) 
    { 
        distances[i] = INT_MAX; 
        visited[i] = false; 
    } 

    // set distance of source to vertex from itself (is always zero) 
     distances[st_node]=0; 

    // find shortest path for all vertices 
    for (count=0; count < num_nodes; count++) 
	{ 
        int min = INT_MAX; 
        size_t index_min; 

        //pick the minimum distance vertex from the set of vertices not yet processed 
        for (i = 0; i < num_nodes; i++) 
			if (!visited[i] && distances[i] <= min) 
            { 
		        min = distances[i]; 
                index_min = i; 
            } 

        //NOTE: in first iteration u is always equal to source 
        u = index_min; 
        // mark the picked vertix as processed 
        visited[u] = true; 

        // update distance value of the vertices which are adjacent of the picked vertex 
        for(i=0; i < num_nodes; i++) 
        {    
            //update distances[i] only if i is not visited, there is an edge from u to i 
            // and total weight of path from source to v through u is smaller than current value 

           if(!visited[i] && WeightMatrix[u][i] != 0 && distances[u] != INT_MAX  
               && distances[u] + WeightMatrix[u][i] < distances[i]) 
			   distances[i] = distances[u] + WeightMatrix[u][i]; 

        } 
    } 
} 

void printSolution (int *distances, size_t num_nodes, size_t src_node) 
{ 
    std::cout << "Min distances from [" << src_node << "] vertex to:\n"; 
    for(size_t i = 0; i < num_nodes; i++) 
    { 
        if (distances[i] != INT_MAX) 
			std::cout<<"Vertex ["<<i<<"]: "<<distances[i]<<"\n"; 
	} 
} 

 
