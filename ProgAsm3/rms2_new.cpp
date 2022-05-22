#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <stack>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdio>
#include <climits>
using namespace std;

// Class for process
class TaskProcess
{
public:
    // Elements of the class include ID, time taken to process, Time period, Arrival time, Deadline
    int task_id, task_time, task_period, task_deadline, task_arrivaltime, task_ideal_finish_time;
    double task_priority;
    // Boolean Variable to denote if the process was interrupted or not
    bool isInterrupted;
    // Constructor Function
    TaskProcess(int task_id, int task_time, int task_period, int task_arrivaltime) : task_id(task_id), task_time(task_time), task_period(task_period),
                                                                                     task_deadline(task_arrivaltime + task_period), task_arrivaltime(task_arrivaltime), task_ideal_finish_time(task_arrivaltime + task_time),
                                                                                     task_priority(1.0 / task_period),
                                                                                     isInterrupted(false) {}
};

class ListProcess : public vector<TaskProcess>
{
    public:
    void SortList()
    {
        sort(this->begin(), this->end(), [](TaskProcess a, TaskProcess b) { return a.task_arrivaltime > b.task_arrivaltime; });
    }
};

int DeadlineMiss(ofstream &output_file, int &numDeadline, int &current_time, TaskProcess &task)
{
    numDeadline++;
    output_file << "Task ID: " << task.task_id << " missed deadline at time: " << current_time << endl;
    return current_time - task.task_arrivaltime - task.task_time;
}

// Function to find the highest priority process in the Ready Queue which is a vector of all Tasks
// Same as finding the least time period process in the Ready Queue
TaskProcess findHighestPriority(vector<TaskProcess> &ReadyQueue)
{
    float highestPriority = 0.0;
    int highestPriorityIndex = -1;
    for(int i = 0; i < ReadyQueue.size(); i++)
    {
        if(ReadyQueue[i].task_priority > highestPriority)
        {
            highestPriority = ReadyQueue[i].task_priority;
            highestPriorityIndex = i;
        }
    }
    TaskProcess returnProcess(ReadyQueue[highestPriorityIndex].task_id, ReadyQueue[highestPriorityIndex].task_time, ReadyQueue[highestPriorityIndex].task_period, ReadyQueue[highestPriorityIndex].task_arrivaltime);
    returnProcess.isInterrupted = ReadyQueue[highestPriorityIndex].isInterrupted;
    // Remove this task from the Ready Queue
    ReadyQueue.erase(ReadyQueue.begin() + highestPriorityIndex);
    // Return the task with the minimum time period which is the highest priority task
    return returnProcess;
}

int main()
{
    ifstream input_file;
    ofstream output_file_log;

    input_file.open("inp_params.txt");
    if (!input_file.is_open())
    {
        cout << "Error in opening the input file" << endl;
        return EXIT_FAILURE;
    }
    cout << "Input File Opened Successfully" << endl;
    output_file_log.open("RMS_log.txt");

    // The number of types of processes given in the input file
    int numProcesses;
    input_file >> numProcesses;
    // cout << numProcesses << endl;
    vector<TaskProcess> ReadyQueue;

    // Lambda expression to sort the vector of processes based on the arrival time
    auto cmp = [](TaskProcess a, TaskProcess b)
    {
        // if (a.task_arrivaltime == b.task_arrivaltime && a.task_period < b.task_period)
        // {
        //     return a.task_id > b.task_id;
        // }
        return a.task_arrivaltime > b.task_arrivaltime;
    };

    // Using priority queue for the list of all processes so that it is self sorting based on the given sorting lambda expression
    priority_queue<TaskProcess, vector<TaskProcess>, decltype(cmp)> ListTaskProcesses(cmp);
    int num_task = 0;

    // Adding all the processes to the priority queue
    for (int i = 0; i < numProcesses; i++)
    {
        int task_id, task_time, task_period, task_repeat;
        input_file >> task_id >> task_time >> task_period >> task_repeat;
        num_task += task_repeat;
        for (int j = 0; j < task_repeat; j++)
        {
            TaskProcess newTask(task_id, task_time, task_period, task_period * j);
            ListTaskProcesses.push(newTask);
        }
        output_file_log << "Task ID:" << task_id << " Process Time: " << task_time << " Process Period: " << task_period << " Number of Repeats: " << task_repeat << endl;
    }

    // Initialzing the variables of Previously Running Process to extreme values to avoid conflict with normal processes
    TaskProcess PrevRunningProcess(INT_MIN, 0, INT_MAX, 0);
    // Boolean to check if the CPU is idle or not
    bool IsIdle = false;

    // Variables to keep track of the current time, total waiting time for all the processes, the arrival time for next process and number of deadlines missed
    int current_time = 0, total_waiting_time = 0, next_arrival_time = 0, num_deadline_miss = 0;
    // int count = 0;
    while (ListTaskProcesses.empty() == false)
    {
        // cout << count << endl;
        // count++;

        // Take the current time as the topmost element of the priority queue
        current_time = ListTaskProcesses.top().task_arrivaltime;

        /******************************************
         * Checking for all missed deadline processeses first in the program
         */
        vector<TaskProcess>::iterator it = ReadyQueue.begin();
        vector<TaskProcess>::iterator missed_it;
        // Checking to see if any of the processes in the Ready Queue have missed a deadline or not
        while (it != ReadyQueue.end())
        {
            // If the deadline time of the process is before the current time, then it missed a deadline
            if (it->task_deadline <= current_time)
            {
                total_waiting_time += DeadlineMiss(output_file_log, num_deadline_miss, current_time, *it);
                // If the process missed the deadline, then it is removed from the Ready Queue and terminated
                missed_it = it;
                ReadyQueue.erase(missed_it);
            }
            if (it == ReadyQueue.end())
            {
                break;
            }
            it++;
        }

        // Checking if the previous process has missed the deadline or not
        if (PrevRunningProcess.task_deadline <= current_time && PrevRunningProcess.task_id > 0)
        {
            total_waiting_time += DeadlineMiss(output_file_log, num_deadline_miss, current_time, PrevRunningProcess);
            // Terminating the Previous Process by setting to extreme values
            PrevRunningProcess = TaskProcess(INT_MIN, 0, INT_MAX, 0);
        }

        /***********************
         * Completed checking for missed deadline processes
         */

        // Adding all the processes to the Ready Queue which have arrived at the current time
        while (ListTaskProcesses.empty() == false && ListTaskProcesses.top().task_arrivaltime == current_time)
        {
            if (ListTaskProcesses.top().task_arrivaltime == current_time)
            {
                ReadyQueue.push_back(ListTaskProcesses.top());
                ListTaskProcesses.pop();
            }
            else
            {
                break;
            }
        }

        // Initializing the arrival time as extreme value if all processes have completed execution
        next_arrival_time = INT_MAX;
        // If list of processes is not empty then initilize the next arrival time as the arrival time of the next process
        if (ListTaskProcesses.empty() == false)
        {
            next_arrival_time = ListTaskProcesses.top().task_arrivaltime;
        }

        // Initialzing the variables of Running Process to extreme values to avoid conflict with normal processes
        TaskProcess runningProcess(INT_MIN, 0, INT_MAX, 0);

        /****************
         * Running Processes from the Ready Queue
         */
        // Checking if the Ready Queue is empty or not or if the Previously Running Process is not terminated
        if (ReadyQueue.empty() == false || PrevRunningProcess.task_id > 0)
        {
            // If Ready Queue is not empty then find the highest Priority Task from it and that becomes the current running process
            if (ReadyQueue.empty() == false)
            {
                runningProcess = findHighestPriority(ReadyQueue);
            }
            // If ready queue is empty then the previously running process is the current running process
            else
            {
                runningProcess = PrevRunningProcess;
            }

            // If uptill now the CPU was Idle then it is make to false
            if (IsIdle == true)
            {
                IsIdle = false;
                output_file_log << "CPU Idle till time: " << current_time << endl;
            }

            // If the current running process has higher priority than the previously running task then the current running process is executed
            if (runningProcess.task_period < PrevRunningProcess.task_period)
            {
                // If the previously running task was not terminated then it is preempted
                if (PrevRunningProcess.task_id > 0)
                {
                    output_file_log << "Task ID: " << PrevRunningProcess.task_id << " preempted by Task ID: " << runningProcess.task_id
                                    << " at time: " << current_time << " Remaining Time: " << PrevRunningProcess.task_time << endl;
                    PrevRunningProcess.isInterrupted = true;
                    ReadyQueue.push_back(PrevRunningProcess);
                }

                // Running the current running process
                // If it was originally preempted then it resumes the process
                if (runningProcess.isInterrupted == true)
                {
                    output_file_log << "Task ID: " << runningProcess.task_id << " resumed at time: " << current_time << endl;
                    runningProcess.isInterrupted = false;
                }
                // Else it is executed for the first time
                else
                {
                    output_file_log << "Task ID: " << runningProcess.task_id << " started at time: " << current_time << endl;
                }
            }
            else
            {
                // If the previously running task has higher priority then the current running task is pushed to the Ready Queue
                ReadyQueue.push_back(runningProcess);
                runningProcess = PrevRunningProcess;
            }

            // Boolean to check if the current running process is completed or not
            bool isCompleted = false;

            // If the process can be completed in time then it is completed
            if (current_time + runningProcess.task_time <= next_arrival_time)
            {
                isCompleted = true;
            }
            while (isCompleted == true)
            {
                // Current time is updated and process is completed
                output_file_log << "Task ID: " << runningProcess.task_id << " completed at time: " << current_time + runningProcess.task_time << endl;
                total_waiting_time += current_time - runningProcess.task_arrivaltime - runningProcess.task_time;

                // Take next process from the Ready Queue
                if (ReadyQueue.empty() == false && current_time < next_arrival_time)
                {
                    runningProcess = findHighestPriority(ReadyQueue);
                    // If the Process was originally preempted then it resumes the process
                    if (runningProcess.isInterrupted == true)
                    {
                        runningProcess.isInterrupted = false;
                        output_file_log << "Task ID: " << runningProcess.task_id << " resumed at time: " << current_time << endl;
                    }
                    else
                    {
                        output_file_log << "Task ID: " << runningProcess.task_id << " started at time: " << current_time << endl;
                    }
                }
                else if (next_arrival_time == INT_MAX)
                {
                    // If ready queue was empty and all processes have been completed then it breaks from the loop
                    break;
                }
                else if (current_time < next_arrival_time)
                {
                    // If the ready queue was empty but there is still some time left then the CPU is idle upto the next task arrival time
                    current_time = next_arrival_time;
                    IsIdle = true;
                    PrevRunningProcess = TaskProcess(INT_MIN, 0, INT_MAX, 0);
                    break;
                }
                else
                {
                    PrevRunningProcess = TaskProcess(INT_MIN, 0, INT_MAX, 0);
                    break;
                }
                if (current_time + runningProcess.task_time > next_arrival_time)
                {
                    isCompleted = false;
                }
            }

            if (isCompleted == false)
            {
                runningProcess.task_time = current_time + runningProcess.task_time - next_arrival_time;
                PrevRunningProcess = runningProcess;
            }
        }
        // If the Ready Queue is empty and the Previously Running Process has also been completed or terminated then the current time to set to the next process arrival time
        else
        {
            current_time = next_arrival_time;
        }
    }

    ofstream output_statistics_file;
    output_statistics_file.open("RMS_Stats.txt");
    output_statistics_file << "Average Waiting Time: " << total_waiting_time / (double)num_task << endl;
    output_statistics_file << "Total Number of Processes: " << num_task << endl;
    output_statistics_file << "Total Number of Deadline Misses: " << num_deadline_miss << endl;

    output_statistics_file.close();
    output_file_log.close();
    input_file.close();
    cout << "Done" << endl;
    return EXIT_SUCCESS;
}