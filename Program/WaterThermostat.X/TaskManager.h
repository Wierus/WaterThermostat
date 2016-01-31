/** TaskManager.h
 * v.1.1
 */

#ifndef TASKMANAGER_H
#define	TASKMANAGER_H

#include <xc.h>
#include "Config.h"

#ifndef TaskManagerQueueSize
    #error "TaskManagerQueueSize symbol is not defined, you should define it in file Config.h before using this library"
#endif

#ifndef TaskManagerTimerType
    #error "TaskManagerTimerType symbol is not defined, you should define it in file Config.h before using this library"
#endif

typedef void (*TPtr)(void);

typedef TaskManagerTimerType TTimer;

TPtr TasksQueue[TaskManagerQueueSize];

TTimer TimersQueue[TaskManagerQueueSize];

void Idle();

void TaskManager();

void TimerService();

void AddTask(TPtr pTask, TTimer time);

void InitRTOS();

void RunRTOS();

#endif /* TASKMANAGER_H */
