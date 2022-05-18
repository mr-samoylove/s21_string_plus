#include "s21_string.h"

void* s21_memchr(const void* str, int c, s21_size_t n) {
  const unsigned char* char_ptr;
  char_ptr = (const unsigned char*)str;
  int flag = 0;

  while (n-- && !flag) {
    if (*char_ptr == (unsigned char)c) {
      flag = 1;
      break;
    }
    char_ptr++;
  }

  return flag ? (void*)char_ptr : s21_NULL;
}

int s21_memcmp(const void* str1, const void* str2, s21_size_t n) {
  const unsigned char* s1 = (const unsigned char*)str1;
  const unsigned char* s2 = (const unsigned char*)str2;
  int dif = 0;

  while (n-- > 0) {
    if (*s1 != *s2) {
      dif = (int)s1[0] - (int)s2[0];
      break;
    }
    s1++;
    s2++;
  }
  return dif;
}

void* s21_memcpy(void* dest, const void* src, s21_size_t n) {
  unsigned char* dest_ptr = (unsigned char*)dest;
  const unsigned char* src_ptr = (const unsigned char*)src;

  while (n--) *dest_ptr++ = *src_ptr++;
  return dest;
}

void* s21_memmove(void* dest, const void* src, s21_size_t n) {
  unsigned char* dest_ptr = (unsigned char*)dest;
  unsigned char* src_ptr = (unsigned char*)src;

  if (dest_ptr - src_ptr >= (long int)n) {
    dest = s21_memcpy(dest, src, n);
  } else {
    dest_ptr += n - 1;
    src_ptr += n - 1;
    while (n--) *dest_ptr-- = *src_ptr--;
  }
  return dest;
}

void* s21_memset(void* str, int c, s21_size_t n) {
  unsigned char* str_ptr = (unsigned char*)str;
  while (n--) *str_ptr++ = (unsigned char)c;
  return str;
}

char* s21_strcat(char* dest, const char* src) {
  s21_strcpy(dest + s21_strlen(dest), src);
  return dest;
}

char* s21_strchr(const char* str, int c) {
  char* res = s21_NULL;
  c = (char)c;
  for (s21_size_t i = 0; i <= s21_strlen(str); i++) {
    if (str[i] == c) {
      res = (char*)&str[i];
      break;
    }
  }
  return res;
}

int s21_strcmp(const char* str1, const char* str2) {
  for (; *str1 && *str1 == *str2; str1++, str2++) {
  }
  return *str1 - *str2;
}

char* s21_strcpy(char* dest, const char* src) {
  return s21_memcpy(dest, src, s21_strlen(src) + 1);
}

s21_size_t s21_strcspn(const char* str1, const char* str2) {
  char* ptr = (char*)str1;
  s21_size_t i = 0;

  for (; str1[i]; ++i) {
    if (!s21_strspn(ptr, str2)) {
      ++ptr;
    } else {
      break;
    }
  }
  return i;
}

void s21_reverse_str(char* dest, char* str) {
  s21_size_t len = s21_strlen(str);
  s21_size_t i = 0;
  for (s21_size_t j = len - 1; i < len; i++, j--) {
    dest[i] = str[j];
  }
  dest[i] = '\0';
}

char* s21_convert_from_int(int num) {
  char tmp[20];
  static char result[20];
  int i = 0;
  int n = num;
  while (n != 0) {
    if (n < 0) {
      tmp[i] = (n * (-1) % 10) + '0';
    } else {
      tmp[i] = (n % 10) + '0';
    }
    n /= 10;
    i++;
  }
  if (num < 0) {
    tmp[i++] = '-';
  }
  tmp[i] = '\0';
  s21_reverse_str(result, tmp);
  return result;
}

char* s21_strerror(int errnum) {
  static char res[100] = {0};
  ARRAY;
  int flag = 0;
  for (int i = 0; i < ERR_MAX; i++) {
    if (errnum == i) {
      s21_strcpy(res, errlist[i]);
      flag = -1;
    }
  }
  if (flag == 0) {
    char str[100] = ERROR;
    char* errnum_str = s21_convert_from_int(errnum);
    char* tmp = s21_strcat(str, errnum_str);
    s21_strcpy(res, tmp);
  }
  return res;
}

s21_size_t s21_strlen(const char* str) {
  s21_size_t len = 0;
  if (str) {
    while (str[len]) len++;
  }
  return len;
}

char* s21_strncat(char* dest, const char* src, s21_size_t n) {
  s21_size_t i;
  s21_size_t j;

  i = 0;
  j = s21_strlen(dest);
  while (i < n && src[i]) {
    dest[j + i] = src[i];
    i++;
  }
  dest[j + i] = '\0';
  return (dest);
}

int s21_strncmp(const char* str1, const char* str2, s21_size_t n) {
  int result = 0;
  while ((*str1 || *str2) && n--)
    if (*str1++ != *str2++) {
      result = (unsigned char)*--str1 - (unsigned char)*--str2;
      break;
    }
  return result;
}

char* s21_strncpy(char* dest, const char* src, s21_size_t n) {
  s21_size_t count;

  count = 0;
  while (count < n && src[count] && src && dest) {
    dest[count] = src[count];
    count++;
  }
  while (count < n && dest && src) {
    dest[count] = '\0';
    count++;
  }
  return (dest);
}

char* s21_strpbrk(const char* str1, const char* str2) {
  int flag = 0;
  while (*str1 && !flag) {
    for (s21_size_t i = 0; i < s21_strlen(str2); i++) {
      if (*str1 == str2[i]) {
        flag = 1;
        break;
      }
    }
    if (!flag) str1++;
  }
  return flag ? (char*)str1 : s21_NULL;
}

char* s21_strrchr(const char* str, int c) {
  const char* ptr = str;
  int flag = 0;

  while (*str) {
    str++;
  }
  while (str >= ptr) {
    if (*str == (char)c) {
      flag = 1;
      break;
    }
    str--;
  }
  return flag ? (char*)str : s21_NULL;
}

s21_size_t s21_strspn(const char* str1, const char* str2) {
  s21_size_t flag = 0;
  s21_size_t count = 0;
  for (s21_size_t i = 0; str1[i]; i++) {
    flag = count;
    for (s21_size_t j = 0; str2[j]; j++) {
      if (str1[i] == str2[j]) {
        count++;
        break;
      }
    }
    if (count == flag) break;
  }
  return count;
}

char* s21_strstr(const char* haystack, const char* needle) {
  s21_size_t needle_len = s21_strlen(needle);
  int flag = 1;

  if (needle_len) {
    flag = 0;
    s21_size_t haystack_len = s21_strlen(haystack);
    while (haystack_len-- >= needle_len) {
      if (!s21_memcmp(haystack, needle, needle_len)) {
        flag = 1;
        break;
      }
      haystack++;
    }
  }

  return flag ? (char*)haystack : s21_NULL;
}

char* s21_strtok(char* str, const char* delim) {
  static char* mem = s21_NULL;

  if (str) mem = str;

  if (mem) {
    str = mem + s21_strspn(mem, delim);
    mem = str + s21_strcspn(str, delim);
    if (*mem != '\0') {
      *mem = '\0';
      ++mem;
    }
  }

  // if (*str == '\0') mem = s21_NULL;

  return *str != '\0' ? str : s21_NULL;
  // return mem ? str : s21_NULL;
}
