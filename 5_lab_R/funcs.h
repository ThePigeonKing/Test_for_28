#ifndef LAB5_FUNC
#define LAB5_FUNC

#include "structs.h"

void erase_all(Vector *vec);
void delete_connected(Vector *vec, int index);
void delete_path(Node *vert, int index);
int add_vert(Vector *vec, int real, int imaginary);
int check_vert(Vector *vec, int rl, int img);
void vert_rename(Vector *vec);

#endif