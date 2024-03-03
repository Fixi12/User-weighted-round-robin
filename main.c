#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_USERS 10
#define MAX_PROCESSES 5
#define MAX_NAME_LENGTH 50

int USERS_NUMBER;

typedef struct Process Process;

typedef struct Process{
    int burst_time;
    char process_name[MAX_NAME_LENGTH];
    Process * next;
};

typedef struct{
    Process* processes;
    char user_name[MAX_NAME_LENGTH];
    int weight;
    int processes_number;
}User;

typedef struct{
    Process* frontt;
    Process* rear;
}Queue;

Process* createProces(Process proces) {
    Process* newNode = (Process*)malloc(sizeof(Process));
    if (newNode == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }
    newNode->burst_time = proces.burst_time;
    strcpy(newNode->process_name,proces.process_name);
    newNode->next = NULL;
    return newNode;
}

int isEmpty(Queue q) {
    return (q.frontt == NULL);
}

void runRoundRobin(User user) {
    printf("User name: %s\n", user.user_name);

    Queue q;
    q.frontt = q.rear =NULL;

    for(int i=0;i < user.processes_number; i++)
    {
        Process* newProces=createProces(user.processes[i]);
        if(isEmpty(q))
        {
            q.frontt = q.rear = newProces;
        }
        else{
            q.rear->next=newProces;
            q.rear=newProces;
        }
    }
    int s=0;
    int f=0;
    while(q.frontt!=NULL)
    {
      if(q.frontt->burst_time>user.weight)
      {
        s=f;
        f+=user.weight;
        q.frontt->burst_time=q.frontt->burst_time - user.weight;
        printf("Procesul %s incepe la %d si se termina la %d\n",q.frontt->process_name,s,f);
        s=f;
        
        Process* newProces=createProces(*q.frontt);
        if(isEmpty(q))
        {
            q.frontt = q.rear =newProces;
        }
        else{
            q.rear->next=newProces;
            q.rear=newProces;
        }


        
      }
      else{
        s=f;
        f+=q.frontt->burst_time;
        printf("Procesul %s incepe la %d si se termina la %d\n",q.frontt->process_name,s,f);
        s=f;

      }
      q.frontt=q.frontt->next;
    }

    while(q.frontt!=NULL)
    {
        printf("%s cu %d",q.frontt->process_name,q.frontt->burst_time);
        q.frontt=q.frontt->next;
    }
}

void readUsersInfo() {
    printf("Type the number of users: ");
    scanf("%d", &USERS_NUMBER);

    User* Users = malloc(USERS_NUMBER * sizeof(User));
    if (Users == NULL) {
        fprintf(stderr, "Memory allocation failed for Users.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < USERS_NUMBER; ++i) {
        printf("Type a name (< 50 chr) for the user with id %d: ", i + 1);
        scanf("%s", Users[i].user_name);

        printf("Type the weight for the user with id %d: ", i + 1);
        scanf("%d", &Users[i].weight);

        printf("Type the processes number for the user with id %d: ", i + 1);
        scanf("%d", &Users[i].processes_number);

        Users[i].processes = malloc(Users[i].processes_number * sizeof(Process));
        if (Users[i].processes == NULL) {
            fprintf(stderr, "Memory allocation failed for Processes of User %s.\n", Users[i].user_name);
            exit(EXIT_FAILURE);
        }

        for (int j = 0; j < Users[i].processes_number; ++j) {
            printf("Type the name (< 50 chr) for the process with id %d of user %s: ", j + 1, Users[i].user_name);
            scanf("%s", Users[i].processes[j].process_name);

            printf("Type the burst time for the process with id %d of user %s: ", j + 1, Users[i].user_name);
            scanf("%d", &Users[i].processes[j].burst_time);
        }
    }

    for (int i = 0; i < USERS_NUMBER; ++i) {
        runRoundRobin(Users[i]);
    }

    for (int i = 0; i < USERS_NUMBER; ++i) {
        free(Users[i].processes);
    }

    free(Users);
}

int main(void) {
    readUsersInfo();
    exit(0);
}
