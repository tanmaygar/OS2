#include <semaphore>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <vector>
#include <random>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cassert>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <bits/stdc++.h>

class semaphore
{
private:
    std::mutex m;
    std::condition_variable cv;
    int count;
public:
    semaphore(int count = 0) : count(count) {}
    void wait()
    {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, [this]{return this->count > 0;});
        --count;
    }
    void signal()
    {
        std::lock_guard<std::mutex> lk(m);
        ++count;
        cv.notify_one();
    }
};

void wait(semaphore &q)
{
    return;
}
void signal(semaphore &q)
{
    return;
}

int main() {
    
    semaphore sem1(1);
    semaphore mutex(1);
    semaphore sem2(1);
    int count = 0;
    
    //Writer
    while(true)
    {
        wait(sem1);
        wait(sem2);
        //Writer Part
        signal(sem2);
        signal(sem1);
    }
    //Reader
    while(true)
    {
        wait(sem1);
        wait(mutex);
        count = count + 1;
        if(count == 1)
        {
            wait(sem2);
        }
        signal(mutex);
        signal(sem1);
        //Reader Part
        wait(mutex);
        count = count - 1;
        if(count == 0)
        {
            signal(sem2);
        }
        signal(mutex);
    }







    return 0;
}