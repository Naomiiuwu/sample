#include <stdio.h>
#include <stdlib.h>
#define MAX 100

void sstf(int requests[], int n, int head) {
    int visited[MAX] = {0}, totalMovement = 0, count = 0;
    printf("\nSSTF Disk Scheduling\nHead Movement Sequence: %d", head);

    while (count < n) {
        int minDistance = 10000, nextIndex = -1;
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                int distance = abs(requests[i] - head);
                if (distance < minDistance) {
                    minDistance = distance;
                    nextIndex = i;
                }
            }
        }
        if (nextIndex != -1) {
            totalMovement += minDistance;
            head = requests[nextIndex];
            visited[nextIndex] = 1;
            printf(" -> %d", head);
            count++;
        }
    }
    printf("\nTotal Head Movement = %d\n", totalMovement);
}

void scan(int requests[], int n, int head, int diskSize, int direction) {
    int totalMovement = 0;
    requests[n] = 0;
    requests[n + 1] = diskSize - 1;
    n += 2;

    // Sort requests in ascending order
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (requests[j] > requests[j + 1]) {
                int temp = requests[j];
                requests[j] = requests[j + 1];
                requests[j + 1] = temp;
            }
        }
    }

    int position = 0;
    for (int i = 0; i < n; i++) {
        if (requests[i] >= head) {
            position = i;
            break;
        }
    }

    printf("\nSCAN Disk Scheduling\nHead Movement Sequence: %d", head);
    if (direction == 1) {
        for (int i = position; i < n; i++) {
            printf(" -> %d", requests[i]);
            totalMovement += abs(requests[i] - head);
            head = requests[i];
        }
        for (int i = position - 1; i >= 0; i--) {
            printf(" -> %d", requests[i]);
            totalMovement += abs(requests[i] - head);
            head = requests[i];
        }
    } else {
        for (int i = position - 1; i >= 0; i--) {
            printf(" -> %d", requests[i]);
            totalMovement += abs(requests[i] - head);
            head = requests[i];
        }
        for (int i = position; i < n; i++) {
            printf(" -> %d", requests[i]);
            totalMovement += abs(requests[i] - head);
            head = requests[i];
        }
    }
    printf("\nTotal Head Movement = %d\n", totalMovement);
}

void c_look(int requests[], int n, int head) {
    int totalMovement = 0;

    // Sort requests
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (requests[j] > requests[j + 1]) {
                int temp = requests[j];
                requests[j] = requests[j + 1];
                requests[j + 1] = temp;
            }
        }
    }

    int position = 0;
    for (int i = 0; i < n; i++) {
        if (requests[i] >= head) {
            position = i;
            break;
        }
    }

    printf("\nC-LOOK Disk Scheduling\nHead Movement Sequence: %d", head);
    for (int i = position; i < n; i++) {
        printf(" -> %d", requests[i]);
        totalMovement += abs(requests[i] - head);
        head = requests[i];
    }
    for (int i = 0; i < position; i++) {
        printf(" -> %d", requests[i]);
        totalMovement += abs(requests[i] - head);
        head = requests[i];
    }
    printf("\nTotal Head Movement = %d\n", totalMovement);
}

int main() {
    int requests[MAX], n, head, diskSize, direction, choice;
    printf("Enter number of requests: ");
    scanf("%d", &n);
    printf("Enter the requests: ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &requests[i]);
    }
    printf("Enter initial head position: ");
    scanf("%d", &head);
    printf("Enter disk size (for SCAN and C-LOOK): ");
    scanf("%d", &diskSize);

    while (1) {
        printf("\nChoose Disk Scheduling Algorithm:\n1. SSTF\n2. SCAN\n3. C-LOOK\n4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                sstf(requests, n, head);
                break;
            case 2:
                printf("Enter initial direction (1 for right, 0 for left): ");
                scanf("%d", &direction);
                scan(requests, n, head, diskSize, direction);
                break;
            case 3:
                c_look(requests, n, head);
                break;
            case 4:
                return 0;
            default:
                printf("Invalid choice!\n");
        }
    }
    return 0;
}


