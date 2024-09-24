# Steps to Run
Note that clean and build need to be run in /tmp due to the bug mentioned in lab1 instructions. 
## To clean:
    make clean

## To build:
    make <target>

\<target\> should be replaced by the target you would like to build. Here is the list of target available:
* q2
* q3
* q4
* q5

## To run the test program:
    make run

# Potential Issues
1.  The error below might be encountered if using mainframer to run this program. But the error will be gone if run on remote machine directly.
```shell
    stty: standard input: Inappropriate ioctl for device
    make: *** [run] Error 1
```
    


