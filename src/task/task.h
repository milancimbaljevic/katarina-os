#pragma once

#include "config.h"
#include "paging/paging.h"
#include "idt/pic.h"
#include "print.h"

struct interrupt_frame;
struct Registers
{
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    uint32_t ip;
    uint32_t cs;
    uint32_t flags;
    uint32_t esp;
    uint32_t ss;
};

class Process
{
public:
    Process() {}
};

class Task
{
    /**
     * The page directory of the task
     */
    PageMapTable *pmt;

    // The registers of the task when the task is not running
    Registers registers;

    // The process of the task
    Process *process;

    // The next task in the linked list
    Task *next = nullptr;

    // Previous task in the linked list
    Task *prev = nullptr;

    Task(Process *p);

    static Task *CurrentTask;
    static Task *TaskHead;
    static Task *TaskTail;

public:
    static Task *GetCurrentTask() { return CurrentTask; }
    static Task *CreateNewTask(Process *process);
    static Task* GetNextTask();
    static void RemoveTask(Task*);
    static void SwitchTask(Task*);
    void SaveState(interrupt_frame*);

    void *operator new(size_t size, void *addr)
    {
        return addr;
    }

    ~Task();
};
