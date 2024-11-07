#include <stdio.h>
#include <limits.h>

// Structure for process
typedef struct {
    int id, arrival_time, burst_time, remaining_time, completion_time;
} Process;

// Function for SJF Preemptive
void sjf_preemptive(Process p[], int n) {
    int completed = 0, time = 0, min_rem_time = INT_MAX, shortest = -1;
    int is_process_executing = 0;

    while (completed != n) {
        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= time && p[i].remaining_time > 0 && p[i].remaining_time < min_rem_time) {
                min_rem_time = p[i].remaining_time;
                shortest = i;
                is_process_executing = 1;
            }
        }
       
        if (is_process_executing == 0) {
            time++;
            continue;
        }
       
        p[shortest].remaining_time--;
        min_rem_time = p[shortest].remaining_time;

        if (p[shortest].remaining_time == 0) {
            p[shortest].completion_time = ++time;
            completed++;
            min_rem_time = INT_MAX;
            is_process_executing = 0;
        } else {
            time++;
        }
    }

    printf("\nSJF Preemptive Scheduling:\n");
    for (int i = 0; i < n; i++) {
        printf("Process %d - Completion Time: %d\n", p[i].id, p[i].completion_time);
    }
}

// Function for Round Robin
void round_robin(Process p[], int n, int quantum) {
    int time = 0, completed = 0;
    int queue[n], front = 0, rear = 0;
   
    for (int i = 0; i < n; i++) queue[rear++] = i;
   
    while (completed < n) {
        int current = queue[front++];
       
        if (p[current].remaining_time > 0 && p[current].arrival_time <= time) {
            int exec_time = (p[current].remaining_time > quantum) ? quantum : p[current].remaining_time;
            p[current].remaining_time -= exec_time;
            time += exec_time;
           
            if (p[current].remaining_time == 0) {
                p[current].completion_time = time;
                completed++;
            } else {
                queue[rear++] = current;
            }
        }
    }

    printf("\nRound Robin Scheduling:\n");
    for (int i = 0; i < n; i++) {
        printf("Process %d - Completion Time: %d\n", p[i].id, p[i].completion_time);
    }
}

int main() {
    int n, quantum;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    Process p[n];

    printf("Enter time quantum for Round Robin: ");
    scanf("%d", &quantum);

    printf("Enter arrival and burst times:\n");
    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        printf("Process %d Arrival Time: ", p[i].id);
        scanf("%d", &p[i].arrival_time);
        printf("Process %d Burst Time: ", p[i].id);
        scanf("%d", &p[i].burst_time);
        p[i].remaining_time = p[i].burst_time;
    }

    sjf_preemptive(p, n);
    for (int i = 0; i < n; i++) p[i].remaining_time = p[i].burst_time; // Reset for RR
    round_robin(p, n, quantum);

    return 0;
}
