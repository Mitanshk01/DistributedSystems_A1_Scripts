# Distributed Systems Assignment-1 Test Scripts

## Overview

This repository contains scripts and test cases for Assignment-1 of the Distributed Systems course. Each assignment has a structured folder containing input/output test cases, a shell script to run the tests, and a `marks.txt` file indicating the marks for each test case.

## Directory Structure

```
scripts/
   1/
       testcases/
           1.in
           1.out
       1_test.sh
       marks.txt
   2/
       testcases/
           1.in
           1.out
           2.in
           2.out
       2_test.sh
       marks.txt
   3/
       testcases/
           1.in
           2.in
           a.txt
```

### Description

- **`testcases/`**: Contains input (`*.in`) and expected output (`*.out`) files for each test case. Currently the testcases provided are only the samples.
- **`*_test.sh`**: Bash script for compiling, running, and validating the program against the test cases. The script also calculates the total marks based on the results.
- **`marks.txt`**: Lists the marks assigned to each test case in the following format (Can be ignored):

  ```
  1 10
  2 15
  3 20
  ...
  ```

  Each line represents the test case number and the corresponding marks.

- **`Note`**: For Problem 3.3, the script has not been added yet. You can test Problems 3.1 and 3.2 for now.

## Running Instructions

1. **Copy the scripts directory to your root folder(RollNumber/)**:

    ```bash
   cp -r /path/to/scripts RollNumber/
   ```

2. **Navigate to the script directory**:
   
   Change to the directory of the assignment you want to run. For example, to run tests for 3.1:

   ```bash
   cd scripts/1
   ```

3. **Run the test script**:

   Execute the provided shell script to compile and run the test cases:

   ```bash
   chmod +x 1_test.sh
   ./1_test.sh
   ```

4. **View the results**:

   The script will display the result of each test case (`PASSED` or `FAILED`) and the total marks scored. Example output:

   ```
   Test case 1: PASSED
   Test case 2: FAILED
   ...
   Final Score: 50/100
   ```

5. **Cleaning Up**:

   Temporary files generated during the execution are managed by the script. If needed, you can manually clean them by removing the `results/` directory:

   ```bash
   rm -rf results/
   ```

## Notes

- Ensure that `mpic++` and `mpiexec` are installed and available in your environment.
- The scripts assume that the code to be tested is located in a specific directory relative to the `*_test.sh` script. Make sure to adjust the paths accordingly if the structure changes.
- The test scripts handle whitespace normalization and newline removals to ensure consistent comparison of outputs.
