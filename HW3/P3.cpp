/*
Student No.:
Student Name:
Email:
SE tag: xnxcxtxuxoxsx
Statement: I am fully aware that this program is not
supposed to be posted to a public server, such as a
public GitHub repository or a public web page.
*/
#include<iostream>
#include<pthread.h>
#include<semaphore.h>
#include<vector>
#include<queue>
#include<set>
#include<sys/time.h>
#include<fstream>
using namespace std;

int n;
vector<int> v;
vector<int> num;
bool task[15]={false};
vector<pair<int,int>> idx(15);
sem_t q_mutex;
sem_t num_task;
sem_t task_mutex;
sem_t finish;
int sem_value;
class protect_q{
    public:
        protect_q(){
            sem_init(&q_mutex,0,1);
        }
        void push(int num){
            sem_wait(&q_mutex);
            q.push(num);
            sem_post(&q_mutex);
        }
        int size(){
            sem_wait(&q_mutex);
            int ret=q.size();
            sem_post(&q_mutex);
            return ret;
        }
        int front(){
            sem_wait(&q_mutex);
            int ret=q.front();
            q.pop();
            sem_post(&q_mutex);
            return ret;
        }
    private:
        queue<int> q;
        sem_t q_mutex;
}q;
void buble_sort(int id){
    int l=idx[id].first;
    int r=idx[id].second;
    for(int i=l;i<=r;i++){
        for(int j=l;j<=r;j++){
            if(num[i]<num[j]){
                swap(num[i],num[j]);
            }
        }
    }
}
void merge_sort(int id){
    int ls=idx[id*2+1].first;
    int le=idx[id*2+1].second;
    int rs=idx[id*2+2].first;
    int re=idx[id*2+2].second;
    int l=ls,r=rs;
    vector<int> tmp;
    while(l<=le && r<=re){
        if(num[l]<num[r]){
            tmp.push_back(num[l++]);
        }
        else{
            tmp.push_back(num[r++]);
        }
    }
    while(l<=le){
        tmp.push_back(num[l++]);
    }
    while(r<=re){
        tmp.push_back(num[r++]);
    }
    for(int i=ls;i<=re;i++){
        num[i]=tmp[i-ls];
    }
}

void* worker(void* arg){
    while(1){
        sem_wait(&num_task); //wait for new task
        if(q.size()==0){
            break;
        }
        int task_id=q.front();
        if(task_id>6){
            buble_sort(task_id);
        }
        else{
            merge_sort(task_id);
        }
        // cout<<"task "<<task_id<<" finish"<<endl;
        sem_wait(&task_mutex); //task_mutex
        task[task_id]=true;
        sem_post(&task_mutex); 
        sem_post(&finish);  //notify dispacher
        // cout<<"worker "<<pthread_self()<<" finish"<<endl;
        // sem_getvalue(&num_task,&sem_value);
        // cout<<"worker "<<pthread_self()<<" "<<sem_value<<endl;
        // sem_wait(&num_task); //wait for new task
        // sem_getvalue(&num_task,&sem_value);
        // cout<<"worker "<<pthread_self()<<" "<<sem_value<<" get new task"<<endl;
        // cout<<"qsize "<<q.size()<<endl;
        // cout<<"worker "<<pthread_self()<<" waiting"<<endl;
        // cout<<sem_getvalue(&num_task,&)<<endl;

    }
    // cout<<"worker "<<pthread_self()<<" exit"<<endl;
    return nullptr;
}
void merge_index(int i,int l,int r,int time){
    idx[i]={l,r};
    if(time==3) return;
    int m=(l+r)/2;
    merge_index(i*2+1,l,m,time+1);
    merge_index(i*2+2,m,r,time+1);
}
int main(){
    //read file from input.txt
    ifstream fin("input.txt");
    int tmp;
    fin>>n;
    while(fin>>tmp){
        v.push_back(tmp);
    }
    merge_index(0,0,n-1,0);
    for(int numthread=1;numthread<=8;numthread++){
        num=v;
        struct timeval start,end;
        gettimeofday(&start, 0);
        ////////////////
        sem_init(&q_mutex,0,1);
        sem_init(&num_task,0,0);
        sem_init(&task_mutex,0,1);
        sem_init(&finish,0,0);
        pthread_t threads[numthread];
        set<int> s;
        for(int i=0;i<15;i++){
            task[i]=false;
        }
        for(int i=0;i<8;i++){
            q.push(i+7);
            s.insert(i+7);
            sem_post(&num_task);
        }
        // cout<<"start qsize "<<q.size()<<endl;
        for(int i=0;i<numthread;i++){
            pthread_create(&threads[i],NULL,worker,NULL);
        }
        while(1){
            sem_wait(&finish);
            sem_wait(&task_mutex);
            if(task[0]){
                sem_post(&task_mutex);
                for(int i=0;i<numthread;i++){
                    sem_post(&num_task);
                }
                for(int i=0;i<numthread;i++){
                    pthread_join(threads[i],NULL);
                }
                // std::cout<<"all task finish"<<endl;
                break;
            }
            sem_post(&task_mutex);  
            for(int i=0;i<7;i++){
                sem_wait(&task_mutex);
                if(!s.count(i) && task[2*i+1] && task[2*i+2]){
                    // cout<<"insert "<<i<<endl;
                    s.insert(i);
                    q.push(i);
                    sem_post(&num_task);
                }
                sem_post(&task_mutex);
            }
        }
        // for(auto& i:task){
        //     cout<<i<<" ";
        // }
        // cout<<endl;
        // for(auto& i:num){
        //     std::cout<<i<<" ";
        // }  
        // std::cout<<endl; 
        ////////////////
        gettimeofday(&end, 0);
        int sec = end.tv_sec - start.tv_sec;
        int usec = end.tv_usec - start.tv_usec;
        std::cout<<"worker thread #"<<numthread<<", elapsed "<<sec*1000+(usec/1000.0)<<" ms"<<endl;
        //write output array to output.txt
        ofstream fout("output_"+to_string(numthread)+".txt");
        for(auto& i:num){
            fout<<i<<" ";
        }
        fout<<endl;
    }
    sem_destroy(&q_mutex);
    sem_destroy(&num_task);
    sem_destroy(&task_mutex);
    sem_destroy(&finish);
    return 0;
}