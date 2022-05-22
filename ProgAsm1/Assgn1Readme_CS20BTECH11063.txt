# Operating System 2 Programming Assignment 1
## Tanmay Garg CS20BTECH11063

- There are 4 files in the folder
    - Assgn1Src_CS20BTECH11063.c
    - Assgn1Readme_CS20BTECH11063.txt
    - Assgn1Report_CS20BTECH11063.pdf
    - input.txt

- To compile and run the program enter the following commands in terminal or powershell window

```
$ gcc -pthread -o final_exec Assgn1Src_CS20BTECH11063.c -lm
$ ./final_exec
```
- To run only the input/output part of the assignment, comment the second ``` main ``` function
- To run the report part of the assignment, comment the first ``` main ``` function, then uncomment the second ``` main ``` function and change the **Number of Points** and **Number of Threads** in the source code
- To run the report part for instance 100 time to test for a partcicular size of array and number of threads then run the following code in bash or zsh
```
$ for i in {1..100} ; do ./final_exec ; done
```
- The above shell script might not give different output in terms of the closest point due to ``` rand() ``` so a modified script can be used which will increase the time it takes to run it those many times
```
$ for i in {1..100} ; do ./final_exec && sleep 0.8; done
```

- Please give the input as mentioned in ``` input.txt ```