#include "funcs.h"
#include "limits.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "structs.h"

Path **extract_pathes(Vector *vec);

//?----------РАБОТА С ФАЙЛАМИ----------
// NULL = error
Vector *read_from_file(char *filename) {
  FILE *f;
  int rsp = 1, rl = 0, img = 0, rl1 = 0, img1 = 0, ind1 = -1, ind2 = -1;
  Vector *vec = vec_init();
  if (vec == NULL) {
    print_yel("Error creating new vec for read!\n");
    return NULL;
  }

  f = fopen(filename, "r");
  if (f == NULL) {
    print_yel("Error opening file!\n");
    free(vec->verts);
    free_set(vec);
    return NULL;
  }

  while (rsp > 0) {
    rsp = fscanf(f, "%d%*c", &rl);
    if (rsp <= 0) {
      break;
    }
    rsp = fscanf(f, "%d%*c%*c%*c", &img);
    rsp = fscanf(f, "%d%*c", &rl1);
    if (rsp <= 0) {
      break;
    }
    rsp = fscanf(f, "%d%*c%%*c", &img1);
    //* конец считывания

    ind1 = vec_find(vec, rl, img);
    if (ind1 == -1) {
      vec_push(vec, rl, img);
    }
    ind2 = vec_find(vec, rl1, img1);
    if (ind2 == -1) {
      vec_push(vec, rl1, img1);
    }

    rsp = path_create(vec, rl, img, rl1, img1);
  }

  free_set(filename);

  return vec;
}

// -1 = fail, 1 = success
int save_to_file(Vector *vec, char *filename) {
  FILE *fl = fopen(filename, "w");
  if (fl == NULL) {
    free_set(filename);
    return -1;
  }
  Path **all_ways = extract_pathes(vec);
  for (int i = 0; i < vec->total_path; ++i) {
    fprintf(fl, "%d,%di->%d,%di\n", all_ways[i]->from->real,
            all_ways[i]->from->imgnry, all_ways[i]->towards->real,
            all_ways[i]->towards->imgnry);
  }
  free_set(filename);
  free_set(all_ways);
  fclose(fl);

  return 1;
}

//?--------------------GRAPHVIZ--------------------
// объединяет все пути в один массив
Path **extract_pathes(Vector *vec) {
  int pos = 0;
  Path **new_arr = calloc(vec->total_path, sizeof(Path *));

  for (int i = 0; i < vec->length; ++i) {
    for (int j = 0; j < vec->verts[i]->amount; ++j) {
      new_arr[pos] = vec->verts[i]->edges[j];
      pos++;
    }
  }

  return new_arr;
}

//-1 = crit error, 1 = success
int convert_dot(Vector *vec, FILE *fl) {
  if (fl == NULL) {
    print_yel("FILE is NULL!\n");
    return -1;
  }
  int ind_fr = 0, ind_to = 0;
  double weight = 0;

  for (int i = 0; i < vec->length; ++i) {
    fprintf(fl, "\ta%03d [shape=circle, label=\"(%d, %di)\"];\n",
            vec->verts[i]->name, vec->verts[i]->real, vec->verts[i]->imgnry);
  }

  Path **all_pathes = extract_pathes(vec);

  for (int i = 0; i < vec->total_path; ++i) {
    ind_fr = all_pathes[i]->from->name;
    ind_to = all_pathes[i]->towards->name;
    weight = all_pathes[i]->weight;

    fprintf(fl, "\ta%03d -> a%03d [color=crimson, label=\"%.2fl\"];\n", ind_fr,
            ind_to, weight);
  }

  free_set(all_pathes);

  return 1;
}

// -1 = fail, 1 = success
int graph_visual(Vector *vec) {
  if (NULL == vec) {
    return -1;
  }

  FILE *fl;
  int rsp = -1;
  char *filename = malloc(50);
  sprintf(filename, "graph.dot");
  if (NULL == filename) {
    return -1;
  }

  fl = fopen(filename, "w");

  free_set(filename);
  fprintf(fl, "digraph {\n");
  rsp = convert_dot(vec, fl);
  fprintf(fl, "}");
  fclose(fl);
  if (rsp == -1) {
    return -1;
  }

  return 1;
}

//?-----------------BFS----------------
// сброс цветов у всех вершин графа
void recolour(Vector *vec) {
  if (NULL == vec) {
    print_yel("Can't recolour NULL!\n");
    return;
  }
  for (int i = 0; i < vec->length; ++i) {
    vec->verts[i]->coloration->color = 1;
    vec->verts[i]->coloration->distance = INT_MAX;
    vec->verts[i]->coloration->pred = NULL;
  }
}

void BFS(Node *curr, Node *parent) {
  if (curr->coloration->color == -1) {
    return;
  }

  curr->coloration->color = -1;
  for (int i = 0; i < curr->amount; ++i) {
    if (NULL == curr->edges[i]->towards->coloration->pred) {
      curr->edges[i]->towards->coloration->pred = curr;
    }
  }

  for (int i = 0; i < curr->amount; ++i) {
    BFS(curr->edges[i]->towards, curr);
  }

  return;
}

// -1 = crit fail, 0 = deadend, 1 = success
int print_bfs(Vector *vec, int ind_fr, int ind_to) {
  Node *from = vec->verts[ind_to], *final = vec->verts[ind_fr];

  while (from != NULL) {
    if (from->name == ind_fr) {
      printf("(%d, %di)\n", from->real, from->imgnry);
    } else {
      printf("(%d, %di) <- ", from->real, from->imgnry);
    }
    if ((from->real == final->real) && (from->imgnry == final->imgnry)) {
      return 1;
    }
    if (NULL == from->coloration->pred) {
      return -1;
    }
    from = from->coloration->pred;
  }

  return -1;
}

// -1 = fail, 1 = success
int BFS_detour(Vector *vec, int index_fr, int index_to) {
  int rsp = 0;
  recolour(vec);

  BFS(vec->verts[index_fr], NULL);

  rsp = print_bfs(vec, index_fr, index_to);
  if (rsp == 1) {
    print_yel("Path found!\n");
  } else if (rsp == 0) {
    print_yel("DEADEND\nPath not found!\n");
  } else {
    print_yel("Crit error!\n");
  }

  return 1;
}

//?-------------------DFS+BFA-------------------
int sum_custom(int a, int b) {
  if ((a == INT_MAX && b != INT_MIN) || (a != INT_MIN && b == INT_MAX)) {
    return INT_MAX;
  } else if ((a == INT_MIN && b != INT_MAX) || (a != INT_MIN && b == INT_MIN)) {
    return INT_MIN;
  } else {
    return a + b;
  }
}

// -1 = fail, 0 = found neg loop, 1 = success
int BFA(Vector *vec, int source_ind) {
  int from_d, to_d;
  double weight;
  Path **all_roads = extract_pathes(vec);

  vec->verts[source_ind]->coloration->distance = 0;

  for (int i = 1; i <= vec->length - 1; ++i) {
    for (int num = 0; num < vec->total_path; ++num) {
      from_d = all_roads[num]->from->coloration->distance;
      to_d = all_roads[num]->towards->coloration->distance;
      weight = all_roads[num]->weight;

      if (to_d > sum_custom(from_d, weight)) {
        all_roads[num]->towards->coloration->distance =
            sum_custom(from_d, weight);
        all_roads[num]->towards->coloration->pred = all_roads[num]->from;
      }
    }
  }

  // проверка на отрицательные циклы
  for (int rd = 0; rd < vec->total_path; ++rd) {
    from_d = all_roads[rd]->from->coloration->distance;
    to_d = all_roads[rd]->towards->coloration->distance;
    weight = all_roads[rd]->weight;

    if ((to_d > sum_custom(from_d, weight)) ||
        ((to_d > sum_custom(from_d, weight)) && (weight < 0))) {
      free_set(all_roads);
      return 0;
    }
  }
  free_set(all_roads);

  return 1;
}

// -1 = fail, 1 = success
int BFA_print(Vector *vec, int index_from, int index_to) {
  Node *from = vec->verts[index_to];

  printf("\n");
  while (from != NULL) {
    if (from->name == index_from) {
      if (from->coloration->distance == INT_MAX) {
        return -1;
      }
      printf("(%d, %di)\n", from->real, from->imgnry);
      printf("Total distance is %.2lf !\n\n",
             vec->verts[index_to]->coloration->distance);
      return 1;
    } else {
      printf("(%d, %di) <- ", from->real, from->imgnry);
    }
    if (from->coloration->pred == NULL) {
      return -1;
    }
    from = from->coloration->pred;
  }
  return -1;
}

// -1 = error, 0 = neg loop, 1 = success
int BFA_detour(Vector *vec, int index_from, int index_to) {
  int stat = -1;
  recolour(vec);

  stat = BFA(vec, index_from);
  if (stat == -1) {
    return -1;
  } else if (stat == 0) {
    return 0;
  }

  stat = BFA_print(vec, index_from, index_to);

  if (stat == -1) {
    print_yel("DEADEND\nVerticy is unreachable!\n\n");
    return 1;
  }

  return 1;
}
