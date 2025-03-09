/*
Student No.: 
Student Name: 
Email: 
SE tag: xnxcxtxuxoxsx
Statement: I am fully aware that this program is not
supposed to be posted to a public server, such as a
public GitHub repository or a public web page.
*/
#include <iostream>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/time.h>

using namespace std;

int main() {
    int n;
    scanf("%d",&n);
    unsigned int *matrix_a=new unsigned int[n*n];
    unsigned int *matrix_b=new unsigned int[n*n];
    int num = 0;
    for (int i=0;i<n*n;i++) {
        matrix_a[i]=num;
        matrix_b[i]=num;
        num++;
    }
    pid_t pid[16];
    for (int i=1;i<=16;i++) {
        unsigned int checksum=0;
        struct timeval start,end;
        gettimeofday(&start, 0);
        int mem_id = shmget(0|IPC_PRIVATE, sizeof(unsigned int)*n*n,IPC_CREAT|0666);
        unsigned int *shm_ptr = (unsigned int *)shmat(mem_id, NULL, 0);
        for (int j = 0; j < i; j++) {
            pid[j] = fork();
            if (pid[j]==0) {
                int start_row=j*(n/i);
                int end_row=(j==i-1)?n:(j+1)*(n/i);
                for(int k=start_row;k<end_row;k++) {
                    for (int l=0;l<n;l++) {
                        for (int m=0;m<n;m++) {
                            shm_ptr[k*n+l]+=matrix_a[k*n+m]*matrix_b[m*n+l];
                        }
                    }
                }
                exit(0);
            }
        }
        for (int j = 0; j < i; j++) wait(NULL);
        gettimeofday(&end, 0);
        int sec = end.tv_sec - start.tv_sec;
        int usec = end.tv_usec - start.tv_usec;
        for(int j = 0; j < n * n; j++) checksum += shm_ptr[j];
        printf("Multiplying matrices using %d process\n",i);
        printf("Elapsed %f sec, ", sec + (usec / 1000000.0));
        printf("Checksum: %u\n", checksum);
        shmdt(shm_ptr);
        shmctl(mem_id, IPC_RMID, NULL);
    }
    delete[] matrix_a;
    delete[] matrix_b;
    return 0;
}
