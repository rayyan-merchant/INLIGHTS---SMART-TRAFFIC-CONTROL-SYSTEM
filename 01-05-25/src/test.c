#include <stdio.h>
int main() {
    FILE* fp = fopen("data/traffic_dataset.csv", "r");
    if (!fp) {
        perror("Failed to open file");
        return 1;
    }
    char line[256];
    if (!fgets(line, sizeof(line), fp)) {
        printf("Failed to read line\n");
        fclose(fp);
        return 1;
    }
    printf("Read line: %s", line);
    fclose(fp);
    return 0;
}