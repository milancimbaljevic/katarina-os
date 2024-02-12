#include "task.h"
#include "paging/paging.h"
#include "config.h"
#include "memory/memory.h"

Task *Task::CurrentTask = nullptr;
Task *Task::TaskHead = nullptr;
Task *Task::TaskTail = nullptr;

Task::Task(Process *process)
{
    pmt = new OneToOnePMT();                               // let it be 4gb one to one for now
    pmt->init(PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL); // no error handling :)

    registers.ip = IP_STARTING_VIRTUAL_ADDRESS;

    registers.ss = USER_DATA_SEGMENT;
    registers.cs = USER_CODE_SEGMENT;
    registers.esp = VIRTUAL_STACK_ADDRESS_START;

    this->process = process;
}

Task *Task::CreateNewTask(Process *process)
{
    void *task = malloc(sizeof(Task));

    if (!task)
        return nullptr;

    new (task) Task(process);

    if (!TaskHead)
    {
        TaskHead = (Task *)task;
        TaskTail = (Task *)task;
        CurrentTask = (Task *)task;
        return (Task *)task;
    }

    Task *tsk = (Task *)task;

    TaskTail->next = tsk;
    tsk->prev = TaskTail;
    TaskTail = tsk;

    return tsk;
}

Task *Task::GetNextTask()
{
    if (!CurrentTask->next)
        return TaskHead;
    return CurrentTask->next;
}

void Task::RemoveTask(Task *task)
{
    if (task->prev)
    {
        task->prev->next = task->next;
    }

    if (task == TaskHead)
    {
        TaskHead = task->next;
    }

    if (task == TaskTail)
    {
        TaskTail = task->prev;
    }

    if (task == CurrentTask)
    {
        CurrentTask = GetNextTask();
    }
}

void Task::SwitchTask(Task *task)
{
    CurrentTask = task;
    task->pmt->load_me_into_pmtr();
}

void Task::SaveState(struct interrupt_frame *frame)
{
    registers.ip = frame->ip;
    registers.cs = frame->cs;
    registers.flags = frame->flags;
    registers.esp = frame->esp;
    registers.ss = frame->ss;
    registers.eax = frame->eax;
    registers.ebp = frame->ebp;
    registers.ebx = frame->ebx;
    registers.ecx = frame->ecx;
    registers.edi = frame->edi;
    registers.edx = frame->edx;
    registers.esi = frame->esi;
}


Task::~Task()
{
    pmt = nullptr;
    RemoveTask(this);
}
