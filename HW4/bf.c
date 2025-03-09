/*
Student No.: 
Student Name: 
Email: 
SE tag: xnxcxtxuxoxsx
Statement: I am fully aware that this program is not
supposed to be posted to a public server, such as a
public GitHub repository or a public web page.
*/
#include<stdio.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<unistd.h>
#include<string.h>


struct block {
    size_t size;
    int free;
    struct block *prev;
    struct block *next;
};

struct block *head = NULL;

//debug func
// void print(){
//     struct block *cur=head;
//     while(cur){
//         char buffer[50];
//         sprintf(buffer,"Is Free? %d,Chunk size:%zu\n",cur->free,cur->size);
//         write(1,buffer,strlen(buffer));
//         cur=cur->next;
//     }
// }
void* malloc(size_t s){
    if(s==0){
        ////debug
        // print();
        ////
        struct block *cur=head;
        cur=head;
        size_t Max=0;
        while(cur!=NULL){
            if(cur->free==1 && cur->size>Max){
                Max=cur->size;
            }
            cur=cur->next;
        }
        // char buffer[50];
        // sprintf(buffer,"Max Free Chunk Size = %zu bytes\n",Max);
        // write(1,buffer,strlen(buffer));
        return NULL;
    }
    if(head==NULL){
        head=mmap(NULL,20000,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANON,-1,0);
        if(head==MAP_FAILED){
            return NULL;
        }
        head->size=20000-sizeof(struct block);
        head->free=1;
        head->prev=NULL;
        head->next=NULL;
        //print memory pool size
        // char buffer[50];
        // sprintf(buffer,"Memory Pool Size = %zu bytes\n",head->size);
        // write(1,buffer,strlen(buffer));
        //////////////////////////

    }
    if(s%32!=0){
        s=s+(32-s%32);
    }
    struct block *cur=head;
    struct block *best=NULL;
    // size_t remain=20000;
    // char buffer[50];
    // sprintf(buffer,"Request for %zu bytes\n",s);
    // write(1,buffer,strlen(buffer));
    while(cur!=NULL){
        if(cur->free==1 && cur->size>=s){
            if(best==NULL) best=cur;
            else if(cur->size<best->size) best=cur;
        }
        cur=cur->next;
    }
    if(best==NULL){
        // sprintf(buffer,"No chunk available\n");
        // write(1,buffer,strlen(buffer));
        return NULL;
    }
    else{
        // sprintf(buffer,"Best size is %zu\n",best->size);
        // write(1,buffer,strlen(buffer));
        if(best->size-s>=sizeof(struct block)){
            struct block *new=(void*)best+s+sizeof(struct block);
            new->size=best->size-s-sizeof(struct block);
            new->free=1;
            new->prev=best;
            new->next=best->next;
            if(best->next!=NULL){
                best->next->prev=new;
            }
            best->size=s;
            best->free=0;
            best->next=new;
        }
        else{
            best->free=0;
        }
        // sprintf(buffer,"Allocated %zu bytes\n",best->size);
        // write(1,buffer,strlen(buffer));
        // print();
        return (void*)best+sizeof(struct block);
    }
}
void free(void* ptr){
    if(ptr==NULL){
        return;
    }
    struct block *cur=ptr-sizeof(struct block);
    //prinf addr
    // char buffer[50];
    // sprintf(buffer,"Freeing %zu bytes\n",cur->size);
    // write(1,buffer,strlen(buffer));
    //////////////////////////
    cur->free=1;
    if(cur->next!=NULL && cur->next->free==1){
        // sprintf(buffer,"Merged with next chunk size:%zu\n",cur->next->size);
        // write(1,buffer,strlen(buffer));
        cur->size=cur->size+cur->next->size+sizeof(struct block);
        cur->next=cur->next->next;
        if(cur->next!=NULL){
            cur->next->prev=cur;
        }
    }
    if(cur->prev!=NULL && cur->prev->free==1){
        // sprintf(buffer,"Merged with prev chunk size:%zu\n",cur->prev->size);
        // write(1,buffer,strlen(buffer));
        cur->prev->size=cur->prev->size+cur->size+sizeof(struct block);
        cur->prev->next=cur->next;
        if(cur->next!=NULL){
            cur->next->prev=cur->prev;
        }
        cur=cur->prev;
    }
    // sprintf(buffer,"Total size: %zu\n",cur->size);
    // write(1,buffer,strlen(buffer));
    // print();
    return;
}