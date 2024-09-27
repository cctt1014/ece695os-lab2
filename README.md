# Steps to Run
Note that clean and build need to be run in /tmp due to the bug mentioned in lab1 instructions. 
## To clean:
```shell
make clean
```

## To build:
```shell
make <target>
```

\<target\> should be replaced by the target you would like to build. Here is the list of target available:
* q2
* q3
* q4
* q5

## To run the test program:
### Run with q5 built

Run with default input values:
```shell
make run_q5
```
Input arguments NUM_N, NUM_O and TEMPERATURE are available for run_q5. To change these inputs:
```shell
make run_q5 NUM_N=16 NUM_O=32 TEMPERATURE=100
```

### Run with any other targets
Run with default input values:
```shell
make run
```
Input arguments NUM_PC_PAIR is available for q2/q3/q4. To change the input:
```shell
make run NUM_PC_PAIR=5
```


# Potential Issues
1.  The error below might be encountered if using mainframer to run this program. But the error will be gone if run on remote machine directly.
```shell
    stty: standard input: Inappropriate ioctl for device
    make: *** [run] Error 1
```
    


