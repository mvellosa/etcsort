# etcsort

A library for multiple types of generic sorting algorithms

____________________

This project was made as a learning experiment, therefore there are some optimizations that could be made but which would add undesired complexity to the project.

## Basic example
____________________

```c
#include "etcsort.h"

int mycmp(const void *x1, const void *x2){
    const int *ix1 = (const int *) x1;
    const int *ix2 = (const int *) x2;
    return *ix1 - *ix2;
}

int main(void){
    int arr[] = {15, 2, 5, 13, 5, 22, 2, 6};

    etcsort_merge(arr, numel, sizeof(int), mycmp);
}
```
____________________
