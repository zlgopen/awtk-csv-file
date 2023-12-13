#include "gtest/gtest.h"
#include "tkc/fs.h"
#include "tkc/path.h"
#include "tkc/mem.h"
#include "tkc/utils.h"
#include "csv_file.h"
#include "csv_file_object.h"
#include "csv_row_object.h"

static int32_t compare_by_col0(void* ctx, csv_row_t* row) {
  const char* a = (const char*)ctx;
  const char* b = csv_row_get(row, 0);

  return strcmp(a, b);
}

TEST(csv_row_object, basic1) {
  const char* str = "aa,bb,cc\n11,12,13\n21,22,23";
  csv_file_t* csv = csv_file_create_with_buff(str, strlen(str), ',');
  tk_object_t* obj = csv_file_object_create(csv);
  tk_object_t* robj = csv_row_object_create(obj, "33,32,33");

  ASSERT_EQ(3, tk_object_get_prop_int(obj, TK_OBJECT_PROP_SIZE, 0));
  ASSERT_EQ(tk_object_can_exec(robj, TK_OBJECT_CMD_ADD, NULL), TRUE);
  ASSERT_EQ(tk_object_can_exec(robj, TK_OBJECT_CMD_SAVE, NULL), TRUE);

  ASSERT_EQ(tk_object_get_prop_int(robj, "0", 0), 33);
  ASSERT_EQ(tk_object_get_prop_int(robj, "1", 0), 32);
  ASSERT_EQ(tk_object_get_prop_int(robj, "2", 0), 33);

  ASSERT_EQ(tk_object_set_prop_int(robj, "0", 311), RET_OK);
  ASSERT_EQ(tk_object_set_prop_int(robj, "1", 322), RET_OK);
  ASSERT_EQ(tk_object_set_prop_str(robj, "2", "333"), RET_OK);

  ASSERT_EQ(tk_object_exec(robj, TK_OBJECT_CMD_ADD, NULL), RET_OK);

  ASSERT_EQ(4, tk_object_get_prop_int(obj, TK_OBJECT_PROP_SIZE, 0));

  ASSERT_EQ(tk_object_get_prop_int(obj, "3.0", 0), 311);
  ASSERT_EQ(tk_object_get_prop_int(obj, "3.1", 0), 322);
  ASSERT_EQ(tk_object_get_prop_int(obj, "3.2", 0), 333);

  csv_row_t* row = csv_file_object_find_first(obj, (tk_compare_t)compare_by_col0, (void*)"21");
  ASSERT_EQ(row != NULL, TRUE);
  
  row = csv_file_object_find_first(obj, (tk_compare_t)compare_by_col0, (void*)"311");
  ASSERT_EQ(row != NULL, TRUE);
  ASSERT_STREQ(csv_row_get(row, 0), "311");

  TK_OBJECT_UNREF(obj);
  TK_OBJECT_UNREF(robj);
}
