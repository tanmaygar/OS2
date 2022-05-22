# Operating System 2 Programming Assignment 5
## Tanmay Garg CS20BTECH11063

- There are 5 files in the folder
    - Assn5_rw_CS20BTECH11063.cpp
    - Assn5_frw_CS20BTECH11063.cpp
    - Assn5_Readme_CS20BTECH11063.txt
    - Assn5_Report_CS20BTECH11063.pdf
    - inp_params.txt

- To compile and run the RW Source Code

```
$ g++ -pthread -o RW Assn5_rw_CS20BTECH11063.cpp
$ ./RW
```
- To compile and run the Fair RW Source Code

```
$ g++ -pthread -o FRW Assn5_frw_CS20BTECH11063.cpp
$ ./FRW
```

- Please make sure that the correct input file is given in the source code (Mentioned in Comments)

- Please make sure that the input is like the one given in sample input file

- To run an executable multiple times
```
$ for i in {1..<number of times>} ; do ./<name of executable> ; done

```
- for RW the logs are printed in ```RW_log.txt```
- for Fair RW the logs are printed in ```FRW_log.txt```
- Average Time statistics are printed in ```AverageTime.txt```
- For the report part a separate file is created which outputs the Average Waiting Time and Worst Waiting time for FRW and RW, the files are ```Report.txt ``` and ```Report_FRW.txt```