#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdbool.h>


#define ALL_PERMISSION (S_IRUSR + S_IWUSR + S_IRGRP + S_IROTH)


ssize_t open_and_stat(char* file_read_name) {

    struct stat txt_inf = { };
    stat(file_read_name, &txt_inf);

    return txt_inf.st_size / sizeof(char);

}


ssize_t text_to_buffer(char* file_read_name, char** buffer_for_read) {

    ssize_t num_chars_in_file = open_and_stat(file_read_name);
    *buffer_for_read = (char*) calloc (num_chars_in_file + 2, sizeof(char));

    int file_decriptor = open(file_read_name, O_RDONLY, ALL_PERMISSION);

    num_chars_in_file = read(file_decriptor, *buffer_for_read, num_chars_in_file);
    (*buffer_for_read)[num_chars_in_file + 0] = '\n';
    (*buffer_for_read)[num_chars_in_file + 1] = '\0';

    close(file_decriptor);

    return num_chars_in_file;

}


ssize_t text_to_file(const char* file_write_name, char* buffer_for_write, long buffer_length) {

    if (buffer_length < 0) {
        return -1;
    }

    int file_decriptor = open(file_write_name, O_WRONLY | O_CREAT | O_TRUNC, ALL_PERMISSION);
    ssize_t num_wrote_chars = write(file_decriptor, buffer_for_write, buffer_length);

    close(file_decriptor);

    return num_wrote_chars;

}

