#include "cpreprocess.h"
#include <string.h>
#include <stdlib.h>

char *strReplace(char *orig, char *rep, char *with) {
  char *result;
  char *ins;
  char *tmp;
  int lenRep;
  int lenWith;
  int lenFront;
  int count;

  if (!orig || !rep)
    return NULL;
  lenRep = strlen(rep);
  if (lenRep == 0)
    return NULL;
  if (!with)
    with = "";
  lenWith = strlen(with);

  ins = orig;
  for (count = 0; tmp = strstr(ins, rep); ++count) {
    ins = tmp + lenRep;
  }

  tmp = result = malloc(strlen(orig) + (lenWith - lenRep) * count + 1);

  if (!result)
    return NULL;

  while (count--) {
    ins = strstr(orig, rep);
    lenFront = ins - orig;
    tmp = strncpy(tmp, orig, lenFront) + lenFront;
    tmp = strcpy(tmp, with) + lenWith;
    orig += lenFront + lenRep;
  }
  strcpy(tmp, orig);
  return result;
}

char *preprocess(char *input) {
  input = strReplace(input, "?""?=", "#");
  input = strReplace(input, "?""?/", "\\");
  input = strReplace(input, "?""?'", "^");
  input = strReplace(input, "?""?(", "[");
  input = strReplace(input, "?""?)", "]");
  input = strReplace(input, "?""?!", "|");
  input = strReplace(input, "?""?<", "{");
  input = strReplace(input, "?""?>", "}");
  input = strReplace(input, "?""?-", "~");

  input = strReplace(input, "\\\n", "");

  return input;
}
