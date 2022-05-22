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
#include <pthread.h>
#include <atomic>
#include <chrono>
#include <cerrno>
#include <unistd.h>
#include <random>
#include <cmath>
#include <semaphore.h>
#include <bits/stdc++.h>
using namespace std;

FILE *output_log_ptr;
sem_t rw_mutex;
sem_t mutex_;
int read_count;

// Variable to store the Wait Time Globally
time_t GlobalWaitTime_Sub1;
time_t GlobalWaitTime_Sub2;

// Variable to store the Wosrt Waiting time for threads
time_t WorstWaitTime_Sub1;
time_t WorstWaitTime_Sub2;

class student_t
{
public:
    int subject_id;
    int student_id;
};

class thread_parameters_t
{
public:
    int nw, nr, kw, kr;
    double u_cs, u_rem;
    int thread_id;
    void initializer(int nw, int nr, double u_cs, double u_rem, int thread_id)
    {
        this->nw = nw;
        this->nr = nr;
        this->kw = 1;
        this->kr = 1;
        this->u_cs = u_cs;
        this->u_rem = u_rem;
        this->thread_id = thread_id;
    }
};

// Function to print Time by taking struct tm pointer
string printTime_tm_ptr(tm *t)
{
    string s(asctime(t));
    s.erase(s.end() - 1);
    return s;
}

// Function to print Time by taking time_t reference
string printtime(time_t &t)
{
    tm *t_tm = localtime(&t);
    string s(asctime(t_tm));
    s.erase(s.end() - 1);
    return s;
}

void *subject1(void *param)
{
    thread_parameters_t *p = (thread_parameters_t *)param;
    int kw = 1;
    double u_cs = 1.0 / p->u_cs;
    double u_rem = 1.0 / p->u_rem;
    int thread_id = p->thread_id;

    // Standard default random number generator to be passed into exponential distribution function
    std::default_random_engine random_number_generator;

    // Exponential distribution function to generate random numbers with exponential distribution
    std::exponential_distribution<double> distribution_cs(u_cs);
    std::exponential_distribution<double> distribution_rem(u_rem);

    for (int i = 0; i < kw; i++)
    {
        time_t reqTime = time(NULL);
        tm *reqTime_tm = localtime(&reqTime);

        fprintf(output_log_ptr, "Request by subject 1 student %d at %s\n", thread_id, printTime_tm_ptr(reqTime_tm).c_str());
        sem_wait(&rw_mutex);

        time_t enterTime = time(NULL);
        tm *enterTime_tm = localtime(&enterTime);
        fprintf(output_log_ptr, "Entry by subject 1 student %d at %s\n", thread_id, printTime_tm_ptr(enterTime_tm).c_str());

        GlobalWaitTime_Sub1 += enterTime - reqTime;
        WorstWaitTime_Sub1 = max(WorstWaitTime_Sub1, enterTime - reqTime);

        sleep(distribution_cs(random_number_generator));
        sem_post(&rw_mutex);

        time_t exitTime = time(NULL);
        tm *exitTime_tm = localtime(&exitTime);
        fprintf(output_log_ptr, "Exit by subject 1 student %d at %s\n", thread_id, printTime_tm_ptr(exitTime_tm).c_str());

        sleep(distribution_rem(random_number_generator));
    }
    return NULL;
}

void *subject2(void *param)
{
    // Copying all the paramters obtained from param
    thread_parameters_t *p = (thread_parameters_t *)param;

    int kr = 1;
    double u_cs = 1.0 / p->u_cs;
    double u_rem = 1.0 / p->u_rem;
    int thread_id = p->thread_id;

    // Standard default random number generator to be passed into exponential distribution function
    std::default_random_engine random_number_generator;

    // Exponential distribution function to generate random numbers with exponential distribution
    std::exponential_distribution<double> distribution_cs(u_cs);
    std::exponential_distribution<double> distribution_rem(u_rem);

    // Running a thread k times
    for (int i = 0; i < kr; i++)
    {
        time_t reqTime = time(NULL);
        tm *reqTime_tm = localtime(&reqTime);

        fprintf(output_log_ptr, "Request by subject 2 student %d at %s\n", thread_id, printTime_tm_ptr(reqTime_tm).c_str());
        sem_wait(&mutex_);

        read_count++;
        if (read_count == 1)
        {
            sem_wait(&rw_mutex);
        }
        sem_post(&mutex_);

        time_t enterTime = time(NULL);
        tm *enterTime_tm = localtime(&enterTime);
        fprintf(output_log_ptr, "Entry by subject 2 student %d at %s\n", thread_id, printTime_tm_ptr(enterTime_tm).c_str());

        GlobalWaitTime_Sub2 += enterTime - reqTime;
        WorstWaitTime_Sub2 = max(WorstWaitTime_Sub2, enterTime - reqTime);

        sleep(distribution_cs(random_number_generator));
        sem_wait(&mutex_);
        read_count--;
        if (read_count == 0)
        {
            sem_post(&rw_mutex);
        }
        sem_post(&mutex_);

        time_t exitTime = time(NULL);
        tm *exitTime_tm = localtime(&exitTime);
        fprintf(output_log_ptr, "Exit by subject 2 student %d at %s\n", thread_id, printTime_tm_ptr(exitTime_tm).c_str());

        sleep(distribution_rem(random_number_generator));
    }
    return NULL;
}

/**********
 * This program assumes that m is 1 and n is greater than 0
 * Also it is taking inspiration from readers writers solution program
 * first subject can act as writer and other can act as reader
 * As we are assuming that value of m is 1 so only one student is accessing the lab at a time
 */

int main()
{
    srand(time(NULL));
    ifstream input_ptr;
    input_ptr.open("input.txt");

    output_log_ptr = fopen("WaitStudentsA_log.txt", "w");
    ofstream output_ptr;
    output_ptr.open("WaitStudentsA_output.txt");

    int K, m, n;
    double lambda, mu;
    input_ptr >> K >> m >> lambda >> n >> mu;
    int num_subject1 = 0, num_subject2 = 0;
    m = 1;
    K = 2;
    for (int i = 0; i < n; i++)
    {
        if (rand() % 2 == 0)
        {
            num_subject1++;
        }
        else
        {
            num_subject2++;
        }
    }

    GlobalWaitTime_Sub2 = 0;
    GlobalWaitTime_Sub1 = 0;
    WorstWaitTime_Sub2 = 0;
    WorstWaitTime_Sub1 = 0;
    read_count = 0;
    sem_init(&mutex_, 0, 1);
    sem_init(&rw_mutex, 0, 1);

    pthread_t subject1_threads[num_subject1];
    pthread_t subject2_threads[num_subject2];

    thread_parameters_t subject1_parameters[num_subject1];
    thread_parameters_t subject2_parameters[num_subject2];

    for (int i = 0; i < num_subject1; i++)
    {
        pthread_attr_t attr;
        pthread_attr_init(&attr);

        subject1_parameters[i].initializer(num_subject1, num_subject2, lambda, mu, i);
        pthread_create(&subject1_threads[i], &attr, subject1, &subject1_parameters[i]);
    }
    for (int i = 0; i < num_subject2; i++)
    {
        pthread_attr_t attr;
        pthread_attr_init(&attr);

        subject2_parameters[i].initializer(num_subject1, num_subject2, lambda, mu, i);
        pthread_create(&subject2_threads[i], &attr, subject2, &subject2_parameters[i]);
    }
    for (int i = 0; i < num_subject1; i++)
    {
        pthread_join(subject1_threads[i], NULL);
    }
    for (int i = 0; i < num_subject2; i++)
    {
        pthread_join(subject2_threads[i], NULL);
    }

    output_ptr << "Statistics" << endl;
    output_ptr << "Average Wait Time for Subject 1 Students: " << GlobalWaitTime_Sub1 / (num_subject1 * 1.0) << endl;
    output_ptr << "Worst Wait Time for Subject 1 Threads: " << WorstWaitTime_Sub1 << endl;
    output_ptr << "Average Wait Time for Subject 2 Threads: " << GlobalWaitTime_Sub2 / (num_subject2* 1.0) << endl;
    output_ptr << "Worst Wait Time for Subject 2 Threads: " << WorstWaitTime_Sub2 << endl;

    
    output_ptr.close();
    input_ptr.close();
    fclose(output_log_ptr);
    return EXIT_SUCCESS;
}