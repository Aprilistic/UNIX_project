
#include "mytest.h"

// quick sort
void arrange(int *data, int start, int end) {
    int left = start, right = end;
    int pivot = data[(left + right) / 2];

    int tmp;
    
    do {
        while (data[left] < pivot) left++;
        while (data[right] > pivot) right--;

        if (left <= right) {
            tmp = data[left];
            data[left] = data[right];
            data[right] = tmp;

            left++;
            right--;
        }
    } while (left <= right);

    if (start < right) arrange(data, start, right);
    if (left < end) arrange(data, left, end);
}