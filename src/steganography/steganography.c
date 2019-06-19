#include "steganography.h"

void distribute_shares(matrix_t ** Sh, const char * dir, uint8_t k, uint8_t n)
{
	DIR * folder;
    struct dirent * entry;

    if (!(k == 4 && n == 8) && !(k == 2 && n == 4))
        return;

    folder = opendir(dir);
    if (folder == NULL)
        return;

    int entry_index = 0;
    while ((entry = readdir(folder)) != NULL) 
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) 
        {
            char * path = malloc(strlen(dir) + 1 + strlen(entry->d_name) + 1);
            sprintf(path, "%s/%s", dir, entry->d_name);
            BITMAP * bitmap = read_bmp(path, true);
            if (bitmap == NULL) {
                free(path);
                free_bmp(bitmap);
                continue;
            }

            if (k == 4 && n == 8)
                distribute_bits(Sh[entry_index], bitmap, 1, 0x1);
            else if (k == 2 && n == 4)
                distribute_bits(Sh[entry_index], bitmap, 2, 0x3);

            bitmap->file_header.bfReserved1 = entry_index + 1;
            write_bmp(path, bitmap);
            free(path);
            free_bmp(bitmap);

            entry_index++;
        }
    }

    closedir(folder);
    return;
}

void distribute_bits(matrix_t * Sh, BITMAP * bitmap, uint8_t bits, uint8_t mask) 
{
    size_t bitmap_i = 0, bitmap_j = 0;

    size_t i, j, bit;
    for (i = 0; i < Sh->rows; i++) 
    {
        for (j = 0; j < Sh->columns; j++) 
        {
            for (bit = 0; bit < 8; bit += bits)
            {
                uint8_t bit_value = (Sh->data[i][j] >> (8 - bits - bit)) & mask;
                bitmap->matrix->data[bitmap_i][bitmap_j] = (bitmap->matrix->data[bitmap_i][bitmap_j] & ~mask) | bit_value;

                bitmap_j++;
                if (bitmap_j == bitmap->matrix->columns)
                {
                    bitmap_i++;
                    bitmap_j = 0;
                }
            }
        }
    }

    return;
}

matrix_t ** recover_shares(const char * dir, uint8_t k, uint8_t n)
{
    DIR * folder;
    struct dirent * entry;

    if (!(k == 4 && n == 8) && !(k == 2 && n == 4))
        return NULL;

    folder = opendir(dir);
    if (folder == NULL)
        return NULL;

    matrix_t ** shares = calloc(n, sizeof(matrix_t *));

    while ((entry = readdir(folder)) != NULL) 
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) 
        {
            char * path = malloc(strlen(dir) + 1 + strlen(entry->d_name) + 1);
            sprintf(path, "%s/%s", dir, entry->d_name);
            BITMAP * bitmap = read_bmp(path, true);
            if (bitmap == NULL) {
                free(path);
                free_bmp(bitmap);
                continue;
            }

            matrix_t * Sh = create(1, bitmap->info_header.biWidth * bitmap->info_header.biHeight * 3 / n);
            if (k == 4 && n == 8)
                recover_bits(Sh, bitmap, 1, 0x1);
            else if (k == 2 && n == 4)
                recover_bits(Sh, bitmap, 2, 0x3);

            shares[bitmap->file_header.bfReserved1 - 1] = Sh;
            free(path);
            free_bmp(bitmap);
        }
    }

    closedir(folder);
    return shares;
}

void recover_bits(matrix_t * Sh, BITMAP * bitmap, uint8_t bits, uint8_t mask) 
{
    size_t bitmap_i = 0, bitmap_j = 0;

    size_t i, j, bit;
    for (i = 0; i < Sh->rows; i++) 
    {
        for (j = 0; j < Sh->columns; j++) 
        {
            uint8_t byte_value = 0;
            for (bit = 0; bit < 8; bit += bits)
            {
                uint8_t bit_value = bitmap->matrix->data[bitmap_i][bitmap_j] & mask;
                byte_value = (byte_value << bits) | bit_value;

                bitmap_j++;
                if (bitmap_j == bitmap->matrix->columns)
                {
                    bitmap_i++;
                    bitmap_j = 0;
                }
            }
            Sh->data[i][j] = byte_value;
        }
    }

    return;
}