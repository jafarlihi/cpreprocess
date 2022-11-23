#ifdef TEST_CPREPROCESS1

#include "cpreprocess.h"
#include <assert.h>
#include <string.h>

int main(int argc, char *argv[]) {
  char *sample1 = "int main(int argc, char *argv[]) {\n"
    "  char *something = \"23\";\n"
    "  char character = something?""?(0?""?);\n"
    "}\n";
  char *expected1 = "int main(int argc, char *argv[]) {\n"
    "  char *something = \"23\";\n"
    "  char character = something[0];\n"
    "}\n";
  assert(strcmp(preprocess(sample1), expected1) == 0);

  char *sample2 = "char *some\\\nthing = \"23\";";
  char *expected2= "char *something = \"23\";";
  assert(strcmp(preprocess(sample2), expected2) == 0);

  char *sample3 = "int something = 23; // comment1\n"
    "int somethingElse = 55;";
  char *expected3 = "int something = 23;  \n"
    "int somethingElse = 55;\n";
  assert(strcmp(preprocess(sample3), expected3) == 0);

  char *sample4 = "int something = 24; // comment1\n"
    "/* block\n"
    "comment */";
  char *expected4= "int something = 24;  \n"
    " \n";
  assert(strcmp(preprocess(sample4), expected4) == 0);

  char *sample5 = "int something = 24; // comment1\n"
    "/* block\n"
    "comment */ abc";
  char *expected5= "int something = 24;  \n"
    " \n"
    " abc\n";
  assert(strcmp(preprocess(sample5), expected5) == 0);
}

#endif
