#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAGIC_NUM   9
#define DEBUG       1

int verbose = 1;
#ifdef DEBUG

#define SHOW_ERROR 1
#define SHOW_WARNING 2
#define SHOW_DEBUG   4

#define ENTER(...)    do {\
                            printf("Line: %d, %s(): ENTER(", __LINE__, __FUNCTION__);\
                            printf(__VA_ARGS__);\
                            printf(")\n");\
                       } while(0)
#define EXIT(...)    do {\
                            printf("Line: %d, %s(): EXIT(", __LINE__, __FUNCTION__);\
                            printf(__VA_ARGS__);\
                            printf(")\n");\
                         } while(0)
#define D(...)\
if(verbose & SHOW_DEBUG) {\
    do {\
        printf("Debug : Line: %d, %s(): ", __LINE__, __FUNCTION__);\
        printf(__VA_ARGS__);\
        printf("\n");\
    } while (0);\
}

#define W(...)\
if(verbose & SHOW_WARNING) {\
    do {\
        printf("Warning : Line: %d, %s(): ", __LINE__, __FUNCTION__);\
        printf(__VA_ARGS__);\
        printf("\n");\
    } while (0);\
}

#define E(...)\
if(verbose & SHOW_ERROR) {\
    do {\
        printf("Error : Line: %d, %s(): ", __LINE__, __FUNCTION__);\
        printf(__VA_ARGS__);\
        printf("\n");\
    } while (0);\
}
#else

#define ENTER(...)
#define EXIT(...)
#define D(...)
#define E(...)
#define W(...)

#endif

/* ==> Copyright: https://www.geeksforgeeks.org/merge-sort/ */
// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(int * arr, int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
 
    // Create temp arrays
    int L[n1], R[n2];
 
    // Copy data to temp arrays L[] and R[]
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
 
    // Merge the temp arrays back into arr[l..r
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    // Copy the remaining elements of L[],
    // if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
 
    // Copy the remaining elements of R[],
    // if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}
 
// l is for left index and r is right index of the
// sub-array of arr to be sorted
void mergeSort(int * arr, int l, int r)
{
    if (l < r) {
        int m = l + (r - l) / 2;
 
        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
 
        merge(arr, l, m, r);
    }
}
/* ==> Copyright: https://www.geeksforgeeks.org/merge-sort/ */

void print_array(int * a, int size)
{
    if(verbose & SHOW_DEBUG) {
        printf("[");
        for (int i = 0; i < size; ++i)
            printf("%d, ", a[i]);
            
        printf("]\n");
    }
}

bool duplicates_found(int * a, int size)
{
    int ret = false;
    ENTER("%d", size);
    for (int i = 0; i < (size - 1); ++i) {
        
        if (a[i] == a[i + 1]) {
            ret = true;
            break;
        }
    }
        
    EXIT("%d", ret);
    return ret;
}

bool is_correct_solution(int grid[MAGIC_NUM][MAGIC_NUM], int size, int size_m)
{
    ENTER("%d", size);
    bool ret = true;
    int * temp = NULL;
    
    if (!size || size != MAGIC_NUM || !grid) {
        E("Invalid matrix");
        ret = false;
        goto out;
    }

    //XXX: ==> Dynamic Allocation that needs to be freed in main()
    temp = (int *)calloc(size_m * sizeof(int), 0);
    if (!temp) {
        E("Error mallocing memory");
        ret = false;
        goto out;
    }
 
    // 1. Check if rows have duplicates
    for (int i = 0; i < size; ++i) {
        memcpy((void *)temp, (void *)grid[i], sizeof(int) * MAGIC_NUM);
        print_array(temp, size_m);
        mergeSort(temp, 0, size_m - 1);
        print_array(temp, size_m);
        D("********");
        if (duplicates_found(temp, size_m)) {
            ret = false;
            W("Duplicates found\n");
            goto out;
        }
    }
    
    // 2. Check if columns have duplicates
    for (int j = 0; j < size_m; ++j) {
        for (int i = 0; i < size; ++i) {
            temp[i] = grid[i][j];
        }
        print_array(temp, size_m);
        mergeSort(temp, 0, size_m - 1);
        print_array(temp, size_m);
        D("********");
        if (duplicates_found(temp, size_m)) {
            W("Duplicates found");
            ret = false;
            goto out;
        }
    }
    
    // 3. Check if 3x3 matrices have duplicates
    for (int i = 0, j = 0, count = 0, idx = 0; i < size; ++i) {
        for (; j < size_m;) {
            temp[idx++] = grid[i][j];
            temp[idx++] = grid[i][j + 1];
            temp[idx++] = grid[i][j + 2];
            break;
        }
         
        if ((i + 1) % 3 == 0) {
            print_array(temp, size_m);
            mergeSort(temp, 0, size - 1);
            print_array(temp, size_m);
            D("********");
            if (duplicates_found(temp, size_m)) {
                W("Duplicates found");
                ret = false;
                goto out;
            }
            idx = 0;
            ++count;
            D("count %d", count);
        }
        if ((i == (size - 1)) && (count < (MAGIC_NUM - 1))) {
            D("count %d - i %d - j %d", count, i , j);
           i = -1;
           j += 3;
        }
    }

out:
    if (temp)
        free(temp);  
    EXIT("%d", ret);
    return ret;
}

/*

Test input:

true
[[1,3,2,5,4,6,9,8,7], 
 [4,6,5,8,7,9,3,2,1], 
 [7,9,8,2,1,3,6,5,4], 
 [9,2,1,4,3,5,8,7,6], 
 [3,5,4,7,6,8,2,1,9], 
 [6,8,7,1,9,2,5,4,3], 
 [5,7,6,9,8,1,4,3,2], 
 [2,4,3,6,5,7,1,9,8], 
 [8,1,9,3,2,4,7,6,5]]

false
 [[1,3,4,2,5,6,9,8,7], 
 [4,6,8,5,7,9,3,2,1], 
 [7,9,2,8,1,3,6,5,4], 
 [9,2,3,1,4,5,8,7,6], 
 [3,5,7,4,6,8,2,1,9], 
 [6,8,1,7,9,2,5,4,3], 
 [5,7,6,9,8,1,4,3,2], 
 [2,4,5,6,3,7,1,9,8], 
 [8,1,9,3,2,4,7,6,5]]

false
 [[1,2,3,4,5,6,7,8,9], 
 [1,2,3,4,5,6,7,8,9], 
 [1,2,3,4,5,6,7,8,9], 
 [1,2,3,4,5,6,7,8,9], 
 [1,2,3,4,5,6,7,8,9], 
 [1,2,3,4,5,6,7,8,9], 
 [1,2,3,4,5,6,7,8,9], 
 [1,2,3,4,5,6,7,8,9], 
 [1,2,3,4,5,6,7,8,9]]

false
 [[8,3,6,5,3,6,7,2,9], 
 [4,2,5,8,7,9,3,8,1], 
 [7,9,1,2,1,4,6,5,4], 
 [9,2,1,4,3,5,8,7,6], 
 [3,5,4,7,6,8,2,1,9], 
 [6,8,7,1,9,2,5,4,3], 
 [5,7,6,9,8,1,4,3,2], 
 [2,4,3,6,5,7,1,9,8], 
 [8,1,9,3,2,4,7,6,5]]

false
 [[1,3,2,5,4,6,9,8,7], 
 [4,6,5,8,7,9,3,2,1], 
 [7,9,8,2,1,3,6,5,4], 
 [9,2,1,4,3,5,8,7,6], 
 [3,5,4,7,6,8,2,1,9], 
 [6,8,7,1,9,2,5,4,3], 
 [5,4,6,9,8,1,4,3,2], 
 [2,7,3,6,5,7,1,9,8], 
 [8,1,9,3,2,4,7,6,5]]

false
 [[1,2,3,4,5,6,7,8,9], 
 [4,6,5,8,7,9,3,2,1], 
 [7,9,8,2,1,3,6,5,4], 
 [1,2,3,4,5,6,7,8,9], 
 [4,6,5,8,7,9,3,2,1], 
 [7,9,8,2,1,3,6,5,4], 
 [1,2,3,4,5,6,7,8,9], 
 [4,6,5,8,7,9,3,2,1], 
 [7,9,8,2,1,3,6,5,4]]

false
 [[5,3,4,6,7,8,9,1,2], 
 [6,7,2,1,9,5,3,4,8], 
 [1,9,8,3,4,2,5,6,7], 
 [8,5,9,9,6,1,4,2,3], 
 [4,2,6,8,5,3,7,9,1], 
 [7,1,3,7,2,4,8,5,6], 
 [9,6,1,5,3,7,2,8,4], 
 [2,8,7,4,1,9,6,3,5], 
 [3,4,5,2,8,6,1,7,9]]

false
 [[5,5,5,5,5,5,5,5,5], 
 [5,5,5,5,5,5,5,5,5], 
 [5,5,5,5,5,5,5,5,5], 
 [5,5,5,5,5,5,5,5,5], 
 [5,5,5,5,5,5,5,5,5], 
 [5,5,5,5,5,5,5,5,5], 
 [5,5,5,5,5,5,5,5,5], 
 [5,5,5,5,5,5,5,5,5], 
 [5,5,5,5,5,5,5,5,5]]

false
 [[5,3,4,6,7,8,9,1,2], 
 [6,7,2,3,9,5,3,4,8], 
 [1,9,8,1,4,2,5,6,7], 
 [8,5,9,7,6,1,4,2,3], 
 [4,2,6,8,5,3,7,9,1], 
 [7,1,3,9,2,4,8,5,6], 
 [9,6,1,5,3,7,2,8,4], 
 [2,8,7,4,1,9,6,3,5], 
 [3,4,5,2,8,6,1,7,9]]

false
 [[5,3,4,6,7,8,9,1,2], 
 [6,7,2,1,9,5,3,4,8], 
 [1,9,8,3,4,2,5,6,7], 
 [8,5,9,7,6,1,4,2,3], 
 [4,2,6,8,5,3,7,9,1], 
 [7,1,3,9,2,4,8,5,6], 
 [9,6,1,5,3,7,2,8,4], 
 [2,5,7,4,1,9,6,3,5], 
 [3,4,5,2,8,6,1,7,9]]

false
 [[1,2,3,4,5,6,7,8,9], 
 [4,5,6,7,8,9,1,2,3], 
 [7,8,9,1,2,3,4,5,6], 
 [2,3,4,5,6,7,8,9,1], 
 [3,4,5,6,7,8,9,1,2], 
 [5,6,7,8,9,1,2,3,4], 
 [6,7,8,9,1,2,3,4,5], 
 [8,9,1,2,3,4,5,6,7], 
 [9,1,2,3,4,5,6,7,8]]

*/
/*************************************/
/*              MAIN                 */
/*************************************/
int main(void)
{
    printf("HELLO SUDOKO WORLD\n");
    bool actual_result = false, expected_result = true;

    int input_puzzle[MAGIC_NUM][MAGIC_NUM] = {
                                     {1,3,2,5,4,6,9,8,7}, 
                                     {4,6,5,8,7,9,3,2,1}, 
                                     {7,9,8,2,1,3,6,5,4}, 
                                     {9,2,1,4,3,5,8,7,6}, 
                                     {3,5,4,7,6,8,2,1,9}, 
                                     {6,8,7,1,9,2,5,4,3}, 
                                     {5,7,6,9,8,1,4,3,2}, 
                                     {2,4,3,6,5,7,1,9,8}, 
                                     {8,1,9,3,2,4,7,6,5}
                                  };

    printf("Test: ");
    actual_result = is_correct_solution(input_puzzle, MAGIC_NUM, MAGIC_NUM);
    if (actual_result != expected_result)
        printf("FAIL\n");
    else
        printf("PASS\n");

    printf("GDBYE SUDOKO WORLD\n");
    return actual_result;
}