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
#define DEFN 100

FILE *output_log_ptr;
sem_t rw_mutex;
sem_t mutex_;
int read_count;
sem_t in_mutex;

time_t GlobalWaitTime[DEFN];
time_t WorstWaitTime[DEFN];

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
    int subject_id;
    void initializer(int nw, int nr, double u_cs, double u_rem, int thread_id, int subject_id)
    {
        this->nw = nw;
        this->nr = nr;
        this->kw = 1;
        this->kr = 1;
        this->u_cs = u_cs;
        this->u_rem = u_rem;
        this->thread_id = thread_id;
        this->subject_id = subject_id;
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

void *subject(void *param)
{
    thread_parameters_t *p = (thread_parameters_t *)param;
    int kw = 1;
    double u_cs = 1.0 / p->u_cs;
    double u_rem = 1.0 / p->u_rem;
    int thread_id = p->thread_id;
    int subject_id = p->subject_id;

    // Standard default random number generator to be passed into exponential distribution function
    std::default_random_engine random_number_generator;

    // Exponential distribution function to generate random numbers with exponential distribution
    std::exponential_distribution<double> distribution_cs(u_cs);
    std::exponential_distribution<double> distribution_rem(u_rem);

    for (int i = 0; i < kw; i++)
    {
        time_t reqTime = time(NULL);
        tm *reqTime_tm = localtime(&reqTime);

        fprintf(output_log_ptr, "Request by subject %d student %d at %s\n", subject_id, thread_id, printTime_tm_ptr(reqTime_tm).c_str());
        sem_wait(&in_mutex);
        sem_wait(&rw_mutex);

        time_t enterTime = time(NULL);
        tm *enterTime_tm = localtime(&enterTime);
        fprintf(output_log_ptr, "Entry by subject %d student %d at %s\n", subject_id, thread_id, printTime_tm_ptr(enterTime_tm).c_str());

        GlobalWaitTime[subject_id] += enterTime - reqTime;
        // WorstWaitTime_Writer = max(WorstWaitTime_Writer, enterTime - reqTime);
        WorstWaitTime[subject_id] = max(WorstWaitTime[subject_id], enterTime - reqTime);

        sleep(distribution_cs(random_number_generator));
        sem_post(&rw_mutex);
        sem_wait(&in_mutex);

        time_t exitTime = time(NULL);
        tm *exitTime_tm = localtime(&exitTime);
        fprintf(output_log_ptr, "Exit by subject %d student %d at %s\n", subject_id, thread_id, printTime_tm_ptr(exitTime_tm).c_str());

        sleep(distribution_rem(random_number_generator));
    }

    return NULL;
}

/**********
 * This program assumes that m is 1 and n is greater than 0
 * Also it is taking inspiration from readers writers solution program
 * first subject can act as writer and other can act as reader
 * As we are assuming that value of m is 1 so only one student is accessing the lab at a time
 *
 * Also it is assumed that number of subjects are not greater than 100 as defined by macro DEFN
 * This is like a fair readers writers solution with an extra semaphore
 */

int main()
{
    srand(time(NULL));
    ifstream input_ptr;
    input_ptr.open("input.txt");

    output_log_ptr = fopen("WaitStudentsC_log.txt", "w");
    ofstream output_ptr;
    output_ptr.open("WaitStudentsC_output.txt");

    int K, m, n;
    double lambda, mu;
    input_ptr >> K >> m >> lambda >> n >> mu;
    // int num_subject1 = 0, num_subject2 = 0;
    m = 1;
    // K = 2;
    int num_subject[K];
    // for (int i = 0; i < n; i++)
    // {
    //     if (rand() % 2 == 0)
    //     {
    //         num_subject1++;
    //     }
    //     else
    //     {
    //         num_subject2++;
    //     }
    // }
    // int total_students = n;

    for(int i = 0; i < K; i++)
    {
        num_subject[i] = 0;
    }
    std::default_random_engine random_uniform_generator;
    std::uniform_int_distribution<int> distribution_uniform(0, K - 1);
    
    // int total_students_alloted = 0;
    // for (int i = 0; i < K - 1; i++)
    // {
    //     int a = rand() % total_students;
    //     num_subject[i] = a;
    //     total_students -= a;
    //     // total_students_alloted += a;
    // }
    // num_subject[K - 1] = total_students;
    for(int i = 0; i < n; i++)
    {
        int a = distribution_uniform(random_uniform_generator);
        num_subject[a]++;
    }

    for (int i = 0; i < DEFN; i++)
    {
        GlobalWaitTime[i] = 0;
        WorstWaitTime[i] = 0;
    }
    read_count = 0;
    sem_init(&mutex_, 0, 1);
    sem_init(&rw_mutex, 0, 1);
    sem_init(&in_mutex, 0, 1);
    for(int i = 0; i < K; i++)
    {
        cout << "Subject " << i + 1 << ":"
             << num_subject[i] << " students" << endl;
    }

    pthread_t subject_threads[K][n];
    thread_parameters_t subject_parameters[K][n];

    for (int i = 0; i < K; i++)
    {
        for (int j = 0; j < num_subject[i]; j++)
        {
            pthread_attr_t attr;
            pthread_attr_init(&attr);

            subject_parameters[i][j].initializer(1, 1, lambda, mu, j, i);
            pthread_create(&subject_threads[i][j], &attr, subject, &subject_parameters[i][j]);
        }
    }

    for (int i = 0; i < K; i++)
    {
        for (int j = 0; j < num_subject[i]; j++)
        {
            pthread_join(subject_threads[i][j], NULL);
        }
    }

    output_ptr << "Statistics" << endl;
    for (int i = 0; i < K; i++)
    {
        if (num_subject[i] > 0)
        {
            output_ptr << "Subject " << i + 1 << ":"
                       << " Average Wait Time: " << GlobalWaitTime[i] / num_subject[i]
                       << " Worst Wait Time: " << WorstWaitTime[i] << endl;
        }
    }
    
    output_ptr.close();
    input_ptr.close();
    fclose(output_log_ptr);
    return EXIT_SUCCESS;
}