#include "header.h"

int main(void) {
    int option;
    char tmps[400];
    while (1) {
        system("clear");
        // system("cls");
        printf(" +----------------------------+\n");
        printf(" |    Choose A Function       |\n");
        printf(" |    1. Lexical Analysis     |\n");
        printf(" |    2. Syntax Analysis      |\n");
        printf(" |    3. Save Formatted File  |\n");
        printf(" |    0. Exit                 |\n");
        printf(" +----------------------------+\n");
        scanf("%d", &option);
        switch (option) {
            case 1:
                printf("请输入源文件路径:\n");
                scanf("%s", fileIn);
                fr = fopen(fileIn, "r");
                if (fr == NULL) {
                    printf("ERR\n");
                }
                lexicalAnalyse();
                fclose(fr);
                printf("Press any key to continue\n");
                getchar();
                getchar();
                break;
            case 2:
                printf("请输入源文件路径:\n");
                scanf("%s", fileIn);
                fr = fopen(fileIn, "r");
                syntaxAnalyse();
                printf("Press any key to continue\n");
                fclose(fr);
                getchar();
                getchar();
                break;
            case 3:
                printf("请输入源文件路径:\n");
                scanf("%s", fileIn);
                fr = fopen(fileIn, "r");
                printf("请输入目标文件路径:\n");
                scanf("%s", fileOut);
                fw = fopen(fileOut, "w");
                formatCode();
                printf("Press any key to continue\n");
                fclose(fr);
                fclose(fw);
                sprintf(tmps, "code -d %s %s", fileIn, fileOut);
                system(tmps);
                getchar();
                getchar();
                break;
            case 0:
                system("clear");
                // system("cls");
                printf("Thanks for using!\n");
                return 0;
            default:
                printf("Unknown key, please enter again\n");
                printf("Press any key to continue\n");
                getchar();
                getchar();
                break;
        }
    }
}
