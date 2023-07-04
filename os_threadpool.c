// SPDX-License-Identifier: UNLICENSED
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "os_threadpool.h"


/* === TASK === */

// function that creates a queue for tasks
os_task_queue_t *task_queue_create(void)
{
	os_task_queue_t *newTaskQueue = malloc(sizeof(os_task_queue_t));

	if (newTaskQueue == NULL) {
		printf("[Error] Cannot create queue\n");
		return NULL;
	}

	newTaskQueue->task = NULL;
	newTaskQueue->next = NULL;

	return newTaskQueue;
}

// function that adds a task to the queue
os_task_queue_t *task_queue_add(os_task_queue_t *taskQueue, os_task_t *task)
{
	os_task_queue_t *newTaskQueue = task_queue_create();

	if (newTaskQueue == NULL) {
		printf("[Error] Cannot add task to queue\n");
		return NULL;
	}

	newTaskQueue->task = task;
	newTaskQueue->next = NULL;

	if (taskQueue == NULL) {
		taskQueue = newTaskQueue;
	} else {

		// add task to the beginning of the queue
		os_task_queue_t *currentTaskQueue = taskQueue;

		newTaskQueue->next = currentTaskQueue;
		taskQueue = newTaskQueue;
	}

	return taskQueue;
}

/* Creates a task that thread must execute */
os_task_t *task_create(void *arg, void (*f)(void *))
{
	// TODO
	os_task_t *task = malloc(sizeof(os_task_t));

	if (task == NULL) {
		printf("[Error] Cannot create task\n");
		return NULL;
	}

	task->argument = arg;
	task->task = f;

	return task;
}

/* Add a new task to threadpool task queue */
void add_task_in_queue(os_threadpool_t *tp, os_task_t *t)
{
	// TODO
	pthread_mutex_lock(&tp->taskLock);
	tp->tasks = task_queue_add(tp->tasks, t);
	pthread_mutex_unlock(&tp->taskLock);
}

/* Get the head of task queue from threadpool */
os_task_t *get_task(os_threadpool_t *tp)
{
	// TODO
	os_task_t *task = NULL;
	os_task_queue_t *taskQueue;

	pthread_mutex_lock(&tp->taskLock);
	taskQueue = tp->tasks;

	if (taskQueue != NULL) {
		task = taskQueue->task;
		tp->tasks = taskQueue->next;
	}

	pthread_mutex_unlock(&tp->taskLock);

	return task;
}

/* === THREAD POOL === */

/* Initialize the new threadpool */
os_threadpool_t *threadpool_create(unsigned int nTasks, unsigned int nThreads)
{
	// TODO
	os_threadpool_t *tp = malloc(sizeof(os_threadpool_t));

	pthread_mutex_init(&tp->taskLock, NULL);
	if (tp == NULL) {
		printf("[Error] Cannot create threadpool\n");
		return NULL;
	}

	tp->tasks = NULL;
	tp->num_threads = nThreads;
	tp->should_stop = 0;
	tp->threads = malloc(sizeof(pthread_t) * nThreads);

	// create threads
	for (int i = 0; i < nThreads; i++)
		pthread_create(&tp->threads[i], NULL, thread_loop_function, tp);

	return tp;
}

/* Loop function for threads */
void *thread_loop_function(void *args)
{
	// TODO
	os_threadpool_t *tp = (os_threadpool_t *)args;
	os_task_t *task;

	// loop until there are no more tasks
	while (1) {
		task = get_task(tp);
		if (task != NULL) {
			task->task(task->argument);
			free(task);
		} else {
			if (tp->should_stop == 1)
				break;
		}
	}

	return NULL;
}

/* Stop the thread pool once a condition is met */
void threadpool_stop(os_threadpool_t *tp, int (*processingIsDone)(os_threadpool_t *))
{
	// TODO
	tp->should_stop = 1;

	for (int i = 0; i < tp->num_threads; i++)
		pthread_join(tp->threads[i], NULL);

	free(tp->threads);
	free(tp);
}
