typedef struct mem_case{
    char* key;
    void* value;
    void* next;
} growing_obj_t;

growing_obj_t* GObj_create();
void* GObj_get(growing_obj_t* obj,char* key);
void GObj_set(growing_obj_t* obj,char* key,void* value);
void GObj_delete(growing_obj_t* obj,char* key);
void GObj_free(growing_obj_t* obj);
void GObj_clear(growing_obj_t* obj);