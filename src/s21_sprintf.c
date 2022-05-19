#include "s21_string.h"

#define BUFF_SIZE 256

struct p_flag {
  s21_size_t pluses;
  s21_size_t minuses;
  s21_size_t spaces;
  s21_size_t length;
  s21_size_t dot;
  s21_size_t precision;
} p_flag; /* создали экземпляр структуры, заполняется в check_flags */

typedef struct P_Token {
  int width;
  int precision;
  int pres_flag;
  int str_len;
} P_Token;

P_Token init_token();
void init_parameters(P_Token *t, int len);
void init_flags();

int is_dec(char c);
s21_size_t find_digit(long long number);

void s21_clear(char *str);
int check_specs(char c);
int check_string(const char *str);
char *s21_parser(const char *format, char *token_str, int *counter);
void menu(char *str, char *token_str, va_list arg_ptr);
void add_char(char *str, const char *format);
void check_flags(char *token_str);
void read_nostar_width_pres(char *token_str, P_Token *t);
void fill_width(char *str, char *copy, P_Token t);
void round_float(char *copy, int *flag);

void processing_d_and_u(char *str, char *token_str, va_list arg_ptr, P_Token t,
                        char ch);
void processing_f(char *str, va_list arg_ptr, P_Token t);
void processing_s(char *str, char *token_str, va_list arg_ptr, P_Token t);
void processing_c(char *str, char *token_str, va_list arg_ptr, P_Token t);

void add_zero_prec_d(char *copy, P_Token t);
void int_to_char(char *copy, long long number);
void float_to_char(char *copy, double number, s21_size_t accuracy);
void get_width_pres(char *token_str, va_list arg_ptr, P_Token *t);
void string_to_char(char *copy, char *tmp, P_Token t);
void wchart_to_string(char *buf, const wchar_t *tmp);

int s21_sprintf(char *str, const char *format, ...) {
  int result = 0;
  int counter = 0;

  str[0] = '\0';
  va_list arg_ptr;
  va_start(arg_ptr, format);
  while (*format) {
    if (*format != '%') {
      add_char(str, format);
      format++;
    } else if (*(format + 1) == '%') {
      add_char(str, format);
      format += 2;
    } else {
      if (!check_string(format)) {  // проверяет всю строку после символа % на
                                    //  наличие одного из спецификаторов
        break;
      }
      char token_str[BUFF_SIZE] = {'\0'};

      s21_parser(format, token_str, &counter);
      check_flags(token_str);
      menu(str, token_str, arg_ptr);
      format += counter;  // размер сдвига рассчитывается в s21_parser, когда мы
                          //  заполняем буффер
      counter = 0;
    }
  }
  va_end(arg_ptr);
  result = s21_strlen(str);
  return result;
}

void add_char(char *str, const char *format) {
  char c[2] = {0};
  c[0] = *format;
  s21_strcat(str, c);
}

/*  сравнивает символ со строкой спецификаторов, используется в check_string */
int check_specs(char c) {
  int res = 0;
  char *specs = "difscu";
  for (int i = 0; specs[i]; i++) {
    if (specs[i] == c) {
      res = 1;
      break;
    }
  }
  return res;
}

/* проверка строки на символ-спецификатор */
int check_string(const char *str) {
  int res = 0, i = 1;
  while (str[i] != '\0') {
    if (check_specs(str[i])) {
      res = 1;
      break;
    }
    i++;
  }
  return res;
}

/* заполняет наш буффер флагами и т.д., пока не встретит в исходной строке
спецификатор он его точно должен встретить, так как до вызова этой ф-ии у нас
была проверка на его наличие после символа % */
char *s21_parser(const char *format, char *token_str, int *counter) {
  int i = 0;
  while (*format) {
    if (check_specs(*format)) {
      *counter += 1;
      token_str[i] = *format;
      token_str[i + 1] = '\0';
      break;
    }
    *counter += 1;
    token_str[i] = *format;
    format++;
    i++;
  }
  token_str[i] = *format;
  token_str[i + 1] = '\0';
  return token_str;
}

void init_flags() {
  p_flag.pluses = 0;
  p_flag.minuses = 0;
  p_flag.spaces = 0;
  p_flag.length = 0;
  p_flag.dot = 0;
  p_flag.precision = 0;
}

/* смотрит наш буфер до спецификатора и заполняет экземпляр структуры
 * соответств. флагами */
void check_flags(char *token_str) {
  init_flags();
  int i = 0;
  while (!check_specs(token_str[i])) {
    if (token_str[i] == '+') {
      p_flag.pluses = 1;
    }
    if (token_str[i] == '-') {
      p_flag.minuses = 1;
    }
    if (token_str[i] == ' ') {
      p_flag.spaces = 1;
    }
    if (token_str[i] == 'l' || token_str[i] == 'L' || token_str[i] == 'h') {
      p_flag.length = 1;
    }
    if (token_str[i] == '.') {
      p_flag.dot = 1;
    }
    i++;
  }
}

void menu(char *str, char *token_str, va_list arg_ptr) {
  P_Token t = init_token();
  read_nostar_width_pres(token_str, &t);
  t.width = t.width < 0 ? t.width * (-1) : t.width;
  int len = s21_strlen(token_str);

  switch (token_str[len - 1]) {
    case 'u':
      processing_d_and_u(str, token_str, arg_ptr, t, 'u');
      break;
    case 'd':
    case 'i':
      processing_d_and_u(str, token_str, arg_ptr, t, 'd');
      break;
    case 'f':
      processing_f(str, arg_ptr, t);
      break;
    case 's':
      processing_s(str, token_str, arg_ptr, t);
      break;
    case 'c':
      processing_c(str, token_str, arg_ptr, t);
      break;
  }
}

P_Token init_token() {
  P_Token t;
  t.width = 0;
  t.precision = 0;
  t.pres_flag = 0;
  t.str_len = 0;
  return t;
}

void read_nostar_width_pres(char *token_str, P_Token *t) {
  ++token_str;
  while (*token_str) {
    if (is_dec(*token_str) && *(token_str - 1) != '.') {
      t->width =
          !p_flag.minuses ? s21_atoi(token_str) : s21_atoi(token_str) * (-1);
      int w_len = find_digit(t->width);
      token_str += w_len;
    }
    if (*token_str == '.') {
      p_flag.precision = 1;
      if (is_dec(*(token_str + 1))) {
        while (*(token_str + 1) == '0') {
          ++token_str;
        }
        if (is_dec(*(token_str + 1))) {
          t->precision = s21_atoi(token_str + 1);
          int p_len = find_digit(t->precision) + 1;
          token_str += p_len + 1;
        } else {
          t->precision = 0;
          token_str += 3;
        }
      } else {
        t->precision = 0;
        token_str += 3;
      }
    } else {
      ++token_str;
    }
  }
}

int is_dec(char c) { return c >= '0' && c <= '9'; }

s21_size_t find_digit(long long number) {
  s21_size_t k = 0;

  while (number != 0) {
    number /= 10;
    ++k;
  }
  k += (k == 0) ? 1 : 0;
  return k;
}

void processing_d_and_u(char *str, char *token_str, va_list arg_ptr, P_Token t,
                        char ch) {
  int token_len = s21_strlen(token_str);
  long d_number = 0;
  unsigned long u_number = 0;
  char copy[1000] = {'\0'};
  switch (ch) {
    case 'd':
      if (p_flag.length) {
        if (token_str[token_len - 2] == 'l') {
          d_number = va_arg(arg_ptr, long);
        }
        if (token_str[token_len - 2] == 'h') {
          d_number = (short)va_arg(arg_ptr, int);
        }
      } else {
        d_number = va_arg(arg_ptr, int);
      }
      break;
    case 'u':
      if (p_flag.length) {
        if (token_str[token_len - 2] == 'l') {
          u_number = va_arg(arg_ptr, long unsigned);
        }
        if (token_str[token_len - 2] == 'h') {
          u_number = (short unsigned)va_arg(arg_ptr, unsigned int);
        }
      } else {
        u_number = va_arg(arg_ptr, int unsigned);
      }
      break;
  }

  if (t.precision == 0 && p_flag.precision == 1 && d_number == 0 &&
      u_number == 0) {
  } else if (ch == 'd') {
    int_to_char(copy, d_number);
    add_zero_prec_d(copy, t);
  } else if (ch == 'u') {
    int_to_char(copy, (long long)u_number);
    add_zero_prec_d(copy, t);
  }
  fill_width(str, copy, t);
}

void add_zero_prec_d(char *copy, P_Token t) {
  s21_size_t len = s21_strlen(copy);
  t.precision -= len;
  t.precision = t.precision < 0 ? 0 : t.precision;
  if (t.precision) {
    if (is_dec(*copy)) {
      s21_memmove(copy + t.precision, copy, len);
      for (int i = 0; i < t.precision; i++) {
        copy[i] = '0';
      }
    } else {
      ++t.precision;
      s21_memmove(copy + t.precision + 1, copy + 1, len - 1);
      for (int i = 1; i < t.precision + 1; i++) {
        copy[i] = '0';
      }
    }
  }
}

void fill_width(char *str, char *copy, P_Token t) {
  s21_size_t len = s21_strlen(copy);

  if (t.width > (int)s21_strlen(copy)) {
    for (s21_size_t i = 0; i < t.width - len; ++i) {
      if (!p_flag.minuses) {
        if (i == 0) {
          s21_memmove(copy + t.width - len, copy, len);
        }
        copy[i] = ' ';
      } else {
        s21_strcat(copy, " ");
      }
    }
  }
  s21_strcat(str, copy);
}

void int_to_char(char *copy, long long number) {
  s21_size_t digit = find_digit(number);

  for (s21_size_t j = digit; j > 0; --j) {
    if (number < 0) {
      if (j == 1) {
        copy[j - 1] = '-';
      }
      copy[j] = (char)((number % 10) * -1 + 48);
    } else {
      if (p_flag.pluses) {
        if (j == 1) {
          copy[j - 1] = '+';
        }
        copy[j] = (char)(number % 10 + 48);
      } else if (p_flag.spaces) {
        if (j == 1) {
          copy[j - 1] = ' ';
        }
        copy[j] = (char)(number % 10 + 48);
      } else {
        copy[j - 1] = (char)(number % 10 + 48);
      }
    }
    number /= 10;
  }
}

void processing_f(char *str, va_list arg_ptr, P_Token t) {
  double number;
  char copy[1000] = {'\0'};

  number = va_arg(arg_ptr, double);
  float_to_char(copy, number, t.precision);
  fill_width(str, copy, t);
}

void float_to_char(char *copy, double number, s21_size_t accuracy) {
  s21_size_t dot = 0;
  long long numberInt;
  int flag = 0;

  if (number < 0) {
    *copy = '-';
    ++copy;
    number *= -1;
    p_flag.pluses = 0;
    p_flag.spaces = 0;
  }
  numberInt = (long long)number;
  int_to_char(copy, numberInt);
  dot = s21_strlen(copy);
  number -= numberInt;
  if (p_flag.precision == 0) {
    accuracy += 6;
  }
  for (s21_size_t power = 1; power != accuracy + 2; ++power) {
    number *= 10.0;
    numberInt = (long long)number;
    copy[dot + power] = numberInt + 48;
    number -= numberInt;
  }
  copy[dot] = '.';
  round_float(copy, &flag);
  if (!accuracy) {
    if (flag)
      copy[dot + 1] = '\0';
    else
      copy[dot] = '\0';
  }
}

void round_float(char *copy, int *flag) {
  if (copy[s21_strlen(copy) - 1] > '4') {
    for (int i = (int)s21_strlen(copy) - 2; i >= 0; --i) {
      if (copy[i] == '.') {
        --i;
      }
      if (copy[i] != '9') {
        copy[i] = copy[i] + 1;
        break;
      } else if (i != 0) {
        copy[i] = '0';
      } else if (copy[i] != '0') {
        *flag = 1;
        for (i = s21_strlen(copy); i > 0; --i) {
          copy[i] = copy[i - 1];
        }
        copy[1] = '0';
        copy[0] = '1';
        break;
      } else {
        copy[i] = '1';
      }
    }
  }
  copy[s21_strlen(copy) - 1] = '\0';
}

void processing_s(char *str, char *token_str, va_list arg_ptr, P_Token t) {
  char copy[1000] = {0};
  char buf[1000] = {0};
  int token_len = s21_strlen(token_str);
  char *c = 0;
  wchar_t *tmp = 0;

  if (token_str[token_len - 2] == 'l') {
    tmp = va_arg(arg_ptr, wchar_t *);
    wchart_to_string(buf, tmp);
    c = buf;
  } else {
    c = va_arg(arg_ptr, char *);
  }

  string_to_char(copy, c, t);
  fill_width(str, copy, t);
}

void processing_c(char *str, char *token_str, va_list arg_ptr, P_Token t) {
  char copy[1000] = {0};
  int token_len = s21_strlen(token_str);
  char ch;
  wchar_t wch;

  if (token_str[token_len - 2] == 'l') {
    wch = va_arg(arg_ptr, wchar_t);
    copy[0] = wch;
  } else {
    ch = (char)va_arg(arg_ptr, int);
    copy[0] = ch;
  }
  fill_width(str, copy, t);
}

void string_to_char(char *copy, char *tmp, P_Token t) {
  if (t.precision != 0)
    s21_memcpy(copy, tmp, t.precision);
  else if (p_flag.precision != 1)
    s21_strcpy(copy, tmp);
}

void wchart_to_string(char *buf, const wchar_t *tmp) {
  int i = 0;
  while (tmp[i]) {
    buf[i] = tmp[i];
    i++;
  }
  buf[i] = '\0';
}
