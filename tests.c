#ifdef TEST_CPREPROCESS1

#include "cpreprocess.h"
#include <assert.h>
#include <string.h>

int main(int argc, char *argv[]) {
  char *sample1 = "#include <stdio.h>\n"
    "int main(int argc, char *argv[]) {\n"
    "  char *something = \"23\";\n"
    "  char character = something?""?(0?""?);\n"
    "}\n";
  char *expected1 = "#include <stdio.h>\n"
    "int main(int argc, char *argv[]) {\n"
    "  char *something = \"23\";\n"
    "  char character = something[0];\n"
    "}\n";
  assert(strcmp(preprocess(sample1), expected1) == 0);

  char *sample2 = "char *some\\\nthing = \"23\";";
  char *expected2= "char *something = \"23\";";
  assert(strcmp(preprocess(sample2), expected2) == 0);
}

#endif
