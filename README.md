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

For instance, when we would like to build q5:
```shell
make q5
```

Note that since all targets are sharing the same bin folder. Ensure to clean before build.


## To run the test program:
### Run with q5 built

Run with default input values:
```shell
make run_q5
```
Input arguments NUM_N, NUM_O and T (temperature) are available for run_q5. To change these inputs:
```shell
make run_q5 NUM_N=16 NUM_O=32 T=100
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

2. The "Usage" error below usually shows up when there is a input argument mismatch. To get rid of this error, ensure that "make clean" is run before building and running a new target.
```shell
    Usage: makeprocs.dlx.obj <number of processes to create>
    No runnable processes - exiting!
```