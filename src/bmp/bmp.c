#include "bmp.h"

matrix_t * read_bmp(const char * filename, bool color_bmp)
{
    FILE * file_ptr;
    BITMAPFILEHEADER bitmap_file_header;
    BITMAPINFOHEADER bitmap_info_header;
    matrix_t * matrix;
    int pixel_size;
    if (color_bmp) {
        pixel_size = 3;
    } else {
        pixel_size = 1;
    }

    //open filename in read binary mode
    file_ptr = fopen(filename,"rb");
    if (file_ptr == NULL)
        return NULL;

    //read the bitmap file header
    fread(&bitmap_file_header, sizeof(BITMAPFILEHEADER), 1, file_ptr);

    //verify that this is a bmp file by check bitmap id
    if (bitmap_file_header.bfType != 0x4D42)
    {
        fclose(file_ptr);
        return NULL;
    }

    //read the bitmap info header
    fread(&bitmap_info_header, sizeof(BITMAPINFOHEADER), 1 , file_ptr);

    matrix = create(bitmap_info_header.biHeight, bitmap_info_header.biWidth * pixel_size);

    //move file point to the beginning of bitmap data
    fseek(file_ptr, bitmap_file_header.bfOffBits, SEEK_SET);

    //read in the bitmap image data
    int i, j;
    for (i = 0; i < matrix->rows; i++) {
        for (j = 0; j < matrix->columns; j++) {
            fread(&(matrix->data[i][j]), 1, 1, file_ptr);
            if (matrix->data[i][j] > 250)
                matrix->data[i][j] = 250;
        }
    }

    //close file and return bitmap image data
    fclose(file_ptr);
    return matrix;
}