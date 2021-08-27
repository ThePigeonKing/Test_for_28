#ifndef LAB5_STRUCTS
#define LAB5_STRUCTS

#define VEC_SIZE 20

typedef struct Color {
  int color;  // -1 = black, 1 = grey, 1 = white
  double distance;
  struct Node *pred;
} Color;

typedef struct Path {
  double weight;

  struct Node *from;
  struct Node *towards;
} Path;

typedef struct Node {
  int name;  // уникальный индекс

  int real;    // действительная часть координаты
  int imgnry;  // мнимая часть координаты

  int capacity;  // вместимость edges
  int amount;    // количество edges
  Path **edges;

  Color *coloration;

} Node;

typedef struct Vector {
  int size;
  int length;
  int total_path;

  Node **verts;

} Vector;

void print_yel(char *msg);
char *getLine();
int getInt(int *res);
int getCoords(int *x, int *y);

Vector *vec_init();
void vec_push(Vector *vec, int real_cord, int img_cord);
int vec_find(Vector *vec, int rl, int img);
int vec_delete(Vector *vec, int index);
void vec_print(Vector *vec);

int path_find(Node *from, Node *to);
int path_append(Node *from, Node *to);
int path_create(Vector *vec, int rl_from, int im_from, int rl_to, int im_to);

#define crtnew(t) ((t *)calloc(1, sizeof(t)))

#define free_set(p) \
  do {              \
    if (p) {        \
      free(p);      \
      (p) = 0;      \
    }               \
  } while (0)

#endif