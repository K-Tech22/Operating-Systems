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
// Schedules based on Round Robin
void schedule()
{
    struct node *gsorted_head = sortList(g_head);
    int num_tasks = count(&gsorted_head);
    int i;
    Task **tasks = malloc(sizeof(Task *) * num_tasks);

    struct node *curr = gsorted_head;

    for (i = 0; i < num_tasks; i++)
    {
        tasks[i] = curr->task;
        curr = curr->next;
    }

    int time = 0;
    bool done = false;
    while (!done)
    {
        done = true;
        for (i = 0; i < num_tasks; i++)
        {
            if (tasks[i]->burst > 0)
            {
                done = false;
                if (tasks[i]->burst <= quantum)
                {
                    int running_time = tasks[i]->burst;
                    run(tasks[i], running_time);
                    time += running_time;
                    printf("\tTime is now: %d\n", time);
                    tasks[i]->burst = 0;
                }
                else
                {
                    run(tasks[i], quantum);
                    time += quantum;
                    printf("\tTime is now: %d\n", time);
                    tasks[i]->burst -= quantum;
                }
            }
        }
    }
    free(tasks);
}