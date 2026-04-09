#include "growing_obj.h"
#include "../driver/memory.h"
#include <stddef.h>
#include "../util/util.h"

growing_obj_t* GObj_create(){
    growing_obj_t* obj=(growing_obj_t*)kmalloc(sizeof(growing_obj_t));
    obj->key=NULL;
    obj->value=NULL;
    obj->next=NULL;
    return obj;
}

void* GObj_get(growing_obj_t* obj,char* key){
    bool is_finished=false;
    growing_obj_t* curr=obj;
    while(!is_finished){
        if(strcmp(curr->key,key)==0){
            is_finished=true;
            return curr->value;
        }
        if(curr->next==NULL){
            is_finished=true;
        }else{
            curr=curr->next;
        }
    }
    return NULL;
}

void GObj_set(growing_obj_t* obj,char* key,void* value){
    bool is_finished=false;
    growing_obj_t* curr=obj;
    while(!is_finished){
        if(strcmp(curr->key,key)==0){
            is_finished=true;
            curr->value=value;
            return;
        }
        if(curr->next==NULL){
            is_finished=true;
        }else{
            curr=curr->next;
        }
    }
    is_finished=false;
    curr=obj;
    while(!is_finished){
        if(curr->key==NULL){
            is_finished=true;
            curr->value=value;
            curr->key=key;
            return;
        }
        if(curr->next==NULL){
            is_finished=true;
        }else{
            curr=curr->next;
        }
    }

    growing_obj_t* next_obj=kmalloc(sizeof(growing_obj_t));
    next_obj->key=key;
    next_obj->value=value;
    next_obj->next=NULL;
    curr->next=next_obj;
    return;
}

void GObj_delete(growing_obj_t* obj,char* key){
    bool is_finished=false;
    growing_obj_t* curr=obj;
    growing_obj_t* last=NULL;
    while(!is_finished){
        if(strcmp(curr->key,key)==0){
            if(last==NULL){
                curr->key=NULL;
                curr->value=NULL;
            }else{
                last->next=curr->next;
                free(curr);
            }
        }
        if(curr->next==NULL){
            is_finished=true;
        }else{
            last=curr;
            curr=curr->next;

        }
    }
}