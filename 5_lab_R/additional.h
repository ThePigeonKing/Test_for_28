#ifndef LAB5_DETOURS
#define LAB5_DETOURS

#include "structs.h"

Vector *read_from_file(char *filename);
int save_to_file(Vector *vec, char *filename);
int graph_visual(Vector *vec);

int BFS_detour(Vector *vec, int index_fr, int index_to);
int BFA_detour(Vector *vec, int index_from, int index_to);

#endif