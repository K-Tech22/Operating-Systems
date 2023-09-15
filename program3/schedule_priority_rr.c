#include <string.h>
#include "list.h"
#include "cpu.h"
#include "task.h"
#include "schedulers.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define quantum 10

struct node *g_head = NULL;

bool comesBefore(char *a, char *b) { return strcmp(a, b) < 0; }

struct node *sortList(struct node *head)
{
    struct node *result = NULL;

    while (head != NULL)
    {
        struct node *current = head;
        head = head->next;

        if (result == NULL || comesBefore(current->task->name, result->task->name))
        {
            current->next = result;
            result = current;
        }
        else
        {
            struct node *temp = result;
            while (temp->next != NULL && comesBefore(temp->next->task->name, current->task->name))
            {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
    }

    return result;
}

Task *checkForSamePriority(Task *best_sofar)
{
    struct node *curr = sortList(g_head);

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

    return best;
}

void add(char *name, int priority, int burst)
{

    Task *task = (Task *)malloc(sizeof(Task));
    task->name = name;
    task->burst = burst;
    task->priority = priority;
    task->timeleft = burst;
    insert(&g_head, task);
}

int count(struct node **head_ref)
{
    int count = 0;
    struct node *current = *head_ref;

    while (current != NULL)
    {
        count++;
        current = current->next;
    }
    return count;
}

void schedule()
{
    struct node *gsorted_head = sortList(g_head);
    int num_tasks = count(&gsorted_head);
    int i = 0;
    int time = 0;
    bool done = false;
    while (!done)
    {
        done = true;
        struct node *curr = gsorted_head;
        while (curr != NULL)
        {
            Task *task = curr->task;
            if (task->burst > 0)
            {
                done = false;
                if (task->burst <= quantum)
                {
                    int running_time = task->burst;
                    run(task, running_time);
                    time += running_time;
                    printf("\tTime is now: %d\n", time);
                    task->burst = 0;
                    delete (&gsorted_head, task);
                    num_tasks = count(&gsorted_head);
                    i++;
                }
                else
                {
                    run(task, quantum);
                    time += quantum;
                    printf("\tTime is now: %d\n", time);
                    task->burst -= quantum;
                    i++;
                }
            }
            if (i == num_tasks)
            {
                i = 0;
                curr = gsorted_head;
            }
            else
            {
                curr = curr->next;
            }
        }
        gsorted_head = sortList(g_head);
    }
}