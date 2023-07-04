// SPDX-License-Identifier: UNLICENSED
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

#include "os_graph.h"
#include "os_threadpool.h"
#include "os_list.h"

#define MAX_TASK 100
#define MAX_THREAD 4

int sum;
os_graph_t *graph;

os_threadpool_t *tp;
pthread_mutex_t lock;

void processNode(unsigned int nodeIdx)
{
	os_node_t *node = graph->nodes[nodeIdx];

	for (int i = 0; i < node->cNeighbours; i++) {

		//if neighbour is not visited, create task for it and add to threadpool
		if (graph->visited[node->neighbours[i]] == 0) {
			os_task_t *task = malloc(sizeof(os_task_t));

			task->argument = graph->nodes[nodeIdx]->neighbours[i];
			task->task = processNode;

			add_task_in_queue(tp, task);
		}
	}

	//if node is not visited, mark it as visited and add its value to sum
	pthread_mutex_lock(&lock);

	if (graph->visited[nodeIdx] == 0) {
		graph->visited[nodeIdx] = 1;
		sum += node->nodeInfo;
	}
	pthread_mutex_unlock(&lock);
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("Usage: %s <input_file>\n", argv[0]);
		exit(1);
	}

	FILE *input_file = fopen(argv[1], "r");

	if (input_file == NULL) {
		printf("[Error] Can't open file\n");
		return -1;
	}

	graph = create_graph_from_file(input_file);
	if (graph == NULL) {
		printf("[Error] Can't read the graph from file\n");
		return -1;
	}

	// TODO: create thread pool and traverse the graph


	//create threadpool
	tp = threadpool_create(MAX_TASK, MAX_THREAD);
	pthread_mutex_init(&lock, NULL);

	if (tp == NULL) {
		printf("[Error] Can't create threadpool\n");
		return -1;
	}

	//create task for each node in graph and add to threadpool
	for (int i = 0; i < graph->nCount; i++) {
		if (graph->visited[i] == 0) {
			os_task_t *task = malloc(sizeof(os_task_t));

			task->argument = i;
			task->task = processNode;

			add_task_in_queue(tp, task);
		}
	}

	threadpool_stop(tp, NULL);
	pthread_mutex_destroy(&lock);
	printf("%d", sum);

	return 0;
}
