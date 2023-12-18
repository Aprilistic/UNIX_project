
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

void sortData(int data[]) {

	int buf[INT_COUNT];

    for (int i = 0; i < INT_COUNT; i += 1024) {
        int bufIndex = (i / 1024) % 4 * (INT_COUNT / 4);
        memcpy(&buf[bufIndex], &data[i], 1024 * sizeof(int));
    }

	memcpy(data, buf, DATA_SIZE);
}