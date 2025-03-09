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
#include<set>
#include<vector>
#include<algorithm>
#include<fstream>
#include<sys/time.h>
using namespace std;


//linked list
struct ListNode{
    unsigned int frame_id;
    int freq;
    int latest_seq;
    int heap_index;
    struct ListNode *next; 
    struct ListNode *prev; 
};
struct ListNode *head=NULL;  //MRU
struct ListNode *tail=NULL; //LRU
//TreeNode
struct TreeNode{
    int frame_id;
    struct ListNode* listnode;
    struct TreeNode *left;
    struct TreeNode *right;
};
//BST
class BST{
    public:
        struct TreeNode* root;
        int size;
        BST(){
            root=NULL;
            size=0;
        }
        ListNode* insert(unsigned int id){
            size+=1;
            if(root==NULL){
                struct TreeNode *new_node=new TreeNode();
                new_node->frame_id=id;
                new_node->left=NULL;
                new_node->right=NULL;
                root=new_node;
                struct ListNode *new_listnode=new ListNode();
                new_listnode->frame_id=id;
                new_listnode->freq=1;
                new_listnode->next=NULL;
                new_listnode->prev=NULL;
                new_node->listnode=new_listnode;
                return new_listnode;
            }
            else{
                struct TreeNode *cur=root;
                while(cur!=NULL){
                    if(cur->frame_id==id){
                        return cur->listnode;
                    }
                    else if(cur->frame_id>id){
                        if(cur->left==NULL){
                            struct TreeNode *new_node=new TreeNode();
                            new_node->frame_id=id;
                            new_node->left=NULL;
                            new_node->right=NULL;
                            cur->left=new_node;
                            struct ListNode *new_listnode=new ListNode();
                            new_listnode->frame_id=id;
                            new_listnode->freq=1;
                            new_listnode->next=NULL;
                            new_listnode->prev=NULL;
                            new_node->listnode=new_listnode;
                            return new_listnode;
                        }
                        else{
                            cur=cur->left;
                        }
                    }
                    else{
                        if(cur->right==NULL){
                            struct TreeNode *new_node=new TreeNode();
                            new_node->frame_id=id;
                            new_node->left=NULL;
                            new_node->right=NULL;
                            cur->right=new_node;
                            struct ListNode *new_listnode=new ListNode();
                            new_listnode->frame_id=id;
                            new_listnode->freq=1;
                            new_listnode->next=NULL;
                            new_listnode->prev=NULL;
                            new_node->listnode=new_listnode;
                            return new_listnode;
                        }
                        else{
                            cur=cur->right;
                        }
                    }
                }
            }
            return nullptr;
        }
        ListNode* find(unsigned int id){
            if(root==NULL){
                return NULL;
            }
            else{
                struct TreeNode *cur=root;
                while(cur!=NULL){
                    if(cur->frame_id==id){
                        return cur->listnode;
                    }
                    else if(cur->frame_id>id){
                        cur=cur->left;
                    }
                    else{
                        cur=cur->right;
                    }
                }
                return NULL;
            }
        }
        void Delete(unsigned int id){
            size-=1;
            if(root==NULL){
                return;
            }
            else{
                struct TreeNode *cur=root;
                struct TreeNode *parent=NULL;
                while(cur!=NULL){
                    if(cur->frame_id==id){
                        break;
                    }
                    else if(cur->frame_id>id){
                        parent=cur;
                        cur=cur->left;
                    }
                    else{
                        parent=cur;
                        cur=cur->right;
                    }
                }
                if(cur==NULL){
                    return;
                }
                else{
                    //no child
                    if(cur->left==NULL&&cur->right==NULL){
                        if(parent==NULL){
                            root=NULL;
                        }
                        else{
                            if(parent->left==cur){
                                parent->left=NULL;
                            }
                            else{
                                parent->right=NULL;
                            }
                        }
                        delete cur;
                    }
                    //one child
                    else if(cur->left==NULL){
                        if(parent==NULL){
                            root=cur->right;
                        }
                        else{
                            if(parent->left==cur){
                                parent->left=cur->right;
                            }
                            else{
                                parent->right=cur->right;
                            }
                        }
                        delete cur;
                    }
                    else if(cur->right==NULL){
                        if(parent==NULL){
                            root=cur->left;
                        }
                        else{
                            if(parent->left==cur){
                                parent->left=cur->left;
                            }
                            else{
                                parent->right=cur->left;
                            }
                        }
                        delete cur;
                    }
                    //two child
                    else{
                        struct TreeNode *temp=cur->right;
                        struct TreeNode *temp_parent=cur;
                        while(temp->left!=NULL){
                            temp_parent=temp;
                            temp=temp->left;
                        }
                        cur->frame_id=temp->frame_id;
                        cur->listnode=temp->listnode;
                        if(temp_parent==cur){
                            cur->right=temp->right;
                        }
                        else{
                            temp_parent->left=temp->right;
                        }
                        delete temp;
                    }
                }
            }
        }
        int TreeSize(){
            return size;
        }
};
void Down_Heapify(vector<struct ListNode*> &heap,struct ListNode* node){
    int index=node->heap_index;
    int left=2*index+1;
    int right=2*index+2;
    int min_index=index;
    if(left<heap.size()&&heap[left]!=NULL){
        if(heap[left]->freq<heap[min_index]->freq){
            min_index=left;
        }
        else if(heap[left]->freq==heap[min_index]->freq){
            if(heap[left]->latest_seq<heap[min_index]->latest_seq){
                min_index=left;
            }
        }
    }
    if(right<heap.size()&&heap[right]!=NULL){
        if(heap[right]->freq<heap[min_index]->freq){
            min_index=right;
        }
        else if(heap[right]->freq==heap[min_index]->freq){
            if(heap[right]->latest_seq<heap[min_index]->latest_seq){
                min_index=right;
            }
        }
    }
    // cout<<"in down min_index: "<<min_index<<" index: "<<index<<endl;
    if(min_index!=index){
        struct ListNode *temp=heap[min_index];
        heap[min_index]=heap[index];
        heap[index]=temp;
        heap[min_index]->heap_index=min_index;
        heap[index]->heap_index=index;
        Down_Heapify(heap,node);
    }
}
void UP_Heapify(vector<struct ListNode*> &heap,struct ListNode *node){
    int index=node->heap_index;
    while(index>0){
        int parent=(index-1)/2;
        // cout<<"index: "<<index<<" parent: "<<parent<<endl;
        if(heap[parent]->freq>heap[index]->freq){
            struct ListNode *temp=heap[parent];
            heap[parent]=heap[index];
            heap[index]=temp;
            heap[parent]->heap_index=parent;
            heap[index]->heap_index=index;
            index=parent;
        }
        else{
            break;
        }
    }
}
int main(int argc,char *argv[]){
    vector<unsigned int> input;
    ifstream fin(argv[1]);
    int n;
    while(fin>>n){
        input.push_back(n);
    }
    //LFU
    printf("LFU policy: \n");
    printf("Frame\tHit\t\tMiss\t\tPage fault ratio\n");
    struct timeval start,end;
    gettimeofday(&start,NULL);
    for(int frame=64;frame<=512;frame*=2){
        head=NULL;
        tail=NULL;
        BST s;
        vector<struct ListNode*> heap;
        int hit=0;
        int miss=0;
        int seq=0;
        for(auto& page:input){
            //using linked list sort in freq then framd_id
             if(!s.find(page)){
                miss+=1;
                if(s.TreeSize()==frame){
                    struct ListNode *node=heap[0];
                    heap[0]=NULL;
                    s.Delete(node->frame_id);
                    struct ListNode *new_node=s.insert(page);
                    new_node->latest_seq=seq;
                    heap[0]=new_node;
                    new_node->heap_index=0;
                    Down_Heapify(heap,new_node);
                }
                else{
                    struct ListNode *new_node=s.insert(page);
                    new_node->latest_seq=seq;
                    heap.push_back(new_node);
                    new_node->heap_index=s.TreeSize()-1;
                    UP_Heapify(heap,new_node);
                }
            }
            else{
                hit+=1;
                struct ListNode *node=s.find(page);
                node->freq+=1;
                node->latest_seq=seq;
                Down_Heapify(heap,node);
            
            }
            seq+=1;
        }   
        printf("%d\t%d\t\t%d\t\t%.10f\n",frame,hit,miss,(double)miss/(hit+miss));
    }
    gettimeofday(&end,NULL);
    double time=(end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec);
    printf("Total elapsed time %.4f sec\n\n", time/1000000);
    //LRU
    // /*
    printf("LRU policy: \n");
    printf("Frame\tHit\t\tMiss\t\tPage fault ratio\n");
    gettimeofday(&start,NULL);
    for(int frame=64;frame<=512;frame*=2){
        head=NULL;
        tail=NULL;
       
        int hit=0;
        int miss=0;
        // set<int> s;
        BST s;
        for(auto& page:input){
            // cout<<page<<endl;
            struct ListNode* page_node=s.find(page);
            if(page_node==NULL){
                miss+=1;
                if(s.TreeSize()==frame){
                    unsigned int id=tail->frame_id;
                    tail=tail->prev;
                    delete tail->next;
                    tail->next=NULL; 
                    s.Delete(id);
                    struct ListNode* node= s.insert(page);
                    node->next=head;
                    node->prev=NULL;
                    head->prev=node;
                    head=node;
                }
                else{
                    struct ListNode* node=s.insert(page);
                    if(head==NULL){
                        head=node;
                        tail=node;
                    }
                    else{
                        node->next=head;
                        node->prev=NULL;
                        head->prev=node;
                        head=node;
                    }
                }
            }
            else{
                hit+=1; 
                //at the head
                if(head->frame_id==page){
                    continue;
                }
                //at the end
                else if(tail->frame_id==page){
                    tail=tail->prev;
                    tail->next=NULL;
                    page_node->next=head;
                    page_node->prev=NULL;
                    head->prev=page_node;
                    head=page_node;
                }
                //in the middle
                else{
                    page_node->prev->next=page_node->next;
                    page_node->next->prev=page_node->prev;
                    page_node->next=head;
                    page_node->prev=NULL;
                    head->prev=page_node;
                    head=page_node;
                }  
            }
        }
        printf("%d\t%d\t\t%d\t\t%.10f\n",frame,hit,miss,(double)miss/(hit+miss));
    }
    gettimeofday(&end,NULL);
    time=(end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec);
    printf("Total elapsed time %.4f sec\n", time/1000000);//
    // */
    return 0;
}