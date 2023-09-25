#include "gtest/gtest.h"
#include "tkc/fs.h"
#include "tkc/path.h"
#include "tkc/mem.h"
#include "csv_file.h"
#include "csv_file_object.h"

TEST(csv_file_object, basic) {
  const char* str = "aa,bb,cc\n11,12,13\n21,22,23";
  csv_file_t* csv = csv_file_create_with_buff(str, strlen(str), ',');
  object_t* obj = csv_file_object_create(csv);

  ASSERT_EQ(object_get_prop_int(obj, "#size", 0), 3);
  ASSERT_STREQ(object_get_prop_str(obj, "[0].[0]"), "aa");
  ASSERT_STREQ(object_get_prop_str(obj, "[0].[1]"), "bb");
  ASSERT_STREQ(object_get_prop_str(obj, "[0].[2]"), "cc");
  
  ASSERT_STREQ(object_get_prop_str(obj, "[1].[0]"), "11");
  ASSERT_STREQ(object_get_prop_str(obj, "[1].[1]"), "12");
  ASSERT_STREQ(object_get_prop_str(obj, "[1].[2]"), "13");
  
  ASSERT_EQ(object_set_prop_int(obj, "[1].[0]", 111), RET_OK);
  ASSERT_EQ(object_set_prop_int(obj, "[1].[1]", 122), RET_OK);
  ASSERT_EQ(object_set_prop_int(obj, "[1].[2]", 133), RET_OK);
  
  ASSERT_STREQ(object_get_prop_str(obj, "[1].[0]"), "111");
  ASSERT_STREQ(object_get_prop_str(obj, "[1].[1]"), "122");
  ASSERT_STREQ(object_get_prop_str(obj, "[1].[2]"), "133");
  
  ASSERT_EQ(object_remove_prop(obj, "[1]"), RET_OK);
  ASSERT_EQ(object_get_prop_int(obj, "#size", 0), 2);
  
  ASSERT_EQ(object_exec(obj, OBJECT_CMD_CLEAR, NULL), RET_OK);
  ASSERT_EQ(object_get_prop_int(obj, "#size", 0), 0);
  
  ASSERT_EQ(object_exec(obj, OBJECT_CMD_ADD, "11,12,13"), RET_OK);
  ASSERT_EQ(object_get_prop_int(obj, "#size", 0), 1);
  
  ASSERT_EQ(object_exec(obj, OBJECT_CMD_ADD, "21,22,23"), RET_OK);
  ASSERT_EQ(object_get_prop_int(obj, "#size", 0), 2);
  
  ASSERT_EQ(object_exec(obj, OBJECT_CMD_REMOVE, "0"), RET_OK);
  ASSERT_EQ(object_get_prop_int(obj, "#size", 0), 1);

  OBJECT_UNREF(obj);
}


TEST(csv_file_object, title ) {
  const char* str = "aa,bb,cc\n11,12,13\n21,22,23";
  csv_file_t* csv = csv_file_create_with_buff(str, strlen(str), ',');
  object_t* obj = csv_file_object_create(csv);

  ASSERT_EQ(object_get_prop_int(obj, "#size", 0), 3);
  ASSERT_STREQ(object_get_prop_str(obj, "[0].aa"), "aa");
  ASSERT_STREQ(object_get_prop_str(obj, "[0].bb"), "bb");
  ASSERT_STREQ(object_get_prop_str(obj, "[0].cc"), "cc");
  
  ASSERT_STREQ(object_get_prop_str(obj, "[1].aa"), "11");
  ASSERT_STREQ(object_get_prop_str(obj, "[1].bb"), "12");
  ASSERT_STREQ(object_get_prop_str(obj, "[1].cc"), "13");
  
  ASSERT_EQ(object_set_prop_int(obj, "[1].aa", 111), RET_OK);
  ASSERT_EQ(object_set_prop_int(obj, "[1].bb", 122), RET_OK);
  ASSERT_EQ(object_set_prop_int(obj, "[1].cc", 133), RET_OK);
  
  ASSERT_STREQ(object_get_prop_str(obj, "[1].aa"), "111");
  ASSERT_STREQ(object_get_prop_str(obj, "[1].bb"), "122");
  ASSERT_STREQ(object_get_prop_str(obj, "[1].cc"), "133");
  
  ASSERT_EQ(object_remove_prop(obj, "[1]"), RET_OK);
  ASSERT_EQ(object_get_prop_int(obj, "#size", 0), 2);
  
  ASSERT_EQ(object_exec(obj, OBJECT_CMD_CLEAR, NULL), RET_OK);
  ASSERT_EQ(object_get_prop_int(obj, "#size", 0), 0);
  
  ASSERT_EQ(object_exec(obj, OBJECT_CMD_ADD, "11,12,13"), RET_OK);
  ASSERT_EQ(object_get_prop_int(obj, "#size", 0), 1);
  
  ASSERT_EQ(object_exec(obj, OBJECT_CMD_ADD, "21,22,23"), RET_OK);
  ASSERT_EQ(object_get_prop_int(obj, "#size", 0), 2);
  
  ASSERT_EQ(object_exec(obj, OBJECT_CMD_REMOVE, "[0]"), RET_OK);
  ASSERT_EQ(object_get_prop_int(obj, "#size", 0), 1);
  
  ASSERT_EQ(object_exec(obj, OBJECT_CMD_ADD, "31,32,33"), RET_OK);
  ASSERT_EQ(object_get_prop_int(obj, "#size", 0), 2);
  
  ASSERT_EQ(object_exec(obj, OBJECT_CMD_SAVE, "test1.csv"), RET_OK);
  
  ASSERT_EQ(object_exec(obj, OBJECT_CMD_REMOVE, "0"), RET_OK);
  ASSERT_EQ(object_get_prop_int(obj, "#size", 0), 1);
  
  ASSERT_EQ(object_exec(obj, OBJECT_CMD_RELOAD, NULL), RET_OK);
  ASSERT_EQ(object_get_prop_int(obj, "#size", 0), 1);

  OBJECT_UNREF(obj);
}

TEST(csv_file_object, checked) {
  const char* str = "aa,bb,cc\n11,12,13\n21,22,23\n31,32,33\n41,42,43";
  csv_file_t* csv = csv_file_create_with_buff(str, strlen(str), ',');
  object_t* obj = csv_file_object_create(csv);

  ASSERT_EQ(object_get_prop_int(obj, "#size", 0), 5);
  ASSERT_EQ(object_set_prop_bool(obj, "[0].checked", TRUE), RET_OK);
  ASSERT_EQ(object_get_prop_bool(obj, "[0].checked", FALSE), TRUE);

  ASSERT_EQ(object_set_prop_bool(obj, "[2].checked", TRUE), RET_OK);
  ASSERT_EQ(object_get_prop_bool(obj, "[2].checked", FALSE), TRUE);

  ASSERT_EQ(object_set_prop_bool(obj, "[4].checked", TRUE), RET_OK);
  ASSERT_EQ(object_get_prop_bool(obj, "[4].checked", FALSE), TRUE);
  
  ASSERT_EQ(object_exec(obj, OBJECT_CMD_REMOVE_CHECKED, NULL), RET_OK);
  ASSERT_EQ(object_get_prop_int(obj, "#size", 0), 2);
  
  ASSERT_STREQ(object_get_prop_str(obj, "[0].0"), "11");
  ASSERT_STREQ(object_get_prop_str(obj, "[0].1"), "12");
  ASSERT_STREQ(object_get_prop_str(obj, "[0].2"), "13");
  
  ASSERT_STREQ(object_get_prop_str(obj, "[1].0"), "31");
  ASSERT_STREQ(object_get_prop_str(obj, "[1].1"), "32");
  ASSERT_STREQ(object_get_prop_str(obj, "[1].2"), "33");
  
  ASSERT_EQ(object_exec(obj, OBJECT_CMD_REMOVE_CHECKED, NULL), RET_OK);
  ASSERT_EQ(object_get_prop_int(obj, "#size", 0), 2);

  ASSERT_EQ(object_set_prop_bool(obj, "[0].checked", TRUE), RET_OK);
  ASSERT_EQ(object_get_prop_bool(obj, "[0].checked", FALSE), TRUE);
  ASSERT_EQ(object_set_prop_bool(obj, "[1].checked", TRUE), RET_OK);
  ASSERT_EQ(object_get_prop_bool(obj, "[1].checked", FALSE), TRUE);
  
  ASSERT_EQ(object_exec(obj, OBJECT_CMD_REMOVE_CHECKED, NULL), RET_OK);
  ASSERT_EQ(object_get_prop_int(obj, "#size", 0), 0);
  ASSERT_EQ(csv_file_object_get_csv(obj), csv);

  OBJECT_UNREF(obj);
}

TEST(csv_file_object, load_from_buff) {
  wbuffer_t wb;
  const char* str = "aa,bb,cc\n11,12,13\n21,22,23\n31,32,33\n41,42,43";
  object_t* obj = csv_file_object_load_from_buff(str, strlen(str), ',');

  wbuffer_init_extendable(&wb);
  
  ASSERT_STREQ(object_get_prop_str(obj, "[0].aa"), "aa");
  ASSERT_STREQ(object_get_prop_str(obj, "[0].bb"), "bb");
  ASSERT_STREQ(object_get_prop_str(obj, "[0].cc"), "cc");
  
  ASSERT_STREQ(object_get_prop_str(obj, "[1].aa"), "11");
  ASSERT_STREQ(object_get_prop_str(obj, "[1].bb"), "12");
  ASSERT_STREQ(object_get_prop_str(obj, "[1].cc"), "13");

  csv_file_object_save_to_buff(obj, &wb);
  ASSERT_STREQ((char*)(wb.data), "aa,bb,cc\r\n11,12,13\r\n21,22,23\r\n31,32,33\r\n41,42,43\r\n");

  wbuffer_deinit(&wb);

  OBJECT_UNREF(obj);
}

TEST(csv_file_object, load_from_file) {
  object_t* obj = NULL;
  char filename[MAX_PATH+1] = {0};
  const char* str = "aa,bb,cc\n11,12,13\n21,22,23\n31,32,33\n41,42,43";

  path_prepend_temp_path(filename, "test.csv");
  file_write(filename, str, strlen(str));

  obj = csv_file_object_load(filename, ',');

  ASSERT_STREQ(object_get_prop_str(obj, "[0].aa"), "aa");
  ASSERT_STREQ(object_get_prop_str(obj, "[0].bb"), "bb");
  ASSERT_STREQ(object_get_prop_str(obj, "[0].cc"), "cc");
  
  ASSERT_STREQ(object_get_prop_str(obj, "[1].aa"), "11");
  ASSERT_STREQ(object_get_prop_str(obj, "[1].bb"), "12");
  ASSERT_STREQ(object_get_prop_str(obj, "[1].cc"), "13");

  fs_remove_file(os_fs(), filename);
  ASSERT_EQ(csv_file_object_save_as(obj, filename), RET_OK);
  OBJECT_UNREF(obj);

  obj = csv_file_object_load(filename, ',');
  ASSERT_STREQ(object_get_prop_str(obj, "[0].aa"), "aa");
  ASSERT_STREQ(object_get_prop_str(obj, "[0].bb"), "bb");
  ASSERT_STREQ(object_get_prop_str(obj, "[0].cc"), "cc");
  
  ASSERT_STREQ(object_get_prop_str(obj, "[1].aa"), "11");
  ASSERT_STREQ(object_get_prop_str(obj, "[1].bb"), "12");
  ASSERT_STREQ(object_get_prop_str(obj, "[1].cc"), "13");

  OBJECT_UNREF(obj);

  fs_remove_file(os_fs(), filename);

}
