#include <stdbool.h>
#include <stdio.h>
#define MAX 10
void calculateNeed(int need[MAX][MAX], int max[MAX][MAX], int allot[MAX][MAX],
                   int p, int r) {
  for (int i = 0; i < p; i++) {
    for (int j = 0; j < r; j++) {
      need[i][j] = max[i][j] - allot[i][j];
    }
  }
}
bool isSafe(int processes[], int avail[], int max[][MAX], int allot[][MAX],
            int p, int r) {
  int need[MAX][MAX];
  calculateNeed(need, max, allot, p, r);

  bool finish[MAX] = {false};
  int work[MAX];

  for (int i = 0; i < r; i++) {
    work[i] = avail[i];
  }
  int count = 0;
  while (count < p) {
    bool found = false;
    for (int i = 0; i < p; i++) {
      if (!finish[i]) {
        bool canProceed = true;
        for (int j = 0; j < r; j++) {
          if (need[i][j] > work[j]) {
            canProceed = false;
            break;
          }
        }
        if (canProceed) {
          for (int k = 0; k < r; k++) {
            work[k] += allot[i][k];
          }
          finish[i] = true;
          count++;
          found = true;
        }
      }
    }
    if (!found) {
      return false; // Not safe
    }
  }
  return true; // Safe
}
int main() {
  int p, r;
  printf("Enter the number of processes: ");
  scanf("%d", &p);
  printf("Enter the number of resources: ");
  scanf("%d", &r);

  int processes[p], avail[r], max[p][r], allot[p][r];
  printf("Enter available resources:\n");
  for (int i = 0; i < r; i++) {
    printf("Resource %d: ", i);
    scanf("%d", &avail[i]);
  }

  printf("Enter maximum demand matrix:\n");
  for (int i = 0; i < p; i++) {
    printf("Process %d: ", i);
    for (int j = 0; j < r; j++) {
      scanf("%d", &max[i][j]);
    }
  }
  printf("Enter allocation matrix:\n");
  for (int i = 0; i < p; i++) {
    printf("Process %d: ", i);
    for (int j = 0; j < r; j++) {
      scanf("%d", &allot[i][j]);
    }
  }
  if (isSafe(processes, avail, max, allot, p, r)) {
    printf("System is in a safe state.\n");
  } else {
    printf("System is not in a safe state.\n");
  }
  return 0;
}


