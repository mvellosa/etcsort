#ifndef __ETCSORT_H
#define __ETCSORT_H

/**
 * << etcsort >> Header only version
 * A library for multiple types of generic sorting algorithms.
 * Usage folows the the stdlib's qsort()
 * Function prefix: etcsort_
 * 
 * @author MVellosa
 * @date 12/2021
 * @version 1.0
*/
#include <alloca.h>

typedef int (*__etccmpr)(const void *,const void *);
/* swap the values of 2 positions < x1 > and < x2 > of byte size < width > */
#define M__SWAP(x1, x2, width)                \
    do {                                      \
        size_t __width_ = (width);            \
        char *__bx1_ = (char *) (x1);         \
        char *__bx2_ = (char *) (x2);         \
        while(__width_-- > 0){                \
            char __tmp_ = *__bx1_;            \
            *__bx1_ = *__bx2_;                \
            *__bx2_ = __tmp_;                 \
            __bx1_++;                         \
            __bx2_++;                         \
        }                                     \
    } while(0)


/* copy < width > bytes from <src> to <dest>  */
#define M__COPY(dest, src, width)             \
    do {                                      \
        size_t __width_ = (width);            \
        char *__bdest_ = (char *) (dest);     \
        char *__bsrc_ = (char *) (src);       \
        while(__width_-- > 0){                \
            *__bdest_ = *__bsrc_;             \
            __bdest_++;                       \
            __bsrc_++;                        \
        }                                     \
    } while(0)


/**
 * << Bubble Sort >>
 * for every element at index < i >, check if the next element (index < i > + 1)
 * is bigger than itself, if it is swap the elements. 
 * Repeat untill no swap is made
 * 
 * Time Complexity: O(n^2)
 * Memory Complexity: O(1)
 * 
 * @param _elements array of elements to be sorted
 * @param _elnum number of elements in the array
 * @param bsize byte size of the elements in the array
 * @param elcmpr pointer to function to compare 2 elements
*/
void etcsort_bubble(void *_elements, size_t _elnum, size_t bsize, __etccmpr _elcmpr){
    int i = 1;
    int endflag = 0;
    while(! endflag){
        endflag = 1;
        for (int j = 0; j < _elnum - i; j++){
            void *current = _elements + (j * bsize);
            void *next = _elements + ((j + 1) * bsize);
            if (_elcmpr(current, next) > 0){
                M__SWAP(current, next, bsize);

                endflag = 0;
            }
        }
        i++;
    }
}

/**
 * << Insertion Sort >>
 * for every element at index < i >, compare it to the previous element (index < i > - 1)
 * if previous is smaller: go to the next element.
 * if previous is greater: swap current with previous until previous is smaller than the current.
 * 
 * Time Complexity: O(n^2)
 * Memory Complexity: O(1)
 * 
 * @param _elements array of elements to be sorted
 * @param _elnum number of elements in the array
 * @param bsize byte size of the elements in the array
 * @param elcmpr pointer to function to compare 2 elements
*/
void etcsort_insertion(void *_elements, size_t _elnum, size_t bsize, __etccmpr _elcmpr){

    for (int i = 1; i < _elnum; i++){

        if (_elcmpr(_elements + (i * bsize), _elements + ((i - 1) * bsize)) < 0){
            int j = i;
            do{
                M__SWAP(_elements + (j * bsize), _elements + ((j - 1) * bsize), bsize);

                j--;
            } while (j > 0 && _elcmpr(_elements + (j * bsize), _elements + ((j - 1) * bsize)) < 0);
        }
    }

}

static void __internal_merge_(void *_elements, size_t bsize, size_t start, size_t middle, size_t end, __etccmpr _elcmpr){
    size_t leftSize = middle - start + 1;
    size_t rightSize = end - middle;

    /* TODO get buffer by parameter so it can reutilize the same buffer */
    void *leftArr = malloc(leftSize * bsize);  
    void *rightArr = malloc(rightSize * bsize);

    for (int i = 0; i < leftSize; i++){
        M__COPY(leftArr + (i * bsize), _elements + ((i + start) * bsize), bsize);
    }
    for (int i = 0; i < rightSize; i++){
        M__COPY(rightArr + (i * bsize), _elements + ((i + middle + 1) * bsize), bsize);
    }

    size_t indexLeft = 0, indexRight = 0, indexMerged = start;

    while (indexLeft < leftSize && indexRight < rightSize){
        void *currentLeft = leftArr + (indexLeft * bsize);
        void *currentRight = rightArr + (indexRight * bsize);
        void *currentMerged = _elements + (indexMerged * bsize);

        if (_elcmpr(currentLeft, currentRight) < 0){
            M__COPY(currentMerged, currentLeft, bsize);
            indexLeft++;
        }else{
            M__COPY(currentMerged, currentRight, bsize);
            indexRight++;
        }
        indexMerged++;
    }

    while(indexLeft < leftSize){
        void *currentLeft = leftArr + (indexLeft * bsize);
        void *currentMerged = _elements + (indexMerged * bsize);

        M__COPY(currentMerged, currentLeft, bsize);

        indexLeft++;
        indexMerged++;
    }

    while(indexRight < rightSize){
        void *currentRight = rightArr + (indexRight * bsize);
        void *currentMerged = _elements + (indexMerged * bsize);
        
        M__COPY(currentMerged, currentRight, bsize);

        indexRight++;
        indexMerged++;
    }
    free(leftArr);
    free(rightArr);
}

static void __internal_mergeSort_(void *_elements, size_t bsize, size_t start, size_t end, __etccmpr _elcmpr){
    if(start < end){
        size_t middle = start + (end - start) / 2;
        __internal_mergeSort_(_elements, bsize, start, middle, _elcmpr);
        __internal_mergeSort_(_elements, bsize, middle + 1, end, _elcmpr);

        __internal_merge_(_elements, bsize, start, middle, end, _elcmpr);
    }
}

/**
 * << Merge Sort >>
 * Divide the array in two parts (left and right) keep dividing until an array
 * of size 1 is reached, when this happens recursively go back merging the 
 * arrays created.
 * merge: compare the elements from the left and right array and rearrange the
 * merged array in order
 * 
 * Time Complexity: O(n log(n))
 * Memory Complexity: O(n)
 * 
 * @param _elements array of elements to be sorted
 * @param _elnum number of elements in the array
 * @param bsize byte size of the elements in the array
 * @param elcmpr pointer to function to compare 2 elements
*/
void etcsort_merge(void *_elements, size_t _elnum, size_t bsize, __etccmpr _elcmpr){
    __internal_mergeSort_(_elements, bsize, 0, _elnum - 1, _elcmpr);
}


#endif
