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
#include<sstream>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<signal.h>
#include<vector>
#include<string>
#include<sys/wait.h>
#include<cstring>

using namespace std;

int main(){
    signal(SIGCHLD, SIG_IGN);
    while(1){
        string input;
        stringstream ss;
        vector<char*> argv;
        vector<char*> argv_2;
        string file_name="";
        int p=0;
        int push=1;
        int mode=0;
        int cont=0;
        cout<<">";
        getline(cin,input);
        ss << input;
        while(ss>>input){
            if(input=="&") cont=1;
            else if(input=="<" && argv.size()){
                file_name=argv.back();
                argv.pop_back();
                mode=1;
            }
            else if(input==">"){
                push=0;
                mode=2;
            }
            else if(push==0) file_name=input;
            else if(input=="|"){
                p=1;
                mode=3;
            }
            else if(p==0){
                char* arg = strdup(input.c_str());
                argv.push_back(arg);
            }
            else{
                char* arg = strdup(input.c_str());
                argv_2.push_back(arg);
            }
        }
        argv.push_back(NULL);
        argv_2.push_back(NULL);
        if(mode==0){
            pid_t child_pid;
            child_pid = fork();
            if(!child_pid) execvp(argv[0],argv.data());
            else if(child_pid>0) {if(!cont) wait(NULL);}
            else{
                cerr<<"Fork fail!"<< endl;
                return 1;
            }
        }
        else if(mode==1){
            pid_t child_pid;
            child_pid=fork();
            if (!child_pid) {
                int fd=open(file_name.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
                if (fd!=-1) {
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }
                execvp(argv[0], argv.data());
            }
            else if (child_pid>0) wait(NULL);
            else {
                cerr<<"Fork fail!"<<endl;
                return 1;
            }
        }
        else if(mode==2){
            pid_t child_pid;
            child_pid = fork();
            if (!child_pid) {
                int fd = open(file_name.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
                if (fd != -1) {
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }
                execvp(argv[0], argv.data());
            }
            else if (child_pid > 0) wait(NULL);
            else {
                cerr << "Fork fail!" << endl;
                return 1;
            }
        }
        else if(mode==3){
            int pipefd[2];
            if (pipe(pipefd) == -1) {
                cerr << "Pipe creation failed!" << endl;
                return 1;
            }
            pid_t cmd1_pid=fork();
            if(cmd1_pid<0) cerr<<"Fork fail!"<<endl;
            else if(cmd1_pid==0){
                close(pipefd[0]);
                dup2(pipefd[1],STDOUT_FILENO);
                close(pipefd[1]);
                execvp(argv[0],argv.data());
            }
            pid_t cmd2_pid=fork();
            if(cmd2_pid<0) cerr<<"Fork fail!"<<endl;
            else if(cmd2_pid==0){
                close(pipefd[1]);
                dup2(pipefd[0],STDIN_FILENO);
                close(pipefd[0]);
                execvp(argv_2[0],argv_2.data());
            }
            close(pipefd[0]);
            close(pipefd[1]);
            waitpid(cmd1_pid,NULL,0);
            waitpid(cmd2_pid,NULL,0);
        }
    }
    return 0;
}