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
#include <bits/stdc++.h>
using namespace std;

// Class for giving paramters to testCS function
class thread_parameters_t
{
public:
    int thread_id;
    int k;
    double l1, l2;
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

// Variable to store the Wait Time Globally
time_t GlobalWaitTime;

// Variable to store the Wosrt Waiting time for threads
time_t WorstWaitTime;

// Using FILE* to write to a file
FILE *output_file_ptr;

// Atomic lock
std::atomic<int> lock_guard_(0);

// Test Critical section Function to test the Critical section
void *testCS(void *param)
{
    // Copying all the paramters obtained from param
    thread_parameters_t *p = (thread_parameters_t *)param;
    int thread_id = p->thread_id;
    int k = p->k;

    // Standard default random number generator to be passed into exponential distribution function
    std::default_random_engine random_number_generator;

    // Double variables to denote lambda 1 and lambda 2
    double l1 = p->l1;
    double l2 = p->l2;

    // Exponential distribution function to generate random numbers with exponential distribution
    std::exponential_distribution<double> distribution_l1(1 / l1);
    std::exponential_distribution<double> distribution_l2(1 / l2);

    // Running a thread k times
    for (int i = 0; i < k; i++)
    {
        // time for requesting to enter time
        time_t reqEnterTime = time(NULL);
        tm *reqEnterTime_tm = localtime(&reqEnterTime);
                
        /*******************
         * Comment this output to the file line when running for report part as a huge file will be read into memory again and again
         */ 
        // output_file_log << "CS: " << i << " The CS Request at: " << printTime_tm_ptr(reqEnterTime_tm) << " by thread " << thread_id << "\n";
        fprintf(output_file_ptr, " %d CS Request at %s by thread %d \n", i + 1, printTime_tm_ptr(reqEnterTime_tm).c_str(), thread_id);

        /**********************
         *   ENTRY SECTION
         */

        // CAS lock
        while (true)
        {
            int a = 0, b = 1;
            // if (lock_guard_.compare_exchange_weak(a, b))
            // {
            //     break;
            // }
            if(atomic_compare_exchange_weak(&lock_guard_, &a, b))
            {
                break;
            }
        }

        // time for entering time
        time_t actEnterTime = time(NULL);
        tm *actEnterTime_tm = localtime(&actEnterTime);

        // Adding Waiting time to the global variable
        GlobalWaitTime = GlobalWaitTime + actEnterTime - reqEnterTime;
        WorstWaitTime = max(WorstWaitTime, actEnterTime - reqEnterTime);

        /*******************
         * Comment this output to the file line when running for report part as a huge file will be read into memory again and again
         */
        // output_file_log << "CS: " << i << " The CS Entry at: " << printTime_tm_ptr(localtime(&actEnterTime)) << " by thread " << thread_id << "\n";
        fprintf(output_file_ptr, " %d CS Entry at %s by thread %d \n", i + 1, printTime_tm_ptr(actEnterTime_tm).c_str(), thread_id);

        /**********************
         *   SLEEP CRITICAL SECTION
         */
        //Simulation of critical section
        sleep(distribution_l1(random_number_generator));

        /**********************
         *   EXIT SECTION
         */

        // Time for exiting time
        time_t actExitTime = time(NULL);
        tm *actExitTime_tm = localtime(&actExitTime);

        /*******************
         * Comment this output to the file line when running for report part as a huge file will be read into memory again and again
         */
        // output_file_log << "CS: " << i << " The CS Exit at: " << printTime_tm_ptr(localtime(&actExitTime)) << " by thread " << thread_id << "\n";
        fprintf(output_file_ptr, " %d CS Exit at %s by thread %d \n", i + 1, printTime_tm_ptr(actExitTime_tm).c_str(), thread_id);

        lock_guard_ = 0;

        /**********************
         *   REMAINDER SECTION
         */
        //Simulation of remainder section
        sleep(distribution_l2(random_number_generator));
    }
    return NULL;
}

int main()
{
    // Input file stream
    ifstream input_file;

    // Variables for number of threads
    int n = 0;

    // Variable for number of repeats
    int k = 0;

    // Variable for lambda 1 of exponential distribution
    double l1 = 0;

    // Variable for lambda 2 of exponential distribution
    double l2 = 0;

    // Initializing the Wait time variables
    GlobalWaitTime = 0;
    WorstWaitTime = 0;

    srand(time(NULL));
    /****************
     * Change the Name of Input File Here
     */
    input_file.open("inp_params.txt");
    if (input_file.is_open() == false)
    {
        cout << "Error in opening the input file" << endl;
        return EXIT_FAILURE;
    }
    cout << "Input File Opened Successfully" << endl;
    /***********************************
     */
    // output_file_log.open("output_log.txt");
    output_file_ptr = fopen("output_log.txt", "a");
    fprintf(output_file_ptr, "CAS Output\n");

    // Ofstream to write the output to a file
    ofstream output_file_log;
    output_file_log.open("output_stats.txt", ios::app);
    // Reading the input file
    input_file >> n >> k >> l1 >> l2;

    // Creating an array of threds and thread_parameters_t to store all the parameters
    pthread_t threads_arr[n];
    thread_parameters_t argrument_thread[n];
    // for (int i = 0; i < n; i++)
    // {
    //     argrument_thread[i] = new thread_parameters_t;
    // }

    // Creating threads and passing the parameters
    for (int i = 0; i < n; i++)
    {
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        argrument_thread[i].thread_id = i + 1;
        argrument_thread[i].k = k;
        argrument_thread[i].l1 = l1;
        argrument_thread[i].l2 = l2;
        pthread_create(&threads_arr[i], &attr, testCS, &argrument_thread[i]);
    }

    // Joining all the threads
    for (int i = 0; i < n; i++)
    {
        pthread_join(threads_arr[i], NULL);
    }

    output_file_log << "The Average Wait Time for CAS n: " << n << " is " << GlobalWaitTime / (n * k * 1.0) << endl;
    output_file_log << "The Worst Wait Time for CAS n: " << n << " is " << WorstWaitTime << endl;

    // Closing the files
    input_file.close();
    output_file_log.close();
    fclose(output_file_ptr);
    cout << "Done\n";

    return EXIT_SUCCESS;
}