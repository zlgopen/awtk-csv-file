#include "gtest/gtest.h"
#include "csv_file.h"


TEST(csv_file, row) {
  csv_row_t r;
  
  csv_row_init(&r, (char*)"aaa\0", 4, FALSE);
  ASSERT_EQ(csv_row_count_cols(&r), 1);
  ASSERT_STREQ(csv_row_get(&r, 0), "aaa");
  ASSERT_EQ(csv_row_get(&r, 1) == NULL, true);
  csv_row_reset(&r);

  csv_row_init(&r, (char*)"a\0b\0", 4, FALSE);
  ASSERT_EQ(csv_row_count_cols(&r), 2);
  ASSERT_STREQ(csv_row_get(&r, 0), "a");
  ASSERT_STREQ(csv_row_get(&r, 1), "b");
  csv_row_reset(&r);
}

TEST(csv_file, rows) {
  uint32_t i = 0;
  csv_rows_t rows;
  csv_row_t* r = NULL;
  ASSERT_EQ(csv_rows_init(&rows, 10), RET_OK);

  for(i = 0; i < 100; i++) {
    r = csv_rows_append(&rows);
    ASSERT_EQ(r != NULL, true);
    r->size = i;
    ASSERT_EQ(rows.size, i+1);
    r = csv_rows_get(&rows, i);
    ASSERT_EQ(r->size, i);
  }

  ASSERT_EQ(csv_rows_reset(&rows), RET_OK);
}

TEST(csv_file, rows1) {
  csv_rows_t rows;
  csv_row_t* r = NULL;
  ASSERT_EQ(csv_rows_init(&rows, 10), RET_OK);

  r = csv_rows_insert(&rows, 0);
  r->size = 0;
  
  r = csv_rows_insert(&rows, 0);
  r->size = 1;
  
  r = csv_rows_insert(&rows, 0);
  r->size = 2;
    
  r = csv_rows_get(&rows, 0);
  ASSERT_EQ(r->size, 2);
  
  r = csv_rows_get(&rows, 1);
  ASSERT_EQ(r->size, 1);
  
  r = csv_rows_get(&rows, 2);
  ASSERT_EQ(r->size, 0);

  ASSERT_EQ(csv_rows_remove(&rows, 0), RET_OK);
  r = csv_rows_get(&rows, 0);
  ASSERT_EQ(r->size, 1);

  ASSERT_EQ(csv_rows_reset(&rows), RET_OK);
}

TEST(csv_file, basic) {
  const char* str = "11,12,13\n21,22,23";
  csv_file_t* csv = csv_file_create_with_buff(str, strlen(str), FALSE, ',');
  ASSERT_EQ(csv_file_get_rows(csv), 2);
  ASSERT_EQ(csv_file_get_cols(csv), 3);

  ASSERT_STREQ(csv_file_get(csv, 0, 0), "11");
  ASSERT_STREQ(csv_file_get(csv, 0, 1), "12");
  ASSERT_STREQ(csv_file_get(csv, 0, 2), "13");
  
  ASSERT_STREQ(csv_file_get(csv, 1, 0), "21");
  ASSERT_STREQ(csv_file_get(csv, 1, 1), "22");
  ASSERT_STREQ(csv_file_get(csv, 1, 2), "23");

  csv_file_destroy(csv);
}

TEST(csv_file, title) {
  const char* str = "aa,bb,cc\n11,12,13\n21,22,23";
  csv_file_t* csv = csv_file_create_with_buff(str, strlen(str), FALSE, ',');
  ASSERT_EQ(csv_file_get_rows(csv), 3);
  ASSERT_EQ(csv_file_get_cols(csv), 3);
  
  ASSERT_STREQ(csv_file_get(csv, 0, 0), "aa");
  ASSERT_STREQ(csv_file_get(csv, 0, 1), "bb");
  ASSERT_STREQ(csv_file_get(csv, 0, 2), "cc");

  ASSERT_STREQ(csv_file_get(csv, 1, 0), "11");
  ASSERT_STREQ(csv_file_get(csv, 1, 1), "12");
  ASSERT_STREQ(csv_file_get(csv, 1, 2), "13");
  
  ASSERT_STREQ(csv_file_get(csv, 2, 0), "21");
  ASSERT_STREQ(csv_file_get(csv, 2, 1), "22");
  ASSERT_STREQ(csv_file_get(csv, 2, 2), "23");
  
  ASSERT_STREQ(csv_file_get_by_name(csv, 1, "aa"), "11");
  ASSERT_STREQ(csv_file_get_by_name(csv, 1, "bb"), "12");
  ASSERT_STREQ(csv_file_get_by_name(csv, 1, "cc"), "13");
  
  ASSERT_STREQ(csv_file_get_by_name(csv, 2, "aa"), "21");
  ASSERT_STREQ(csv_file_get_by_name(csv, 2, "bb"), "22");
  ASSERT_STREQ(csv_file_get_by_name(csv, 2, "cc"), "23");

  ASSERT_EQ(csv_file_get_col_of_name(csv, "aa"), 0);
  ASSERT_EQ(csv_file_get_col_of_name(csv, "bb"), 1);
  ASSERT_EQ(csv_file_get_col_of_name(csv, "cc"), 2);

  csv_file_destroy(csv);
}

TEST(csv_file, modify) {
  const char* str = "aa,bb,cc\n";
  csv_file_t* csv = csv_file_create_with_buff(str, strlen(str), FALSE, ',');
  ASSERT_EQ(csv_file_get_rows(csv), 1);
  ASSERT_EQ(csv_file_get_cols(csv), 3);
  ASSERT_EQ(csv->has_title, FALSE);

  csv_file_destroy(csv);
}

TEST(csv_file, main_title) {
  const char* str = "awtk\naa,bb,cc\n";
  csv_file_t* csv = csv_file_create_with_buff(str, strlen(str), FALSE, ',');
  ASSERT_EQ(csv_file_get_rows(csv), 2);
  ASSERT_EQ(csv_file_get_cols(csv), 3);
  ASSERT_EQ(csv->has_title, TRUE);
  ASSERT_STREQ(csv_file_get_title(csv), "awtk");

  csv_file_destroy(csv);
}

