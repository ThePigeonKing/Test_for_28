#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "structs.h"

void print_yel(char *msg) {
  printf("\x1b[33m%s\x1b[0m", msg);
  return;
}

// char* = success, NULL = fail
char *getLine() {  // считывание строки произвольной длины

  char *line = (char *)malloc(sizeof(char));
  *line = '\0';
  if (line == NULL)  //! MemError
    return NULL;
  char buf[81];
  int rsp, bf_len, ln_len = 0;

  do {
    rsp = scanf("%80[^\n]", buf);
    if (rsp == 0) {
      scanf("%*[\n]");
    }
    if (rsp > 0) {
      bf_len = strlen(buf);
      if (bf_len == 0) {  // пустая строка

        rsp = 0;
        continue;
      }
      ln_len += bf_len;
      line = realloc(line, (ln_len + 1) * sizeof(char));
      if (line == NULL) {  //! MemError
        return NULL;
      }
      strcat(line, buf);
    }
  } while (rsp == 0 || bf_len == 80);

  if (rsp < 0) {
    printf("EOF found!\n");
    free(line);
    return NULL;
  }

  return line;
}

// -1 = error, 1 = success
int getInt(int *num) {
  int rsp;
  while (1) {
    rsp = scanf("%d", num);

    if (rsp > 0) {  // success
      scanf("%*[^\n]");
      return 1;
    } else if (rsp == 0) {
      scanf("%*[^\n]");
      print_yel("Wrong int! Try again..\n");
    } else {
      scanf("%*[^\n]");
      print_yel("EOF in getInt!\n");
      return -1;
    }
  }

  return -1;
}

// -1 = error, 1 - success
int getCoords(int *real, int *img) {
  int stat = -1;

  printf("--> ");
  stat = getInt(real);
  if (-1 == stat) {
    return -1;
  }
  printf("--> ");
  stat = getInt(img);
  if (-1 == stat) {
    return -1;
  }

  return 1;
}

//?------------------Неупорядоченный вектор(ВЕРШИНЫ)------------------

Vector *vec_init() {
  Vector *vec = crtnew(Vector);

  if (NULL == vec) {  //! MemError
    print_yel("Error in vec_init!\n");
    exit(1);
  }

  vec->size = VEC_SIZE;
  vec->verts = calloc(vec->size, sizeof(Node *));
  vec->total_path = 0;

  if (NULL == vec->verts) {
    print_yel("Error in allocating vec->verts!\n");
    free(vec);
    exit(1);
  }

  return vec;
}

// Добавление вершины
void vec_push(Vector *vec, int real_cord, int img_cord) {
  if (vec->length >= vec->size) {
    // удвоение выделенного пространства
    vec->verts = realloc(vec->verts, sizeof(Node *) * (vec->size * 2));
    if (NULL == vec->verts) {
      print_yel("Error in doubling verts space!\n");
      exit(1);
    }
    vec->size *= 2;
  }

  vec->verts[vec->length] = crtnew(Node);
  if (NULL == vec->verts[vec->length]) {
    print_yel("Err in creating new vert!\n");
    exit(1);
  }

  vec->verts[vec->length]->real = real_cord;
  vec->verts[vec->length]->imgnry = img_cord;
  vec->verts[vec->length]->coloration = crtnew(Color);

  if (vec->length == 0) {
    vec->verts[vec->length]->name = 0;
  } else {
    vec->verts[vec->length]->name = vec->verts[vec->length - 1]->name + 1;
  }

  vec->verts[vec->length]->capacity = VEC_SIZE;
  vec->verts[vec->length]->edges =
      realloc(vec->verts[vec->length]->edges, sizeof(Path *) * VEC_SIZE);

  if (NULL == vec->verts[vec->length]->edges) {
    print_yel("Error in allocating Node->edges!\n");
    exit(1);
  }

  vec->length++;
}

// -1 = not found
int vec_find(Vector *vec, int rl, int img) {
  Node **vert = vec->verts;
  for (int i = 0; i < vec->length; ++i) {
    if ((vert[i]->real == rl) && (vert[i]->imgnry == img)) {
      return i;
    }
  }

  return -1;
}

// 1 = success, -1 = fail
int vec_delete(Vector *vec, int index) {
  if (index >= vec->length) {
    print_yel("Index out of bounds!\n");
    return -1;
  }

  vec->length = vec->length - 1;
  free_set(vec->verts[index]);

  // надо ли переставлять элемент
  if (index != vec->length) {
    vec->verts[index] = vec->verts[vec->length];
  }

  return 1;
}

//--4DBG--
void vec_print(Vector *vec) {
  Node *current = NULL;
  printf("VECTOR(%d, %d) {\n", vec->size, vec->length);
  for (int i = 0; i < vec->length; ++i) {
    current = vec->verts[i];
    printf("%d:  (%d & %di)\n", current->name, current->real, current->imgnry);
  }
  printf("}\n");
}

//?----------Работа с ПУТЯМИ----------

// -1 = not found, index = success
int path_find(Node *from, Node *to) {
  for (int i = 0; i < from->amount; ++i) {
    if (from->edges[i]->towards == to) {
      return i;
    }
  }
  return -1;
}

// -1 = fail, 0 = already exists, 1 = success
int path_append(Node *from, Node *to) {
  if (from->amount >= from->capacity) {
    from->edges = realloc(from->edges, sizeof(Node *) * (from->capacity * 2));
    if (NULL == from->edges) {
      print_yel("Error in extending from->edges!\n");
      exit(1);
    }
    from->capacity *= 2;
  }

  Path *new_path = crtnew(Path);
  if (NULL == new_path) {
    print_yel("Error in creating new_path!\n");
    exit(1);
  }

  double new_weight =
      ((from->real + to->real) * (from->real + to->real) -
       (from->imgnry + to->imgnry) * (from->imgnry + to->imgnry));
  new_path->weight = new_weight;
  new_path->from = from;
  new_path->towards = to;
  from->edges[from->amount] = new_path;
  from->amount++;

  return 1;
}

// -1 = fail, 0 = path exist, 10 = from not found, 20 = to not found, 1 =
// success
int path_create(Vector *vec, int rl_from, int im_from, int rl_to, int im_to) {
  if (NULL == vec) {
    return 0;
  }

  int index_from = vec_find(vec, rl_from, im_from);
  if (index_from == -1) {
    return 10;
  }
  int index_to = vec_find(vec, rl_to, im_to);
  if (index_to == -1) {
    return 20;
  }

  int path_index = path_find(vec->verts[index_from], vec->verts[index_to]),
      rsp = -1;
  if (path_index != -1) {
    return 0;
  } else {
    rsp = path_append(vec->verts[index_from], vec->verts[index_to]);
  }
  vec->total_path++;

  return 1;
}
