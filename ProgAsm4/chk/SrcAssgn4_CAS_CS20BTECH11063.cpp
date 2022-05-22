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
using namespace std;

class thread_parameters_t
{
public:
    int thread_id;
    int k;
    double l1, l2;
};

string printTime(time_t &t)
{
    string s = ctime(&t);
    return s;
}
string printTime2(tm *t)
{
    string s = asctime(t);
    return s;
}

time_t GlobalWaitTime;
time_t WorstWaitTime;

atomic<int> lock_guard(0);

void *testCS(void *param)
{
    thread_parameters_t *p = (thread_parameters_t *)param;
    int thread_id = p->thread_id;
    int k = p->k;
    std::default_random_engine random_number_generator;
    //int random_time = 1000 + rand() % 1000;
    double l1 = p->l1;
    double l2 = p->l2;

    std::exponential_distribution<double> distribution_l1(1 / l1);
    std::exponential_distribution<double> distribution_l2(1 / l2);

    for (int i = 0; i < k; i++)
    {
        time_t reqEnterTime = time(NULL);
        tm *reqEnterTime_tm = localtime(&reqEnterTime);
        cout << i << "The CS Request at: " << printTime2(reqEnterTime_tm) << " by thread " << thread_id << endl;

        while (true)
        {
            int a = 0, b = 1;
            if (lock_guard.compare_exchange_weak(a, b))
            {
                break;
            }
        }

        time_t actEnterTime = time(NULL);
        GlobalWaitTime = GlobalWaitTime + actEnterTime - reqEnterTime;
        WorstWaitTime = max(WorstWaitTime, actEnterTime - reqEnterTime);
        cout << i << "The CS Entry at: " << printTime2(localtime(&actEnterTime)) << " by thread " << thread_id << endl;

        sleep(distribution_l1(random_number_generator));

        time_t actExitTime = time(NULL);
        cout << i << "The CS Exit at: " << printTime2(localtime(&actExitTime)) << " by thread " << thread_id << endl;

        lock_guard = 0;
        sleep(distribution_l2(random_number_generator));
    }
    return NULL;
}

int main()
{
    ifstream input_file;
    ofstream output_file_log;

    int n = 0, k = 0;
    double l1 = 0, l2 = 0;
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
    output_file_log.open("output_log.txt");

    input_file >> n >> k >> l1 >> l2;
    pthread_t threads_arr[n];
    struct thread_parameters_t *argrument_thread[n];
    for (int i = 0; i < n; i++)
    {
        argrument_thread[i] = new thread_parameters_t;
    }
    for (int i = 0; i < n; i++)
    {
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        argrument_thread[i]->thread_id = i + 1;
        argrument_thread[i]->k = k;
        argrument_thread[i]->l1 = l1;
        argrument_thread[i]->l2 = l2;
        pthread_create(&threads_arr[i], &attr, testCS, argrument_thread);
    }
    for (int i = 0; i < n; i++)
    {
        pthread_join(threads_arr[i], NULL);
    }

    return EXIT_SUCCESS;
}