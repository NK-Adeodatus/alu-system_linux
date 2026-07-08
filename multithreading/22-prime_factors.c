#include "multithreading.h"
#include <stdlib.h>
#include <pthread.h>

/**
 * create_task - creates a task structure
 * @entry: pointer to the entry function of the task
 * @param: parameter to be passed to the entry function
 *
 * Return: pointer to the created task structure, or NULL on failure
 */
task_t *create_task(task_entry_t entry, void *param)
{
	task_t *task;

	task = calloc(1, sizeof(*task));
	if (!task)
		return (NULL);

	task->entry = entry;
	task->param = param;
	task->status = PENDING;
	task->result = NULL;
	pthread_mutex_init(&task->lock, NULL);

	return (task);
}

/**
 * destroy_task - destroys a task structure
 * @task: pointer to the task to destroy
 */
void destroy_task(task_t *task)
{
	if (task)
	{
		pthread_mutex_destroy(&task->lock);
		if (task->result)
		{
			list_destroy((list_t *)task->result, free);
			free(task->result);
		}
		free(task);
	}
}

/**
 * exec_tasks - Thread entry to execute a list of tasks
 * @tasks: pointer to the list of tasks
 *
 * Return: NULL
 */
void *exec_tasks(list_t const *tasks)
{
	node_t *node;
	task_t *task;
	int task_id = 0;

	if (!tasks)
		return (NULL);

	for (node = tasks->head; node; node = node->next, task_id++)
	{
		task = (task_t *)node->content;
		if (task->status == PENDING)
		{
			pthread_mutex_lock(&task->lock);
			if (task->status == PENDING)
			{
				task->status = STARTED;
				tprintf("[%02d] Started\n", task_id);
				pthread_mutex_unlock(&task->lock);

				task->result = task->entry(task->param);

				pthread_mutex_lock(&task->lock);
				task->status = task->result ? SUCCESS : FAILURE;
				tprintf("[%02d] %s\n", task_id,
					task->result ? "Success" : "Failure");
			}
			pthread_mutex_unlock(&task->lock);
		}
	}
	return (NULL);
}
