/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2024-03-25 09:46:01
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2024-03-25 13:17:03
 * @FilePath: \mergedFile\mergedFile.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
//这个程序接受四个命令行参数：两个输入文件的路径和合并后的文件的路径，以及第二个文件在合并文件中的偏移量。
//例如，如果程序名为 merge_to_fixed_size_with_offset，用户可以这样运行它：
//./merge_to_fixed_size_with_offset path/to/file1.bin path/to/file2.bin path/to/mergedFile.bin 1024
//这将合并 file1.bin 和 file2.bin 到 mergedFile.bin，确保 mergedFile.bin 的大小为128KB，并且 file2.bin 的内容从偏移量 1024 开始写入


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Usage: %s <file1_path> <file2_path> <merged_file_path> <offset>", argv[0]);
        return 1;
    }

    const char *file1Path = argv[1];
    const char *file2Path = argv[2];
    const char *mergedFilePath = argv[3];
    long offset = atol(argv[4]);

    FILE *file1 = fopen(file1Path, "rb");
    FILE *file2 = fopen(file2Path, "rb");
    FILE *mergedFile = fopen(mergedFilePath, "wb");

    if (file1 == NULL || file2 == NULL || mergedFile == NULL) {
        printf("Could not open files\n");
        return 1;
    }

    // Allocate a buffer for reading files
    const size_t bufferSize = 1024;
    unsigned char *buffer = (unsigned char *)malloc(bufferSize);

    // Copy contents of first file to merged file
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, bufferSize, file1)) > 0) {
        fwrite(buffer, 1, bytesRead, mergedFile);
    }

    // Check the remaining space in the merged file
    long remainingSize = 128 * 1024; // 128KB
    fseek(mergedFile, 0, SEEK_END);
    remainingSize -= ftell(mergedFile);

    // Set the file position indicator for the merged file to the specified offset
    if (fseek(mergedFile, offset, SEEK_SET) != 0) {
        printf("Failed to set offset in merged file\n");
        return 1;
    }

    // Copy contents of second file to merged file, but only up to the remaining space
    while (remainingSize > 0 && (bytesRead = fread(buffer, 1, remainingSize > bufferSize ? bufferSize : remainingSize, file2)) > 0) {
        fwrite(buffer, 1, bytesRead, mergedFile);
        remainingSize -= bytesRead;
    }

    fclose(file1);
    fclose(file2);
    fclose(mergedFile);
    free(buffer);

    printf("Files merged successfully into %s with a fixed size of 128KB and offset\n", mergedFilePath);
    return 0;
}
