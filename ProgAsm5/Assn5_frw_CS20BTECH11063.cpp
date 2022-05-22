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

// Using FILE* to write to a file
FILE *output_log_ptr;
sem_t rw_mutex;
sem_t mutex_;
int read_count;

/////////////////
sem_t in_mutex;
////////////////

// Variable to store the Wait Time Globally
time_t GlobalWaitTime_Writer;
time_t GlobalWaitTime_Reader;

// Variable to store the Wosrt Waiting time for threads
time_t WorstWaitTime_Writer;
time_t WorstWaitTime_Reader;

class thread_parameters_t
{
public:
    int nw, nr, kw, kr;
    double u_cs, u_rem;
    int thread_id;
    // thread_parameters_t(int nw, int nr, int kw, int kr, double u_cs, double u_rem, int thread_id)
    // {
    //     this->nw = nw;
    //     this->nr = nr;
    //     this->kw = kw;
    //     this->kr = kr;
    //     this->u_cs = u_cs;
    //     this->u_rem = u_rem;
    //     this->thread_id = thread_id;
    // }
    void initializer(int nw, int nr, int kw, int kr, double u_cs, double u_rem, int thread_id)
    {
        this->nw = nw;
        this->nr = nr;
        this->kw = kw;
        this->kr = kr;
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

void *writer(void *param)
{
    // Copying all the paramters obtained from param
    thread_parameters_t *p = (thread_parameters_t *)param;
    // int nw = p->nw, nr = p->nr, kw = p->kw, kr = p->kr;
    int kw = p->kw;
    double u_cs = p->u_cs, u_rem = p->u_rem;
    int thread_id = p->thread_id;

    // Standard default random number generator to be passed into exponential distribution function
    std::default_random_engine random_number_generator;

    // Exponential distribution function to generate random numbers with exponential distribution
    std::exponential_distribution<double> distribution_cs(1 / u_cs);
    std::exponential_distribution<double> distribution_rem(1 / u_rem);

    // Running a thread k times
    for (int i = 0; i < kw; i++)
    {
        time_t reqTime = time(NULL);
        tm *reqTime_tm = localtime(&reqTime);

        fprintf(output_log_ptr, " %d CS Request by Writer thread %d at %s\n", i, thread_id, printTime_tm_ptr(reqTime_tm).c_str());

        ///////////////////
        sem_wait(&in_mutex);
        ///////////////////
        sem_wait(&rw_mutex);

        time_t enterTime = time(NULL);
        tm *enterTime_tm = localtime(&enterTime);
        fprintf(output_log_ptr, " %d CS Entry by Writer thread %d at %s\n", i, thread_id, printTime_tm_ptr(enterTime_tm).c_str());

        GlobalWaitTime_Writer += enterTime - reqTime;
        WorstWaitTime_Writer = max(WorstWaitTime_Writer, enterTime - reqTime);

        sleep(distribution_cs(random_number_generator));
        sem_post(&rw_mutex);


        ///////////////////
        sem_post(&in_mutex);
        ///////////////////

        time_t exitTime = time(NULL);
        tm *exitTime_tm = localtime(&exitTime);
        fprintf(output_log_ptr, " %d CS Exit by Writer thread %d at %s\n", i, thread_id, printTime_tm_ptr(exitTime_tm).c_str());

        sleep(distribution_rem(random_number_generator));
    }
    return NULL;
}

void *reader(void *param)
{
    // Copying all the paramters obtained from param
    thread_parameters_t *p = (thread_parameters_t *)param;
    // int nw = p->nw, nr = p->nr, kw = p->kw, kr = p->kr;
    int kr = p->kr;
    double u_cs = p->u_cs, u_rem = p->u_rem;
    int thread_id = p->thread_id;

    // Standard default random number generator to be passed into exponential distribution function
    std::default_random_engine random_number_generator;

    // Exponential distribution function to generate random numbers with exponential distribution
    std::exponential_distribution<double> distribution_cs(1 / u_cs);
    std::exponential_distribution<double> distribution_rem(1 / u_rem);

    // Running a thread k times
    for (int i = 0; i < kr; i++)
    {
        time_t reqTime = time(NULL);
        tm *reqTime_tm = localtime(&reqTime);

        fprintf(output_log_ptr, " %d CS Request by Reader thread %d at %s\n", i, thread_id, printTime_tm_ptr(reqTime_tm).c_str());

        ///////////////////
        sem_wait(&in_mutex);
        ///////////////////
        sem_wait(&mutex_);

        read_count++;
        if (read_count == 1)
        {
            sem_wait(&rw_mutex);
        }
        sem_post(&mutex_);

        ///////////////////
        sem_post(&in_mutex);
        ///////////////////

        time_t enterTime = time(NULL);
        tm *enterTime_tm = localtime(&enterTime);
        fprintf(output_log_ptr, " %d CS Entry by Reader thread %d at %s\n", i, thread_id, printTime_tm_ptr(enterTime_tm).c_str());

        GlobalWaitTime_Reader += enterTime - reqTime;
        WorstWaitTime_Reader = max(WorstWaitTime_Reader, enterTime - reqTime);

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
        fprintf(output_log_ptr, " %d CS Exit by Reader thread %d at %s\n", i, thread_id, printTime_tm_ptr(exitTime_tm).c_str());

        sleep(distribution_rem(random_number_generator));
    }
    return NULL;
}

int main()
{
    // Input file stream
    ifstream input_file;
    input_file.open("inp_params.txt");
    if (input_file.is_open() == false)
    {
        cout << "Error in opening the input file" << endl;
        return EXIT_FAILURE;
    }
    cout << "Input File Opened Successfully" << endl;

    output_log_ptr = fopen("FRW_log.txt", "w");
    ofstream output_file;
    output_file.open("AverageTime.txt");
    int nw, nr, kw, kr;
    double u_cs, u_rem;
    input_file >> nw >> nr >> kw >> kr >> u_cs >> u_rem;
    sem_init(&mutex_, 0, 1);
    sem_init(&rw_mutex, 0, 1);

    ////////////////
    sem_init(&in_mutex, 0, 1);
    ////////////////

    pthread_t writer_threads[nw];
    pthread_t reader_threads[nr];
    thread_parameters_t writer_parameters[nw];
    thread_parameters_t reader_parameters[nr];
    for (int i = 0; i < nw; i++)
    {
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        writer_parameters[i].initializer(nw, nr, kw, kr, u_cs, u_rem, i);
        pthread_create(&writer_threads[i], &attr, writer, &writer_parameters[i]);
    }
    for (int i = 0; i < nr; i++)
    {
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        reader_parameters[i].initializer(nw, nr, kw, kr, u_cs, u_rem, i);
        pthread_create(&reader_threads[i], &attr, reader, &reader_parameters[i]);
    }
    for (int i = 0; i < nr; i++)
    {
        pthread_join(reader_threads[i], NULL);
    }
    for (int i = 0; i < nw; i++)
    {
        pthread_join(writer_threads[i], NULL);
    }
    
    fclose(output_log_ptr);
    output_file << "FRW Statistics" << endl;
    output_file << "Average Wait Time for Writer Threads: " << GlobalWaitTime_Writer / (nw * kw * 1.0) << endl;
    output_file << "Worst Wait Time for Writer Threads: " << WorstWaitTime_Writer << endl;
    output_file << "Average Wait Time for Reader Threads: " << GlobalWaitTime_Reader / (nr * kr * 1.0) << endl;
    output_file << "Worst Wait Time for Reader Threads: " << WorstWaitTime_Reader << endl;
    output_file.close();
    input_file.close();

    ofstream output_file_report;
    output_file_report.open("Report_FRW.txt", ios::app);
    output_file_report << nw << " " << nr << " " << GlobalWaitTime_Writer / (nw * kw * 1.0) << " " << WorstWaitTime_Writer << " " << GlobalWaitTime_Reader / (nr * kr * 1.0) << " " << WorstWaitTime_Reader << endl;
    output_file_report.close();
    return EXIT_SUCCESS;
}