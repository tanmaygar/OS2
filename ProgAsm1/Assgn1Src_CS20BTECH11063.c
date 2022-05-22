#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#define MAX_ARR_SIZE 5000
#define MAX_THREAD 16

//Struct of Coordinates
struct point_t
{
    int x, y;
};

//Struct for giving paramters to find_nearest_point function
struct argument_multithreading_t
{
    int startIndex, endIndex;
    struct point_t nearest_point;
};

//Array of Struct to store list of all coordinates
struct point_t listPoints[MAX_ARR_SIZE];

//Global Struct for the Source point
struct point_t SrcPoint;

//Global Variable for Number of Points
int NumPoints = 0;

//Function to find the distance between two points
double find_distance(struct point_t *p1, struct point_t *p2)
{
    double distance = sqrt(pow((p1->x - p2->x), 2) + pow((p1->y - p2->y), 2));
    return distance;
}

//Function to find nearest point amongst an array of points wrt Source coordinates by passing arguments
void *find_nearest_point(void *arg)
{
    //Initializing the starting and ending point in the array
    int startIndex = ((struct argument_multithreading_t *)arg)->startIndex;
    int endIndex = ((struct argument_multithreading_t *)arg)->endIndex;

    double distance = DBL_MAX;
    double tmp_distance;
    for (int i = startIndex; i <= endIndex; i++)
    {
        tmp_distance = find_distance(&SrcPoint, &listPoints[i]);
        if (tmp_distance < distance)
        {
            distance = tmp_distance;
            ((struct argument_multithreading_t *)arg)->nearest_point.x = listPoints[i].x;
            ((struct argument_multithreading_t *)arg)->nearest_point.y = listPoints[i].y;
        }
    }

    return NULL;
}

// //This main function is for Input Output part of the Assignment
// int main()
// {
//     int num_threads = 2;

//     srand(time(NULL));
//     char input_filename[20] = "input.txt";

//     //Taking input from file
//     FILE *ptr = fopen(input_filename, "r");
//     if (ptr == NULL)
//     {
//         printf("FILE DOES NOT EXIST\n");
//         return EXIT_FAILURE;
//     }
//     fscanf(ptr, "%d", &num_threads);
//     fscanf(ptr, "%d %d", &SrcPoint.x, &SrcPoint.y);
//     fscanf(ptr, "%d", &NumPoints);
//     int tmp_x, tmp_y;
//     for (int i = 0; i < NumPoints; i++)
//     {
//         fscanf(ptr, "%d %d", &tmp_x, &tmp_y);
//         listPoints[i].x = tmp_x;
//         listPoints[i].y = tmp_y;
//     }
//     fclose(ptr);
//     // SrcPoint.x = 1;
//     // SrcPoint.y = 1;
//     // NumPoints = 4;
//     // listPoints[0].x = 2;
//     // listPoints[0].y = 2;
//     // listPoints[1].x = 4;
//     // listPoints[1].y = 5;
//     // listPoints[2].x = 0;
//     // listPoints[2].y = 3;
//     // listPoints[3].x = 1;
//     // listPoints[3].y = 1;

//     clock_t t = clock();

//     //Threads
//     pthread_t threads_arr[MAX_THREAD];

//     //Allocating memory for arguments struct array
//     struct argument_multithreading_t *argrument_findNearest[MAX_THREAD];
//     for (int i = 0; i < num_threads; i++)
//     {
//         argrument_findNearest[i] = malloc(sizeof(struct argument_multithreading_t));
//     }

//     //Initializing the startIndex and endIndex for subarrays
//     int startIndex_loop = 0, endIndex_loop = NumPoints / num_threads - 1;
//     for (int i = 0; i < num_threads; i++)
//     {
//         if (i == num_threads - 1)
//         {
//             argrument_findNearest[i]->startIndex = startIndex_loop;
//             argrument_findNearest[i]->endIndex = NumPoints - 1;
//             pthread_attr_t attr;
//             pthread_attr_init(&attr);
//             pthread_create(&threads_arr[i], &attr, find_nearest_point, argrument_findNearest[i]);
//         }
//         else
//         {
//             argrument_findNearest[i]->startIndex = startIndex_loop;
//             argrument_findNearest[i]->endIndex = endIndex_loop;
//             pthread_attr_t attr;
//             pthread_attr_init(&attr);
//             pthread_create(&threads_arr[i], &attr, find_nearest_point, argrument_findNearest[i]);

//             startIndex_loop = startIndex_loop + (NumPoints / num_threads);
//             endIndex_loop = endIndex_loop + (NumPoints / num_threads);
//         }
//     }

//     //Joining all the threads
//     for (int i = 0; i < num_threads; i++)
//     {
//         pthread_join(threads_arr[i], NULL);
//     }

//     //Finding the nearest point from the list of points obtained from subarrays which is stored in the argrument_findNearest
//     double distance = DBL_MAX;
//     double tmp_distance;
//     struct point_t NearestPoint;
//     for (int i = 0; i < num_threads; i++)
//     {
//         tmp_distance = find_distance(&SrcPoint, &(argrument_findNearest[i]->nearest_point));
//         if (tmp_distance < distance)
//         {
//             distance = tmp_distance;
//             NearestPoint.x = argrument_findNearest[i]->nearest_point.x;
//             NearestPoint.y = argrument_findNearest[i]->nearest_point.y;
//         }
//     }
//     t = clock() - t;
//     printf("The nearest point is: %d %d\n", NearestPoint.x, NearestPoint.y);
//     printf("Time taken: %f\n", (double)t / CLOCKS_PER_SEC);

//     //Sending output to output.txt
//     FILE *output_ptr = fopen("output.txt", "w");
//     fprintf(output_ptr, "%f seconds\n", (double)t / CLOCKS_PER_SEC);
//     fprintf(output_ptr, "(%d, %d)\n", NearestPoint.x, NearestPoint.y);
//     fclose(output_ptr);

//     return EXIT_SUCCESS;
// }

//This main function is for the report part of the Assignment
int main()
{
    srand(time(NULL));
    int num_threads = 1;

    /****************************************************************
     * This is the place where you need to make changes to size of array and number of threads
    */
    NumPoints = 5000;
    num_threads = 1;
    /****************************************************************
    */

    SrcPoint.x = rand() % MAX_ARR_SIZE;
    SrcPoint.y = rand() % MAX_ARR_SIZE;
    for (int i = 0; i < NumPoints; i++)
    {
        listPoints[i].x = rand() % MAX_ARR_SIZE;
        listPoints[i].y = rand() % MAX_ARR_SIZE;
    }
    clock_t t = clock();

    //Threads
    pthread_t threads_arr[MAX_THREAD];

    //Allocating memory for arguments struct array
    struct argument_multithreading_t *argrument_findNearest[MAX_THREAD];
    for (int i = 0; i < num_threads; i++)
    {
        argrument_findNearest[i] = malloc(sizeof(struct argument_multithreading_t));
    }

    //Initializing the startIndex and endIndex for subarrays
    int startIndex_loop = 0, endIndex_loop = NumPoints / num_threads - 1;
    for (int i = 0; i < num_threads; i++)
    {
        if (i == num_threads - 1)
        {
            argrument_findNearest[i]->startIndex = startIndex_loop;
            argrument_findNearest[i]->endIndex = NumPoints - 1;
            pthread_attr_t attr;
            pthread_attr_init(&attr);
            pthread_create(&threads_arr[i], &attr, find_nearest_point, argrument_findNearest[i]);
        }
        else
        {
            argrument_findNearest[i]->startIndex = startIndex_loop;
            argrument_findNearest[i]->endIndex = endIndex_loop;
            pthread_attr_t attr;
            pthread_attr_init(&attr);
            pthread_create(&threads_arr[i], &attr, find_nearest_point, argrument_findNearest[i]);

            startIndex_loop = startIndex_loop + (NumPoints / num_threads);
            endIndex_loop = endIndex_loop + (NumPoints / num_threads);
        }
    }

    //Joining all the threads
    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads_arr[i], NULL);
    }

    //Finding the nearest point from the list of points obtained from subarrays which is stored in the argrument_findNearest
    double distance = DBL_MAX;
    double tmp_distance;
    struct point_t NearestPoint;
    for (int i = 0; i < num_threads; i++)
    {
        tmp_distance = find_distance(&SrcPoint, &(argrument_findNearest[i]->nearest_point));
        if (tmp_distance < distance)
        {
            distance = tmp_distance;
            NearestPoint.x = argrument_findNearest[i]->nearest_point.x;
            NearestPoint.y = argrument_findNearest[i]->nearest_point.y;
        }
    }
    t = clock() - t;
    printf("The nearest point is: %d %d\n", NearestPoint.x, NearestPoint.y);
    printf("Time taken: %f\n", (double)t / CLOCKS_PER_SEC); //Time taken in seconds

    for (int i = 0; i < num_threads; i++)
    {
        free(argrument_findNearest[i]);
    }

    //Sending output to outputcsv.txt
    FILE *output_csv = fopen("outputcsv.txt", "a");
    fprintf(output_csv, "%d, %d, %f\n", NumPoints, num_threads, (double)t / CLOCKS_PER_SEC);

    return EXIT_SUCCESS;
}