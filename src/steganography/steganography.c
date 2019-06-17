#include "steganography.h"

void distribute_shares(const matrix_t ** Sh, const char * dir, uint8_t k, uint8_t n) 
{
	DIR * folder;
    struct dirent * entry;

    folder = opendir(dir);
    if (folder == NULL)
        return;

    int entry_index = 0;
    while ((entry = readdir(folder)) != NULL) 
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..")) 
        {
            BITMAP * bitmap = read_bmp(entry->d_name, true);
            if (k == 4 && n == 8)
                distribute_bits(Sh[entry_index], bitmap, 1, 0x1);
            else if (k == 2 && n == 4)
                distribute_bits(Sh[entry_index], bitmap, 2, 0x3);
            else 
            {
                free_bmp(bitmap);
                closedir(folder);
                return;
            }
            bitmap->file_header.bfReserved1 = entry_index;
            write_bmp(entry->d_name, bitmap);
            free_bmp(bitmap);

            entry_index++;
        }
    }

    closedir(folder);
    return;
}

void distribute_bits(const matrix_t * Sh, BITMAP * bitmap, uint8_t bits, uint8_t mask) 
{
    int bitmap_i = 0, bitmap_j = 0;

    int i, j, bit;
    for (i = 0; i < Sh->rows; i++) 
    {
        for (j = 0; j < Sh->columns; j++) 
        {
            for (bit = 0; bit < 8; bit += bits)
            {
                uint8_t bit_value = (Sh->data[i][j] >> bit) & mask;
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
    
}