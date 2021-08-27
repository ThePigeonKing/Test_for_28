#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "structs.h"

// -1 = fail, 0 = exists, 1 = success
int add_vert(Vector *vec, int real, int imaginary) {
  int index = -1;
  if (vec->verts[0] != NULL) {
    index = vec_find(vec, real, imaginary);
  }

  if (index != -1) {
    return 0;
  } else {
    vec_push(vec, real, imaginary);
    return 1;
  }

  return -1;
}

void delete_path(Node *vert, int index) {
  if (index >= vert->amount) {
    print_yel("Path index out of range!\n");
    return;
  }
  vert->amount = vert->amount - 1;

  free(vert->edges[index]);

  if (index != vert->amount) {
    vert->edges[index] = vert->edges[vert->amount];
  }

  return;
}

void delete_connected(Vector *vec, int index) {
  for (int i = 0; i < vec->length; ++i) {
    for (int j = 0; j < vec->verts[i]->amount; ++j) {
      if (vec->verts[i]->edges[j]->towards == vec->verts[index]) {
        delete_path(vec->verts[i], j);
        vec->total_path = vec->total_path - 1;
      }
    }
  }
}

void erase_all(Vector *vec) {
  if (vec == NULL) {
    return;
  }

  for (int i = 0; i < vec->length; ++i) {
    for (int j = 0; j < vec->verts[i]->amount; ++j) {
      free_set(vec->verts[i]->edges[j]);
    }
    free_set(vec->verts[i]->edges);
    free_set(vec->verts[i]->coloration);
    free_set(vec->verts[i]);
  }
  free_set(vec->verts);
  free_set(vec);
}

// -1 = not found, 1 = success
int check_vert(Vector *vec, int rl, int img) {
  int index = vec_find(vec, rl, img);

  if (index == -1) {
    return -1;
  } else {
    printf("\"%d, %di\" has %d paths connected\n", rl, img,
           vec->verts[index]->amount);
    return 1;
  }
}

// переставить name на всех точках
void vert_rename(Vector *vec) {
  for (int i = 0; i < vec->length; ++i) {
    if (vec->verts[i]->name != i) {
      vec->verts[i]->name = i;
    }
  }
}