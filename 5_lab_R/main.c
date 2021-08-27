#include "additional.h"
#include "funcs.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "structs.h"

void list_commands() {
  printf("\n\n1. ====Vertices====\n");
  printf("11. Add verticy\n");
  printf("12. Check verticy\n");
  printf("13. Print all vertices\n");
  printf("14. Delete verticy\n");

  printf("2. ====Pathes===\n");
  printf("21. Add path\n");
  printf("22. Check path\n");
  printf("23. Delete path\n");

  printf("===Utils===\n");
  printf("31. Read from file\n");
  printf("32. Save to file\n");
  printf("33. Convert to .dot\n");
  printf("34. BFS\n");
  printf("35. Bellman-Ford Alghorithm\n");
  // printf("35. Find SCC\n");

  // printf( "36. Generate random\n" );
  printf("0. Exit programm\n");
  printf("\n");
}

int main() {
  int rsp, command, loop = 0;
  int rl = 0, rl1 = 0, img = 0, img1 = 0, index1 = 0, index2 = 0;
  Vector *vec = vec_init();
  char *filename;

  while (loop != -1) {
    list_commands();
    printf("--> ");
    rsp = getInt(&command);
    if (rsp == -1) {
      print_yel("Err in getting command number!\n");
      //! DELETE
      return 1;
    }

    switch (command) {
      // добавление вершины
      case 11:
        printf("Enter \"rl\" & \"img\" for new verticy: \n");
        rsp = getCoords(&rl, &img);
        if (rsp == -1) {
          print_yel("Error in adding new vert!\n");
          //! DELETE
          return 1;
        }
        rsp = add_vert(vec, rl, img);
        if (rsp == -1) {
          print_yel("Error in adding new verticy!\n");
          //! DELETE
          return 1;
        } else if (rsp == 0) {
          print_yel("Error! Verticy already exists!\n");
        } else {
          print_yel("Success!\n");
        }
        break;

      // проверка вершины + инфо
      case 12:
        printf("Enter \"rl\" & \"img\" to find: \n");
        rsp = getCoords(&rl, &img);
        if (rsp == -1) {
          print_yel("Error in finding vert!\n");
          //! DELETE
          return 1;
        }
        rsp = check_vert(vec, rl, img);
        if (rsp == -1) {
          print_yel("Not found!\n");
        }

        break;

      // все точки графа
      case 13:
        vec_print(vec);
        break;

      // удаление вершины
      case 14:
        printf("Enter \"rl\" & \"img\" to delete: \n");
        rsp = getCoords(&rl, &img);
        if (rsp == -1) {
          print_yel("Error in getting coordinates!\n");
          //! DELETE
          return 1;
        }
        index1 = vec_find(vec, rl, img);
        if (index1 == -1) {
          print_yel("Not found!\n");
        } else {
          delete_connected(vec, index1);
          vec->length -= 1;
          for (int i = 0; i < vec->verts[index1]->amount; ++i) {
            free_set(vec->verts[index1]->edges[i]);
          }
          // переставление если удалён не последний элемент
          free(vec->verts[index1]->edges);
          free_set(vec->verts[index1]);
          if (index1 != vec->length) {
            vec->verts[index1] = vec->verts[vec->length];
          }

          vert_rename(vec);
        }

        break;

      // добавление пути
      case 21:
        printf("Enter coords FROM which to pave the way: \n");
        rsp = getCoords(&rl, &img);
        if (rsp == -1) {
          print_yel("Error in getting first cords!\n");
          //! DELETE;
          return 1;
        }
        printf("Enter coords TOWARDS which to pave the way: \n");
        rsp = getCoords(&rl1, &img1);
        if (rsp == -1) {
          print_yel("Error in getting second cords!\n");
          //! DELETE;
          return 1;
        }

        rsp = path_create(vec, rl, img, rl1, img1);
        if (rsp == -1) {
          print_yel("Error in creating new path!\n");
          //! DELETE
          return 1;
        } else if (rsp == 0) {
          print_yel("Path already exist!\n");
        } else if (rsp == 10) {
          print_yel("Verticy FROM not found!\n");
        } else if (rsp == 20) {
          print_yel("Verticy TOWARDS not found!\n");
        } else if (rsp == 1) {
          print_yel("Added!\n");
        }
        break;

      // проверка существования пути
      case 22:
        printf("Enter cords FROM which to check path: \n");
        rsp = getCoords(&rl, &img);
        if (rsp == -1) {
          print_yel("Error in getting coordinates1!\n");
          //! DELETE
          return 1;
        }
        printf("Enter (x2, y2) TOWARDS which to find path -->\n");
        rsp = getCoords(&rl1, &img1);
        if (-1 == rsp) {
          print_yel("Error in getting coordinates2!\n");
          //! DELETE
          return 1;
        }
        index1 = vec_find(vec, rl, img);
        if (index1 == -1) {
          print_yel("First vert not found!\n");
          break;
        }
        index2 = vec_find(vec, rl1, img1);
        if (index2 == -1) {
          print_yel("Second vert not found!\n");
          break;
        }
        rsp = path_find(vec->verts[index1], vec->verts[index2]);
        if (rsp == -1) {
          print_yel("Path doesn't exist!\n");
        } else {
          printf("Path from (%d, %di) to (%d, %di) has weight %.2lf\n", rl, img,
                 rl1, img1, vec->verts[index1]->edges[rsp]->weight);
        }

        break;

      // удаление пути
      case 23:
        printf("Enter cords FROM which to delete path: \n");
        rsp = getCoords(&rl, &img);
        if (rsp == -1) {
          print_yel("Error in getting coordinates1!\n");
          //! DELETE
          return 1;
        }
        printf("Enter (x2, y2) TOWARDS which to delete path -->\n");
        rsp = getCoords(&rl1, &img1);
        if (-1 == rsp) {
          print_yel("Error in getting coordinates2!\n");
          //! DELETE
          return 1;
        }
        index1 = vec_find(vec, rl, img);
        if (index1 == -1) {
          print_yel("First vert not found!\n");
          break;
        }
        index2 = vec_find(vec, rl1, img1);
        if (index2 == -1) {
          print_yel("Second vert not found!\n");
          break;
        }
        rsp = path_find(vec->verts[index1], vec->verts[index2]);
        if (rsp == -1) {
          print_yel("Path not found!\n");
        } else {
          vec->verts[index1]->amount -= 1;
          free_set(vec->verts[index1]->edges[rsp]);
          // надо ли переставлять
          if (rsp != vec->verts[index1]->amount) {
            vec->verts[index1]->edges[rsp] =
                vec->verts[index1]->edges[vec->verts[index1]->amount];
          }
        }

        break;

      // считывание из файла
      case 31:

        printf("Enter name of file: ");
        filename = getLine();
        if (NULL == filename) {
          print_yel("Error getting filename!\n");
          //! DELETE
          return 1;
        }

        erase_all(vec);

        vec = NULL;
        vec = read_from_file(filename);
        if (NULL == vec) {
          print_yel("Failed to read graph!\n");
        } else {
          print_yel("Success!\n");
        }

        break;

      // сохранить в файл
      case 32:
        printf("Enter name of file to save --> ");
        filename = getLine();
        if (NULL == filename) {
          print_yel("Error in getting filename!\n");
          //! DELETE
          return 1;
        }
        rsp = save_to_file(vec, filename);
        if (rsp == 1) {
          print_yel("Success!\n");
        }

        break;

      // визуализация графа
      case 33:
        rsp = graph_visual(vec);
        if (rsp == -1) {
          print_yel("FAIL converting to .dot!\n");
        } else {
          print_yel("Success!\n");
        }

        break;

      // bfs
      case 34:
        printf("Enter starting point: \n");
        rsp = getCoords(&rl, &img);
        if (rsp == -1) {
          print_yel("Error in getting first cords!\n");
          //! DELETE;
          return 1;
        }
        printf("Enter final point: \n");
        rsp = getCoords(&rl1, &img1);
        if (rsp == -1) {
          print_yel("Error in getting second cords!\n");
          //! DELETE;
          return 1;
        }
        index1 = vec_find(vec, rl, img);
        if (index1 == -1) {
          print_yel("Starting point not found!\n");
          break;
        }
        index2 = vec_find(vec, rl1, img1);
        if (index2 == -1) {
          print_yel("Final point not found!\n");
          break;
        }
        rsp = BFS_detour(vec, index1, index2);

        break;

      // Bellmann-Ford
      case 35:

        printf("Enter starting point: \n");
        rsp = getCoords(&rl, &img);
        if (rsp == -1) {
          print_yel("Error in getting first cords!\n");
          //! DELETE;
          return 1;
        }
        printf("Enter final point: \n");
        rsp = getCoords(&rl1, &img1);
        if (rsp == -1) {
          print_yel("Error in getting second cords!\n");
          //! DELETE;
          return 1;
        }
        index1 = vec_find(vec, rl, img);
        if (index1 == -1) {
          print_yel("Starting point not found!\n");
          break;
        }
        index2 = vec_find(vec, rl1, img1);
        if (index2 == -1) {
          print_yel("Final point not found!\n");
          break;
        }

        rsp = BFA_detour(vec, index1, index2);

        if (rsp == 0) {
          print_yel("Negative loop found! Can't calculate shortest path!\n");
        } else if (rsp == -1) {
          print_yel("Error in BFA_DETOUR!\n");
          //! DELETE
          return 1;
        }

        break;

      case 0:
        print_yel("End of programm...\n");
        erase_all(vec);
        return 0;

      default:
        print_yel("Wrong command!\n");
        break;
    }
  }

  return 0;
}
