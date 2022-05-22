# Operating System 2 Programming Assignment 4
## Tanmay Garg CS20BTECH11063

- There are 6 files in the folder
    - SrcAssgn4_CAS_CS20BTECH11063.cpp
    - SrcAssgn4_TAS_CS20BTECH11063.cpp
    - SrcAssgn4_BCAS_CS20BTECH11063.cpp
    - Assgn4_Readme_CS20BTECH11063.txt
    - Assgn4_Report_CS20BTECH11063.pdf
    - inp_params.txt

- To compile and run the CAS Source Code

```
$ g++ -pthread -o cas SrcAssgn4_CAS_CS20BTECH11063.cpp
$ ./cas
```
- To compile and run the TAS Source Code

```
$ g++ -pthread -o tas SrcAssgn4_TAS_CS20BTECH11063.cpp
$ ./tas
```
- To compile and run the BCAS Source Code

```
$ g++ -pthread -o bcas SrcAssgn4_BCAS_CS20BTECH11063.cpp
$ ./bcas

```
- Please make sure that the correct input file is given in the source code (Mentioned in Comments)

- Please make sure that the input is like the one given in sample input file

- To run an executable multiple times
```
$ for i in {1..<number of times>} ; do ./<name of executable> ; done

```
- Output of logs will be in ```output_log.txt```
- Output of Statistics will be in ```output_stats.txt```
- For running the report part make sure to comment the file output linesin TestCS function to avoid overconsumption of memory to print logs