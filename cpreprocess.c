#include "cpreprocess.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

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

int countChars(char *s, char c) {
  return *s == '\0' ? 0 : countChars(s + 1, c) + (*s == c);
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

  int numOfLines = countChars(input, '\n') + 2;
  char **lines = calloc(numOfLines, sizeof(char *));
  size_t inputLen = strlen(input);
  char *line = strtok(input, "\n");
  int linesIndex = 0;
  while (line != NULL) {
    lines[linesIndex++] = line;
    line = strtok(NULL, "\n");
  }

  linesIndex = 0;
  while (true) {
    if (!lines[linesIndex])
      break;
    char *line = lines[linesIndex++];
    size_t lineLen = strlen(line);
    for (int i = 0; i < lineLen; i++) {
      if (line[i] == '/' && line[i + 1] && line[i + 1] == '/') {
        line[i] = ' ';
        for (int j = i + 1; j < lineLen; j++)
          line[j] = '\0';
      }
    }
  }

  linesIndex = 0;
  while (true) {
    if (!lines[linesIndex])
      break;
    char *line = lines[linesIndex++];
    size_t lineLen = strlen(line);
    for (int i = 0; i < lineLen; i++) {
      if (line[i] == '/' && line[i + 1] && line[i + 1] == '*') {
        line[i] = ' ';
        for (int j = i + 1; j < lineLen; j++) {
          if (line[j] == '*' && line[j + 1] && line[j + 1] == '/') {
            if (line[j + 2])
              strncpy(line, line + j + 2, lineLen - j - 2);
            break;
          } else {
            line[j] = '\0';
            if (j == lineLen - 1) {
              line = lines[linesIndex++];
              lineLen = strlen(line);
              j = -1;
            }
          }
        }
      }
    }
  }

  for (int i = 0; i < numOfLines; i++) {
    if (lines[i]) {
      for (int j = 0; j < strlen(lines[i]); j++) {
        if (!isspace(lines[i][j]) && lines[i][j] != '#') break;
        if (lines[i][j] == '#' && strncmp(lines[i] + j, "#include", strlen("#include")) == 0) {
          // TODO
        };
      }
    }
  }

  char *result = calloc(inputLen + 1, sizeof(char));
  linesIndex = 0;
  while (true) {
    if (!lines[linesIndex])
      break;
    if (!linesIndex)
      snprintf(result, inputLen + 1, "%s\n", lines[linesIndex++]);
    else if (lines[linesIndex][0] != '\0')
      snprintf(result + strlen(result), inputLen, "%s\n", lines[linesIndex++]);
    else
      linesIndex++;
  }

  return result;
}
