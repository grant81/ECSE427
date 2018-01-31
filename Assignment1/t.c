#include <stdio.h>
 
int main(void)
{
  FILE *fp;
  int   lines = 0;
  char  buffer[BUFSIZ];
 
  fp = fopen("file.txt", "r");
if(fp)
 {
   printf("here");
     while (fgets(buffer, sizeof buffer, fp)) {
    ++lines;
  }
  fclose(fp);
  printf("lines are %d\n", lines);
 }
else
 {
     printf("%d", fp);
  printf("error opening the file");
 }

 
  return 0;
}