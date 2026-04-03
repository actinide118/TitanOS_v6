typedef struct line{
    char* characters;
    int len;
}text_line_t;

typedef struct matrice{
    text_line_t* lines;
    int len;
}text_matrice_t;