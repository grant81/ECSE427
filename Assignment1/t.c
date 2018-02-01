#include <stdio.h>
#include <string.h>   //for string manipulation

int wordCount(char *filename, char *flag)
{
    int cnt = 0;
    FILE *fp;
    fp = fopen(filename, "r");
    char c;
    if(fp == NULL){
        printf ("file does not exist");
        return 0;
    }
    if (!strcmp("-l", flag))
    {
        while ((c = fgetc(fp)) != EOF)
        {
            if (c == '\n')
            {
                cnt = cnt + 1;
            }
        }
    }
    else if (!strcmp("-w",flag))
    {
        while ((c = fgetc(fp)) != EOF)
        {
            if (c == '\n' || c == ' ')
            {
                while ((c = fgetc(fp)) == ' ')
                {
                    cnt = cnt;
                }
                cnt = cnt + 1;
            }
        }
    }
    else
    {
        printf("wrong wc flag\n");
    }

    fclose(fp);
    return cnt;
}
int main(void)
{
//   FILE *fp;
//   int   lines = 0;

//     char c;
//   fp = fopen("c.txt", "r");
//         while ((c = fgetc(fp)) != EOF)
//         {
//             if (c == '\n' || c == ' ')
//             {
//                 while ((c = fgetc(fp)) == ' ')
//                 {
//                     lines = lines;
//                 }
//                 lines = lines + 1;
//             }
//         }
// //      while (fgets(buffer, sizeof buffer, fp)) {
// //     ++lines;
// //   }
//   fclose(fp);
//   printf("word are %d\n", lines);
int lines = wordCount("c.txt","-w");
printf("word are %d\n", lines);
 return 0;
}
