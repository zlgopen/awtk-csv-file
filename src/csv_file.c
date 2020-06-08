/**
 * File:   csv_file.c
 * Author: AWTK Develop Team
 * Brief:  csv file
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2020-06-08 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "csv_file.h"
#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/fs.h"


static csv_file_t* csv_file_parse(csv_file_t* csv);
static ret_t csv_rows_extend_rows(csv_rows_t* rows, uint32_t delta);

const char* csv_row_get(csv_row_t* row, uint32_t col) {
  uint32_t i = 0;
  uint32_t c = 0;
  return_value_if_fail(row != NULL, NULL);

  while (i < row->size) {
    const char* p = row->buff + i;
    if (c == col) {
      return p;
    }

    c++;
    i += strlen(p) + 1;
  }

  return NULL;
}

int32_t csv_row_get_col(csv_row_t* row, const char* value) {
  uint32_t i = 0;
  uint32_t col = 0;
  return_value_if_fail(row != NULL && value != NULL, -1);

  while (i < row->size) {
    const char* p = row->buff + i;
    if (tk_str_eq(p, value)) {
      return col;
    }

    col++;
    i += strlen(p) + 1;
  }

  return -1;
}

ret_t csv_row_to_str(csv_row_t* row, str_t* str, char sep) {
  uint32_t i = 0;
  uint32_t col = 0;
  return_value_if_fail(row != NULL && str != NULL, RET_BAD_PARAMS);

  str_set(str, "");
  while ((i + 1) < row->size) {
    const char* p = row->buff + i;
    str_append(str, p);
    str_append_char(str, sep);

    i += strlen(p) + 1;
  }

  str_append(str, "\r\n");

  return RET_OK;
}

uint32_t csv_row_count_cols(csv_row_t* row) {
  uint32_t i = 0;
  uint32_t cols = 0;
  return_value_if_fail(row != NULL, 0);

  while (i < row->size) {
    const char* p = row->buff + i;
    cols++;
    i += strlen(p) + 1;
  }

  return cols;
}

ret_t csv_row_set(csv_row_t* row, uint32_t col, const char* value) {
  return_value_if_fail(row != NULL && value != NULL, RET_BAD_PARAMS);

  return RET_OK;
}

ret_t csv_row_init(csv_row_t* row, char* buff, uint32_t size, bool_t should_free_buff) {
  return_value_if_fail(row != NULL && buff != NULL, RET_BAD_PARAMS);

  row->buff = buff;
  row->size = size;
  row->should_free_buff = should_free_buff;

  return RET_OK;
}

static ret_t csv_row_set_data(csv_row_t* row, const char* data, char sep) {
  uint32_t i = 0;
  csv_row_reset(row);

  row->buff = tk_strdup(data);
  return_value_if_fail(row->buff != NULL, RET_OOM);

  row->should_free_buff = TRUE;
  row->size = strlen(data) + 1;
  for(i = 0; i < row->size; i++) {
    if(row->buff[i] == sep) {
      row->buff[i] = '\0';
    }
  }

  return RET_OK;
}

ret_t csv_row_reset(csv_row_t* row) {
  return_value_if_fail(row != NULL, RET_BAD_PARAMS);
  if (row->should_free_buff) {
    TKMEM_FREE(row->buff);
  }
  memset(row, 0x00, sizeof(*row));

  return RET_OK;
}

static ret_t csv_rows_extend_rows(csv_rows_t* rows, uint32_t delta) {
  uint32_t capacity = 0;
  csv_row_t* first = NULL;
  return_value_if_fail(rows != NULL, RET_BAD_PARAMS);

  if ((rows->size + delta) < rows->capacity) {
    return RET_OK;
  }

  first = rows->rows;
  capacity = rows->capacity * 1.5 + delta;
  first = TKMEM_REALLOC(first, capacity * sizeof(csv_row_t));
  return_value_if_fail(first != NULL, RET_OOM);

  rows->rows = first;
  rows->capacity = capacity;

  return RET_OK;
}

ret_t csv_rows_remove(csv_rows_t* rows, uint32_t row) {
  uint32_t i = 0;
  csv_row_t* r = NULL;
  return_value_if_fail(rows != NULL && row < rows->size, RET_BAD_PARAMS);

  r = rows->rows;
  for (i = row; i < rows->size; i++) {
    r[i] = r[i + 1];
  }
  rows->size++;

  return RET_OK;
}

csv_row_t* csv_rows_append(csv_rows_t* rows) {
  csv_row_t* r = NULL;
  return_value_if_fail(csv_rows_extend_rows(rows, 1) == RET_OK, NULL);

  r = rows->rows + rows->size++;
  memset(r, 0x00, sizeof(*r));

  return r;
}

csv_row_t* csv_rows_insert(csv_rows_t* rows, uint32_t row) {
  uint32_t i = 0;
  csv_row_t* r = NULL;
  return_value_if_fail(rows != NULL, NULL);
  return_value_if_fail(csv_rows_extend_rows(rows, 1) == RET_OK, NULL);

  if (row >= rows->size) {
    return csv_rows_append(rows);
  }

  r = rows->rows;
  for (i = rows->size; i > row; i--) {
    r[i] = r[i - 1];
  }
  r = r + row;
  memset(r, 0x00, sizeof(*r));
  rows->size++;

  return r;
}

csv_row_t* csv_rows_get(csv_rows_t* rows, uint32_t row) {
  return_value_if_fail(rows != NULL && row < rows->size, NULL);

  return rows->rows + row;
}

ret_t csv_rows_reset(csv_rows_t* rows) {
  return_value_if_fail(rows != NULL && rows->rows != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(rows->rows);
  memset(rows, 0x00, sizeof(*rows));

  return RET_OK;
}

ret_t csv_rows_init(csv_rows_t* rows, uint32_t init_capacity) {
  return_value_if_fail(rows != NULL, RET_BAD_PARAMS);

  init_capacity = tk_max(10, init_capacity);

  rows->size = 0;
  rows->capacity = init_capacity;
  rows->rows = TKMEM_ZALLOCN(csv_row_t, init_capacity);

  return rows->rows != NULL ? RET_OK : RET_OOM;
}

csv_file_t* csv_file_create(const char* filename, char sep) {
  uint32_t size = 0;
  void* buff = NULL;
  return_value_if_fail(filename != NULL, NULL);

  buff = file_read(filename, &size);
  return_value_if_fail(buff != NULL, NULL);

  return csv_file_create_with_buff(buff, size, TRUE, sep);
}

csv_file_t* csv_file_create_with_buff(const char* buff, uint32_t size, bool_t should_free, char sep) {
  csv_file_t* csv = NULL;
  return_value_if_fail(buff != NULL, NULL);

  csv = TKMEM_ZALLOC(csv_file_t);
  if (csv == NULL) {
    if (should_free) {
      TKMEM_FREE(buff);
    }
    return_value_if_fail(csv != NULL, NULL);
  }

  csv->sep = sep;
  csv->size = size;
  if (!should_free) {
    csv->buff = TKMEM_ALLOC(size + 1);
    if (csv->buff == NULL) {
      TKMEM_FREE(csv);
    }
    return_value_if_fail(csv != NULL, NULL);
    memcpy(csv->buff, buff, size);
    csv->buff[size] = '\0';
  } else {
    csv->buff = (char*)buff;
  }

  return csv_file_parse(csv);
}

static uint32_t csv_file_count_rows(csv_file_t* csv) {
  uint32_t i = 0;
  uint32_t rows = 0;
  const char* p = csv->buff;

  for (i = 0; i < csv->size; i++, p++) {
    if (*p == '\r' || *p == '\n') {
      rows++;
      while ((*p == '\r' || *p == '\n') && i < csv->size) {
        p++;
        i++;
      }
    }
  }

  return rows + 1;
}

static csv_file_t* csv_file_parse(csv_file_t* csv) {
  uint32_t i = 0;
  csv_row_t* r = NULL;
  char* p = csv->buff;
  uint32_t rows = csv_file_count_rows(csv);

  return_value_if_fail(csv_rows_init(&(csv->rows), rows) == RET_OK, NULL);

  r = csv_rows_append(&(csv->rows));
  return_value_if_fail(r != NULL, NULL);

  r->buff = p;
  for (i = 0; i < csv->size; i++, p++) {
    if (*p == '\r' || *p == '\n') {
      r->size = p - r->buff + 1;
      while ((*p == '\r' || *p == '\n' || *p == '\0') && i < csv->size) {
        *p = '\0';
        p++;
        i++;
      }

      r = csv_rows_append(&(csv->rows));
      return_value_if_fail(r != NULL, NULL);
      r->buff = p;
    } else if (*p == csv->sep) {
      *p = '\0';
    }
  }

  if (r->size == 0) {
    r->size = p - r->buff + 1;
    if (r->size <= 2 && *r->buff == '\0') {
      csv->rows.size--;
    }
  }

  r = csv_file_get_row(csv, 0);
  if (r != NULL) {
    uint32_t cols1 = 0;
    uint32_t cols0 = csv_row_count_cols(r);
    if (cols0 == 1) {
      r = csv_file_get_row(csv, 1);
      cols1 = csv_row_count_cols(r);
      if (cols1 > cols0) {
        csv->has_title = TRUE;
      }
      csv->cols = tk_max(cols0, cols1);
    } else {
      csv->cols = csv_row_count_cols(r);
    }
  }

  return csv;
}

const char* csv_file_get(csv_file_t* csv, uint32_t row, uint32_t col) {
  csv_row_t* r = csv_file_get_row(csv, row);
  return_value_if_fail(r != NULL, NULL);
  return_value_if_fail(col < csv_file_get_cols(csv), NULL);

  return csv_row_get(r, col);
}

int32_t csv_file_get_col_of_name(csv_file_t* csv, const char* name) {
  return csv_row_get_col(csv_file_get_row(csv, 0), name);
}

const char* csv_file_get_by_name(csv_file_t* csv, uint32_t row, const char* name) {
  int32_t col = csv_file_get_col_of_name(csv, name);
  return_value_if_fail(col >= 0, NULL);

  return csv_file_get(csv, row, col);
}

uint32_t csv_file_get_rows(csv_file_t* csv) {
  return_value_if_fail(csv != NULL, 0);

  return csv->rows.size;
}

uint32_t csv_file_get_cols(csv_file_t* csv) {
  return_value_if_fail(csv != NULL, 0);

  return csv->cols;
}

ret_t csv_file_set(csv_file_t* csv, uint32_t row, uint32_t col, const char* value) {
  csv_row_t* r = csv_file_get_row(csv, row);
  return_value_if_fail(r != NULL, RET_BAD_PARAMS);
  return_value_if_fail(col < csv_file_get_cols(csv), RET_BAD_PARAMS);

  return csv_row_set(r, col, value);
}

csv_row_t* csv_file_get_row(csv_file_t* csv, uint32_t row) {
  return_value_if_fail(csv != NULL, NULL);

  return csv_rows_get(&(csv->rows), row);
}

ret_t csv_file_insert_row(csv_file_t* csv, uint32_t row, const char* data) {
  csv_row_t* r = NULL;
  return_value_if_fail(csv != NULL && data != NULL, RET_BAD_PARAMS);

  r = csv_rows_insert(&(csv->rows), row);
  return_value_if_fail(r != NULL, RET_OOM);
  
  return csv_row_set_data(r, data, csv->sep);
}

ret_t csv_file_append_row(csv_file_t* csv, const char* data) {
  csv_row_t* r = NULL;
  return_value_if_fail(csv != NULL && data != NULL, RET_BAD_PARAMS);

  r = csv_rows_append(&(csv->rows));
  return_value_if_fail(r != NULL, RET_OOM);

  return csv_row_set_data(r, data, csv->sep);
}

ret_t csv_file_remove_row(csv_file_t* csv, uint32_t row) {
  return_value_if_fail(csv != NULL, RET_BAD_PARAMS);

  return csv_rows_remove(&(csv->rows), row);
}

ret_t csv_file_save(csv_file_t* csv, const char* filename) {
  str_t str;
  uint32_t i = 0;
  csv_row_t* r = NULL;
  fs_file_t* f = NULL;
  return_value_if_fail(csv != NULL && filename != NULL, RET_BAD_PARAMS);
  return_value_if_fail(str_init(&str, 512) != NULL, RET_OOM);

  f = fs_open_file(os_fs(), filename, "wb+");
  if(f != NULL) {
    for(i = 0; i < csv->rows.size; i++) {
      r = csv->rows.rows + i;
      csv_row_to_str(r, &str, csv->sep);
      ENSURE(fs_file_write(f, str.str, str.size) == str.size);
    }
    fs_file_close(f);
  }
  str_reset(&str);

  return RET_NOT_IMPL;
}

const char* csv_file_get_title(csv_file_t* csv) {
  csv_row_t* r = NULL;
  return_value_if_fail(csv != NULL && csv->has_title, NULL);

  r = csv_file_get_row(csv, 0);
  return_value_if_fail(r != NULL, NULL);

  return r->buff;
}

ret_t csv_file_destroy(csv_file_t* csv) {
  uint32_t i = 0;
  return_value_if_fail(csv != NULL && csv->buff != NULL, RET_BAD_PARAMS);

  for (i = 0; i < csv->rows.size; i++) {
    csv_row_t* r = csv->rows.rows + i;
    csv_row_reset(r);
  }

  TKMEM_FREE(csv->rows.rows);
  TKMEM_FREE(csv->buff);

  memset(csv, 0x00, sizeof(*csv));

  return RET_OK;
}
