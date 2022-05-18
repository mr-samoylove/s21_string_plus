#include "s21_string.h"

int s21_atoi(const char *str) {
  int i = 0;
  int res = 0;
  int sign = 1;
  while (str[i] == '0' || str[i] == ' ') i++;
  if (str[i] == '-') {
    sign = -1;
    i++;
  }
  while (48 <= str[i] && str[i] <= 57) {
    res = res * 10 + ((int)str[i] - 48);
    i++;
  }
  return res * sign;
}

// Возвращает новую строку, в которой указанная строка (str) вставлена в
// указанную позицию (start_index) в данной строке (src). В случае какой-либо
// ошибки следует вернуть значение s21_NULL
void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  s21_size_t size_src = s21_strlen(src);
  s21_size_t size_str = s21_strlen(str);
  char *inserted = s21_NULL;

  if ((size_str != 0 || size_src != 0) && start_index <= size_src) {
    s21_size_t i = 0;
    inserted = calloc(size_src + size_str + 2, 1);
    if (inserted != s21_NULL) {
      for (; i < start_index; i++) {
        inserted[i] = src[i];
      }
      int j = 0;
      for (; i < start_index + size_str; i++, j++) {
        inserted[i] = str[0 + j];
      }
      for (; i < size_src + size_str; i++) {
        inserted[i] = src[i - size_str];
      }
    }
  }
  return (void *)inserted;
}

int find_start_index(const char *src, const char *trim_chars) {
  int flag = -1;
  for (int i = 0; src[i] != 0; i++) {
    int match = 0;
    for (int j = 0; trim_chars[j]; j++) {
      if (src[i] == trim_chars[j]) match = 1;
    }
    if (match != 1) {
      flag = i;
      break;
    }
  }
  return flag;
}

int find_end_index(const char *src, const char *trim_chars) {
  int flag = -1;
  int i = s21_strlen(src) - 1;
  for (; i >= 0; i--) {
    int match = 0;
    for (int j = 0; trim_chars[j]; j++) {
      if (src[i] == trim_chars[j]) match = 1;
    }
    if (match != 1) {
      flag = i;
      break;
    }
  }
  return flag;
}

void *s21_trim(const char *src, const char *trim_chars) {
  int start = find_start_index(src, trim_chars);
  int end = find_end_index(src, trim_chars);
  char *trimmed = s21_NULL;
  if (end - start > 0) {
    trimmed = calloc(end - start + 2, 1);
    if (trimmed != s21_NULL) {
      for (int k = 0; start <= end; start++, k++) trimmed[k] = src[start];
    }
  }
  return (void *)trimmed;
}

void *s21_to_lower(const char *str) {
  char *low = s21_NULL;
  if (str[0]) {
    low = calloc(s21_strlen(str) + 2, 1);
    if (low != s21_NULL) {
      for (int i = 0; str[i]; i++) {
        if ('A' <= str[i] && str[i] <= 'Z')
          low[i] = str[i] + 32;
        else
          low[i] = str[i];
      }
    }
  }
  return (void *)low;
}

void *s21_to_upper(const char *str) {
  char *up = s21_NULL;
  if (str[0]) {
    up = calloc(s21_strlen(str) + 2, 1);
    if (up != s21_NULL) {
      for (int i = 0; str[i]; i++) {
        if ('a' <= str[i] && str[i] <= 'z')
          up[i] = str[i] - 32;
        else
          up[i] = str[i];
      }
    }
  }
  return (void *)up;
}
