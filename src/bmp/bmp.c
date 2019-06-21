#include "bmp.h"

BITMAP * read_bmp(const char * filename, bool color_bmp, bool enforce_mod)
{
    FILE * file_ptr;
    BITMAP * bitmap;
    int pixel_size = color_bmp ? 3 : 1;

    //open filename in read binary mode
    file_ptr = fopen(filename, "rb");
    if (file_ptr == NULL)
        return NULL;

    bitmap = calloc(1, sizeof(BITMAP));
    if (bitmap == NULL)
        return NULL;

    //read the bitmap file header
    fread(&bitmap->file_header, sizeof(BITMAPFILEHEADER), 1, file_ptr);

    //verify that this is a bmp file by check bitmap id
    if (bitmap->file_header.bfType != 0x4D42)
    {
        fclose(file_ptr);
        free(bitmap);
        return NULL;
    }

    //read the bitmap info header
    fread(&bitmap->info_header, sizeof(BITMAPINFOHEADER), 1, file_ptr);

    //read metadata between header and matrix
    if (bitmap->file_header.bfOffBits > sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)) 
    {
        bitmap->metadata_length = bitmap->file_header.bfOffBits - (sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
        bitmap->metadata = malloc(bitmap->metadata_length);
        fread(bitmap->metadata, bitmap->metadata_length, 1, file_ptr);
    }

    bitmap->matrix = create(bitmap->info_header.biHeight, bitmap->info_header.biWidth * pixel_size);

    //move file point to the beginning of bitmap data
    fseek(file_ptr, bitmap->file_header.bfOffBits, SEEK_SET);

    //read in the bitmap image data
    size_t i, j;
    for (i = 0; i < bitmap->matrix->rows; i++) 
    {
        for (j = 0; j < bitmap->matrix->columns; j++) 
        {
            fread(&bitmap->matrix->data[i][j], 1, 1, file_ptr);
            if (enforce_mod && bitmap->matrix->data[i][j] > 250)
                bitmap->matrix->data[i][j] = 250;
        }
    }

    //close file and return bitmap image data
    fclose(file_ptr);
    return bitmap;
}

void write_bmp(const char * filename, BITMAP * bitmap) {
    FILE * file_ptr;

    if (bitmap == NULL)
        return;

    //open filename in write binary mode
    file_ptr = fopen(filename, "wb");
    if (file_ptr == NULL)
        return;

    fwrite(&bitmap->file_header, sizeof(BITMAPFILEHEADER), 1, file_ptr);
    fwrite(&bitmap->info_header, sizeof(BITMAPINFOHEADER), 1, file_ptr);
    if (bitmap->metadata_length != 0) 
    {
        fwrite(bitmap->metadata, bitmap->metadata_length, 1, file_ptr);
    }

    size_t i, j;
    for (i = 0; i < bitmap->matrix->rows; i++) 
    {
        for (j = 0; j < bitmap->matrix->columns; j++) 
        {
            fwrite(&bitmap->matrix->data[i][j], 1, 1, file_ptr);
        }
    }

    //close file and return
    fclose(file_ptr);
    return;
}

void free_bmp(BITMAP * bitmap) {
    delete(bitmap->matrix);
    free(bitmap->metadata);
    free(bitmap);
}