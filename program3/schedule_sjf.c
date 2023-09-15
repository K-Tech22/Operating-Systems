#include <string.h>
#include "list.h"
#include "cpu.h"
#include "task.h"
#include "schedulers.h"
#include <stdbool.h>

struct node *g_head = NULL;

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

    int lowestburst = 100;

    while (temp != NULL)
    {
        if (temp->task->burst < lowestburst)
        {
            best_sofar = temp->task;
            lowestburst = temp->task->burst;
        }
        // if (comesBefore(temp->task->name, best_sofar->name))
        //     best_sofar = temp->task;
        temp = temp->next;
    }
    // delete the node from list, Task will get deleted later
    delete (&g_head, best_sofar);
    return best_sofar;
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