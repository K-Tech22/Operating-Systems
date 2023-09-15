#include <string.h>
#include "list.h"
#include "cpu.h"
#include "task.h"
#include "schedulers.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct node *g_head = NULL;

bool comesBefore(char *a, char *b) { return strcmp(a, b) < 0; }

Task *checkForSamePriority(Task *best_sofar)
{
    struct node *curr = g_head;

    int priority = best_sofar->priority;

    Task *inordertask = best_sofar;

    while (curr != NULL)
    {
        if (curr->task->priority == priority)
        {
            if (comesBefore(curr->task->name, inordertask->name))
            {
                inordertask = curr->task;
            }
        }
        curr = curr->next;
    }
    return inordertask;
}

// based on traverse from list.c
// finds the task whose name comes first in dictionary
Task *pickNextTask()
{
    // if list is empty, nothing to do
    if (!g_head)
        return NULL;

    struct node *temp;
    temp = g_head;
    Task *best_sofar = temp->task;

    int priority = 1;

    while (temp != NULL)
    {
        if (temp->task->priority > priority)
        {
            best_sofar = temp->task;
            priority = temp->task->priority;
        }
        temp = temp->next;
    }
    Task *best = checkForSamePriority(best_sofar);
    // delete the node from list, Task will get deleted later
    delete (&g_head, best);
    return best;
}

void add(char *name, int priority, int burst)
{
    Task *task = (Task *)malloc(sizeof(Task));
    task->name = name;
    task->burst = burst;
    task->priority = priority;
    insert(&g_head, task);
}

void schedule()
{
    struct node *curr = g_head;
    int time = 0;

    while (curr != NULL)
    {
        Task *task = pickNextTask();
        run(task, task->burst);
        time += task->burst;
        printf("\tTime is now: %d\n", time);
        curr = curr->next;

        if (curr == NULL)
        {
            if (g_head != NULL)
                curr = g_head;
        }
    }
}