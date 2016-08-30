#include <stdio.h>
#include "./vendor/json/libjson/libjson.h"

int main(int argc, char **argv) {
  JSONNODE *n = json_new(JSON_NODE);
  json_push_back(n, json_new_a("String Node", "String Value"));
  json_push_back(n, json_new_i("Integer Node", 42));
  json_push_back(n, json_new_f("Floating Point Node", 3.14));
  json_push_back(n, json_new_b("Boolean Node", 1));

  JSONNODE *c = json_new(JSON_ARRAY);
  json_set_name(c, "ArrayOfNumbers");
  json_push_back(c, json_new_i(NULL, 16));
  json_push_back(c, json_new_i(NULL, 42));
  json_push_back(c, json_new_i(NULL, 128));
  json_push_back(n, c);

  json_char *jc = json_write_formatted(n);
  printf("%s\n", jc);
  json_free(jc);

  json_delete(n);

  return 0;
}
