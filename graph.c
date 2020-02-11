#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <assert.h>
int ** make_node (int node_num)
{
	int ** graph;
	int i, j;
	graph = (int **)malloc(sizeof(int *)* (node_num+1));
	for(i=0; i<node_num+1; i++)
	{
		graph[i] = (int *)malloc(sizeof(int) * (node_num+1));
	}
	for(i=0; i<node_num+1; i++)
	{
		for(j=0; j<node_num+1; j++)
		{
			graph[i][j] = 0;
		}
	}
	return graph;
}

int ** make_graph (char * filename, int * num)
{
	int node_num, node1, node2;
	int ** graph;
	char * trash = (char *)malloc(sizeof(char));
	FILE * fp = fopen(filename, "r");
	fscanf(fp, "%s", trash);
	fscanf(fp, "%d", &node_num);
	fscanf(fp, "%s", trash);

	graph = make_node(node_num);
	
	* num = node_num;

	while(1)
	{
		fscanf(fp, "%d %d", &node1, &node2);
		if(feof(fp)) break;
		
		graph [node1][node2] = 1;
		graph [node2][node1] = 1;
	}
	
	free(trash);
	fclose(fp);
	return graph;

};

void DFSprint(int ** graph, int node_num)
{
	int * check_node = (int *)malloc(sizeof(int)*node_num);
	int * stack = (int *)malloc(sizeof(int)*node_num);
	int stack_pop = -1, out_stack;
	int i;
	
	for(i=0; i<node_num; i++)
	{
		check_node[i] = 0;
		stack[i] = 0;
	}

	while(1)
	{
		if(stack_pop == -1)
		{
			for(i=0; i<node_num; i++)
			{
				if(check_node[i] == 0)
				{
					stack[0] = i + 1;
					stack_pop = 0;
					check_node[i] = 1;
					break;
				}
				if(i == node_num - 1) 
				{
					free(check_node);
					free(stack);
					return;
				}
			}
		}
		else
		{
			out_stack = stack[stack_pop];
			printf("%d ", out_stack);
			stack[stack_pop] = 0;
			stack_pop--;
			
			for(i=1; i<node_num + 1; i++)
			{
				if(graph[out_stack][i] == 1 && check_node[i-1] == 0)
				{
					stack_pop++;
					stack[stack_pop] =i;
					check_node[i-1] = 1;
				}
			}
		}
	}
};	

void BFSprint(int ** graph, int node_num)
{
	int * check_node = (int *)malloc(sizeof(int)*node_num);
	int * queue = (int *)malloc(sizeof(int)*node_num);
	int first=0, last=0, out_queue, i;
	
	for(i=0; i<node_num; i++)
	{
		check_node[i] = 0;
		queue[i] = 0;
	}
	
	while(1)
	{
		if(first == last)
		{
			for(i=0; i<node_num; i++)
			{
				if(check_node[i] == 0)
				{
					queue[first] = i + 1;
					last = first + 1;
					check_node[i] = 1;
					break;
				}
				if(i == node_num - 1) 
				{
					free(check_node);
					free(queue);
					return;
				}
			}
		}
		else
		{
			out_queue = queue[first];
			for(i=1; i<node_num + 1; i++)
			{
				if(graph[out_queue][i] == 1 && check_node[i-1] == 0)
				{
					queue[last] =i;
					last++;
					check_node[i-1] = 1;
				}
			}
			
			printf("%d ", out_queue);
			queue[first] = 0;
			first ++;		
		}
	}
};

void destroy_graph(int ** graph, int node_num)
{
	int i;
	for(i=0; i<=node_num; i++)
	{
		free(graph[i]);
	}
	free(graph);
}

int main(int argc, char **argv)
{
	char * trash;
	int node_num;
	int ** graph;
	int i, j;
	
	if (argc != 2)
	{
		printf("Usage : %s FILE\n", argv[0]);
		return 2;
	}
	
	graph = make_graph (argv[1], &node_num);
	
	//DFS
	printf("DFS : ");
	DFSprint(graph, node_num);
	printf("\n");
	//BFS
	printf("BFS : ");
	BFSprint(graph, node_num);
	printf("\n");
	
	destroy_graph(graph, node_num);

	return 0;

}

