/*
 * =====================================================================================
 *
 *       Filename:  growing_obj.c
 *
 *    Description:  This file contain functions to manage linked list with memory allocation 
 *
 *        Version:  1.0
 *        Created:  27/05/2026 21:15:20
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Titouan (actinide118), 
 *   Organization:  
 *
 * =====================================================================================
 */


#include "growing_obj.h"
#include "../driver/memory.h"
#include <stddef.h>
#include "../util/util.h"
#include "../driver/vga.h"


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  GObj_create
 *  Description:  Create a linked list and return a pointer to the first element 
 * =====================================================================================
 */
growing_obj_t* GObj_create(){
    growing_obj_t* obj=(growing_obj_t*)kmalloc(sizeof(growing_obj_t));
    if(obj==NULL){
        print_string("Allocation failed");
    }
    obj->key=NULL;
    obj->value=NULL;
    obj->next=NULL;
    return obj;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  GObj_get
 *  Description:  Get the value of an element in the list using its key 
 * =====================================================================================
 */
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

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  GObj_set
 *  Description:  Set a key-value couple 
 * =====================================================================================
 */
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

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  GObj_delete
 *  Description:  Delete an element using its index 
 * =====================================================================================
 */
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

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  GObj_free
 *  Description:  Delete a linked list ( don't free the pointer of the key and value ) 
 * =====================================================================================
 */
void GObj_free(growing_obj_t* obj){
    growing_obj_t* curr=obj;
    growing_obj_t* next;
    while(curr->next!=NULL){
        next=curr->next;
        free(curr);
        curr=next;
    }
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  GObj_delete
 *  Description:  Delete a linked list ( free the pointer in value ) 
 * =====================================================================================
 */
void GObj_clear(growing_obj_t* obj){
    growing_obj_t* curr=obj;
    growing_obj_t* next;
    while(curr->next!=NULL){
        next=curr->next;
        //free(curr->key);
        //print_string(curr->key);
        free(curr->value);
        free(curr);
        curr=next;
    }
}
