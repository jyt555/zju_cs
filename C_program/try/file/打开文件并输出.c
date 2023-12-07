//打开文件并输出 

#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE* file;
    char line[256];

    file = fopen("filename.txt", "r");

    if (file == NULL) {
        printf("Failed to open file.\n");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), file)) {
        // 处理读取到的每一行数据
        printf("%s", line);
    }

    fclose(file);

    return 0;
}

