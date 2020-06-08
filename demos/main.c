/**
 * File:   csv_dump.c
 * Author: AWTK Develop Team
 * Brief:  csv file dump
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

int main(int argc, char* argv[]) {
  uint32_t r = 0;
  uint32_t c = 0;
  const char* filename = argc > 1 ? argv[1] : "./demos/test.csv";
  csv_file_t* csv = csv_file_create(filename, ',');
  uint32_t rows = csv_file_get_rows(csv);
  uint32_t cols = csv_file_get_cols(csv); 

  if(csv->has_title) {
    r++;
    log_debug("title:%s\n", csv_file_get_title(csv));
  }

  for(; r < rows; r++) {
    log_debug("%d: ", r);
    for(c = 0; c < cols; c++) {
      log_debug("%s, ", csv_file_get(csv, r, c));
    }
    log_debug("\n");
  }

  csv_file_destroy(csv);
  return 0;
}
