//���ļ������ 

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
        // �����ȡ����ÿһ������
        printf("%s", line);
    }

    fclose(file);

    return 0;
}

