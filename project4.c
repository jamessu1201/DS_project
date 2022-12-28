#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

typedef unsigned long ul;

int ans_count=0;
char ans[30000][30000];


typedef struct Node{
    bool used;
    bool no_right;
    int level;
    int index;
    char* string;
    ul hash;
    struct Node* parent;
    struct Node* left_child;
    struct Node* right_child;
}Node;

ul hash_func(char *key){     //MurmurOAAT32
    ul h=3323198485ul;
    for(;*key;++key){
        h^=*key;
        h*=0x5bd1e995;
        h^=h>>15;
    }
    return h;
}

void add_node(Node* left_node,Node* right_node,int len,int level,int index,bool no_right){    //add new node
    Node *new_node=(Node*)malloc(sizeof(Node));
    char new_str[len+1];
    ul new;
    new=left_node->hash+right_node->hash;
    sprintf(new_str,"%lu",new);
    new=hash_func(new_str);
    new_node->left_child=left_node;
    new_node->right_child=right_node;
    new_node->hash=new;
    new_node->parent=NULL;
    new_node->used=false;
    new_node->level=level;
    new_node->index=index;
    new_node->no_right=no_right;
    left_node->parent=new_node;
    right_node->parent=new_node;
    new_node->left_child->used=true;
    new_node->right_child->used=true;
    // printf("newnode%lu\n",left_node->parent->hash);
    // printf("right_child%lu\n",new_node->right_child->hash);
}

void print_tree(Node* node){
    if(node->right_child==NULL){
        return;
    }
    printf("%d%d %lu\n",node->left_child->level,node->left_child->index,node->left_child->hash);
    printf("%d%d %lu\n",node->right_child->level,node->right_child->index,node->right_child->hash);
    print_tree(node->left_child);
    print_tree(node->right_child);
}

void search_incorrect_string(Node* node,int level,int index){
    ul value;
    printf("%d %d %d\n",1,level,index);
    fflush(NULL);
    scanf("%lu",&value);
    if(node->hash==value){
        return;
    }else{
        if(node->left_child!=NULL){
            search_incorrect_string(node->left_child,node->left_child->level,node->left_child->index);
        }else{
            strcpy(ans[ans_count],node->string);
            ans_count++;
            return;
            //printf("%s\n",node->string);
        }
        if(node->right_child!=NULL&&node->no_right==false){
            search_incorrect_string(node->right_child,node->right_child->level,node->right_child->index);
        }else{
            strcpy(ans[ans_count],node->string);
            ans_count++;
            return;
            //printf("%s\n",node->string);
        }
    }
}

int main(){
    int i,j;
    int strings,strmaxlen;         //input strings and strmaxlen
    scanf("%d %d",&strings,&strmaxlen);

    int count=strings,time=1;
    while(count!=1){                //count the doing time
        if(count%2!=0){
            count++;
        }
        count/=2;
        time++;
    }

    char string[strings][strmaxlen+1];
    Node node[strings];
    for(i=0;i<strings;i++){        //input strings
        scanf("%s",string[i]);
    }

    for(i=0;i<strings;i++){                     //hash every leaves
        node[i].hash=hash_func(string[i]);
        node[i].string=(char*)malloc((strmaxlen+1)*sizeof(char));
        strcpy(node[i].string,string[i]);
        node[i].left_child=NULL;
        node[i].right_child=NULL;
        node[i].parent=NULL;
        node[i].used=false;
        node[i].level=time-1;
        node[i].index=i;
    }
    int pair;
    int node_level=0;  //node level
    while(time>0){                          //creating the tree
        int index=0;
        Node *tmp[2],*parent;
        pair=0;
        for(i=0;i<strings;i++){
            parent=&node[i];
            for(j=0;j<node_level;j++){
                parent=parent->parent;
            }
            if(parent->used==false){
                tmp[pair]=parent;
                parent->used=true;
                pair++;
                if(pair>1){
                    add_node(tmp[0],tmp[1],strmaxlen,time-2,index,0);
                    index++;
                    pair=0;
                }
            }
        }
        if(time==2){          //last node is the root node
            break;
        }
        if(pair==1){
            add_node(tmp[0],tmp[0],strmaxlen,time-2,index,1);
            index++;
            pair=0;
        }
        time--;
        node_level++;
    }



    Node *root=&node[0];                //find the root of merkle tree
    while(root->parent!=NULL){
        root=root->parent;
    }



    // printf("root %lu\n",root->hash);
    // printf("%lu %lu\n",root->left_child->hash,root->right_child->hash);
    // printf("%lu %lu %lu %lu\n",root->left_child->left_child->hash,root->left_child->right_child->hash,root->right_child->left_child->hash,root->right_child->right_child->hash);
    // printf("%lu %lu %lu %lu %lu %lu %lu %lu\n",root->left_child->left_child->left_child->hash,root->left_child->left_child->right_child->hash,root->left_child->right_child->left_child->hash,root->left_child->right_child->right_child->hash,root->right_child->left_child->left_child->hash,root->right_child->left_child->right_child->hash,root->right_child->right_child->left_child->hash,root->right_child->right_child->right_child->hash);
    // for(i=0;i<strings;i++){
    //     printf("%lu ",node[i].hash);
    // }

    // printf("%d%d %lu\n",0,0,root->hash);
    // print_tree(root);

    // printf("root %d%d\n",root->level,root->index);
    // printf("%d%d %d%d\n",root->left_child->level,root->left_child->index,root->right_child->level,root->right_child->index);
    // printf("%d%d %d%d %d%d %d%d\n",root->left_child->left_child->level,root->left_child->left_child->index,root->left_child->right_child->level,root->left_child->right_child->index,root->right_child->left_child->level,root->right_child->left_child->index,root->right_child->right_child->level,root->right_child->right_child->index);
    // printf("%lu %lu %lu %lu %lu %lu %lu %lu\n",root->left_child->left_child->left_child->hash,root->left_child->left_child->right_child->hash,root->left_child->right_child->left_child->hash,root->left_child->right_child->right_child->hash,root->right_child->left_child->left_child->hash,root->right_child->left_child->right_child->hash,root->right_child->right_child->left_child->hash,root->right_child->right_child->right_child->hash);

    

    search_incorrect_string(root,0,0);

    printf("%d %d\n",2,ans_count);
    for(i=0;i<ans_count;i++){
        printf("%s\n",ans[i]);
    }


}