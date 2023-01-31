#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

/* функция для разделения */
void split(FILE *in, long size, FILE *out, int bl_num, int column, int row,
           int *col_key, int *row_key);
/* функция для построения */
void build(FILE *in, long size, FILE *out, int bl_num, int column, int row,
           int *col_key, int *row_key);

/* проверьте правильность ввода */
int check() {
  int flag = 0; /* Флаг для правильного ввода */
  int a = 5;    /* Количество символов для чтения */
  char num[a]; /* Массив для обработки входных данных */

  /* Получить рабочий режим */
  int mood = 0;
  while (0 == mood) {
    for (int i = 0; i < a; i++) {
      num[i] = '\0';
    }
    fgets(num, a, stdin);
    for (int i = 0; i < a; i++) {
      if (num[i] == '\n') {
        flag = 1;
        break;
      }
    }
    if (flag != 1) {
      int c;
      printf("Ввод слишком длинный, повторите попытку: ");
      while ((c = getchar()) != '\n')
        continue;
    } else if (1 == flag) {
      for (int i = 0; num[i] != '\n'; i++) {
        if (!isdigit(num[i])) {
          int c;
          printf("Неверный ввод, повторите попытку: ");
          while ((c = getchar()) != '\n')
            flag = 0;
          break;
        }
      }
      if (1 == flag) {
        mood = atoi(num);
      }
    }
  }
  return mood;
}

int main(int argc, char **argv) {
  /* Проверьте количество аргументов */
  if (argc != 3) {
    fprintf(stderr, "Укажите входной и выходной файл:\n");
    exit(EXIT_FAILURE);
  }

  /*Переменная для входного файла */
  FILE *in, *out;
  in = fopen(argv[1], "r");
  if (!in) {
    perror(argv[1]);
    exit(EXIT_FAILURE);
  }

  /* Переменная для выходного файла */
  out = fopen(argv[2], "w");
  if (!out) {
    perror(argv[2]);
    exit(EXIT_FAILURE);
  }

  /* Получить размер входного файла */
  struct stat buf;
  if (-1 == (stat(argv[1], &buf))) {
    perror(argv[1]);
    exit(EXIT_FAILURE);
  }

  /* Получение режима работы */
  printf("Мы делаем сборку или разделение?\n");
  printf("1 - разрушение\n2 - строительство\n");
  int mood = 0;
  while (0 == mood) {
    mood = check();
    if ((mood != 1) && (mood != 2)) {
      mood = 0;
      printf("Введите 1 или 2: ");
    }
  }

  /* Получить количество блоков и столбцов */
  int bl_num = 0; /* Количество блоков */
  int column = 0; /* Количество столбцов */
  printf("Введите количество блоков: ");
  while (0 == bl_num) {
    bl_num = check();
    if (bl_num <= 0) {
      bl_num = 0;
      printf("Введите число, большее нуля: ");
    }
  }
  printf("Введите количество столбцов:\n");
  while (0 == column) {
    column = check();
    if (column <= 0) {
      column = 0;
      printf("Введите число, большее нуля: ");
    }
  }

  /* Рассчитайте количество строк, возьмите максимально возможное */
  int row = 0;
  for (int i = bl_num - 1; i >= 1; i--) {
    if (bl_num == i * column) {
      row = i;
      break;
    }
  }

  /* Массив для ключа столбца */
  int col_key[column];
/* Пометка на случай, если ключи неверны */
again_col:
  for (int i = 0; i < column; i++)
    col_key[i] = 0;

  /* Получить ключ для столбца */
  for (int i = 0; i < column; i++) {
    while (0 == col_key[i]) {
      printf("Введите символ ключа столбца %d:\n", i + 1);
      col_key[i] = check();
      if ((col_key[i] <= 0) || (col_key[i] > bl_num)) {
        col_key[i] = 0;
        printf("Вы находитесь за пределами дозволенного\n");
      }
    }
  }
  /* Убедитесь, что они не совпадают */
  for (int i = 0; i < column; i++) {
    for (int j = 0; j < column; j++) {
      if (i == j)
        continue;
      if (col_key[i] == col_key[j]) {
        printf("Значения ключей совпадают, повторно введите все\n");
        goto again_col;
      }
    }
  }

  /* Массив для ключей строк */
  int row_key[row];
/* Метка в случае неправильного ввода */
again_row:
  for (int i = 0; i < row; i++)
    row_key[i] = 0;
  /* Получить ключ для строк*/
  for (int i = 0; i < row; i++) {
    while (0 == row_key[i]) {
      printf("Введите ключевой символ %d для строк:\n", i + 1);
      row_key[i] = check();
      if ((row_key[i] <= 0) || (row_key[i] > bl_num)) {
        row_key[i] = 0;
        printf("Не удалось выполнить границу, попробуйте еще раз\n");
      }
    }
  }
  /* Проверка ключей на совпадение */
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < row; j++) {
      if (i == j)
        continue;
      if (row_key[i] == row_key[j]) {
        printf("Значения ключей совпадают, введите все еще раз\n");
        goto again_row;
      }
    }
  }

  /* Вызовите функцию разделения или сборки */
  if (1 == mood)
    split(in, buf.st_size, out, bl_num, column, row, col_key, row_key);

  else
    build(in, buf.st_size, out, bl_num, column, row, col_key, row_key);

  /* Когда функция выполняется, мы печатаем сообщение, в котором говорится
все данные были занесены в файл */
  printf("Результат находится в выходном файле\n");

  /* Закройте файлы */
  fclose(in);
  fclose(out);
  return 0;
}

/* функция для разделения */
void split(FILE *in, long size, FILE *out, int bl_num, int column, int row,
           int *col_key, int *row_key) {
  char blocks[bl_num][size / bl_num + 1]; /* массив блоков */
  int koef[bl_num]; /* В этом массиве мы подсчитываем, сколько символов
                    записываются в каждом блоке*/
  for (int i = 0; i < bl_num; i++) /*Мы обнуляем его */
    koef[i] = 0;
  int i = column - 1; /* Переменная для обхода столбцов */
  int j = row - 1; /* Переменная для обхода строк */
  int c; /* Переменная для чтения символов */

  /* Заполнение наших блоков */
  while ((c = fgetc(in)) != EOF) {
    /* Перейти к следующему ключевому символу столбца */
    i++;
    if (i >= column)
      i = 0;
    if (0 == i) {
      j++;
      if (j >= row)
        j = 0;
    }

    int k = 0; /* вычислите номер блока по формуле из задания */
    k = (column * (row_key[j] - 1)) + col_key[i] - 1;
    blocks[k][koef[k]] = (char)c;
    koef[k]++;
  }

  /* Заполните выходной файл необходимыми значениями из наших блоков */
  for (int a = 0; a < bl_num; a++) {
    for (int b = 0; b < koef[a]; b++) {
      fprintf(out, "%c", blocks[a][b]);
    }
  }
}

/* Функция для построения */
void build(FILE *in, long size, FILE *out, int bl_num, int column, int row,
           int *col_key, int *row_key) {
  char blocks[bl_num][size / bl_num + 1]; /* массив блоков */
  int koef[bl_num]; /* В этом массиве мы подсчитываем, сколько символов
                    записываются в каждом блоке */
  for (int q = 0; q < bl_num; q++)
    koef[q] = 0;
  int i = column - 1; /* Переменная для обхода столбцов */
  int j = row - 1; /* Переменная для обхода строк */
  int n = size;

  /* Запомните в массиве koef, сколько элементов содержится в каждом блоке */
  while (n) {
    /* Переход к следующему ключевому символу столбца */
    i++;
    if (i >= column)
      i = 0;
    if (0 == i) {
      j++;
      if (j >= row)
        j = 0;
    }

    int k = 0; /* вычислите номер блока по формуле из задания */
    k = (column * (row_key[j] - 1)) + col_key[i] - 1;
    koef[k]++;
    n--;
  }

  /*Запись элементов в блоки */
  int c; /* Для чтения символов */
  for (int a = 0; a < bl_num; a++) {
    for (int b = 0; b < koef[a]; b++) {
      if ((c = fgetc(in)) != EOF) {
        blocks[a][b] = (char)c;
      }
    }
  }

  /* Обнуление массива, который будет использоваться для создания файла */
  for (int a = 0; a < bl_num; a++)
    koef[a] = 0;

  /* сгенерируйте файл */
  i = column - 1;
  j = row - 1;
  n = size;
  while (n) {
    /*Переход к следующему ключевому символу столбца*/
    i++;
    if (i >= column)
      i = 0;
    if (0 == i) {
      j++;
      if (j >= row)
        j = 0;
    }

    int k = 0; /*вычислите номер блока по формуле из задания */
    k = (column * (row_key[j] - 1)) + col_key[i] - 1;
    fprintf(out, "%c", blocks[k][koef[k]]);
    koef[k]++;
    n--;
  }
}