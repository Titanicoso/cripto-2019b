#include <stdio.h>

void printWelcome()
{
    printf("Welcome to the security program\n");
    printf("Made by Agustín Calatayud, Diego Bruno Cilla and Lóránt Mikolás\n");
    printf("Execute with option -h for help\n");
    printf("Reading options and starting...\n\n");
}

void printHelp()
{
    printf("-d: indica que se va a distribuir una imagen secreta en otras imágenes.\n");
    printf("-r: indica que se va a recuperar una imagen secreta a partir de otras imágenes.\n");
    printf("-s imagen: El nombre imagen corresponde al nombre de un archivo de extensión .bmp. En el\n");
    printf("caso de que se haya elegido la opción (-d) este archivo debe existir ya que es la imagen a\n");
    printf("ocultar y debe ser una imagen en blanco y negro (8 bits por pixel) Si se eligió la opción (-r)\n");
    printf("éste archivo será el archivo de salida, con la imagen secreta revelada al finalizar el programa.\n");
    printf("-m imagen: El nombre imagen corresponde al nombre de un archivo con extensión .bmp. En el\n");
    printf("caso de que se haya elegido la opción (-d) este archivo es una imagen en blanco y negro que\n");
    printf("servirá como marca de agua para verificar todo el proceso. Debe ser de igual tamaño que la\n");
    printf("imagen secreta. En el caso de que se haya elegido la opción (-r) este archivo es una imagen en\n");
    printf("blanco y negro que contiene la transformación de la imagen de marca de agua.\n");
    printf("-k número: El número corresponde a la cantidad mínima de sombras necesarias para\n");
    printf("recuperar el secreto en un esquema (k, n).\n");
    printf("-n número: El número corresponde a la cantidad total de sombras en las que se distribuirá\n");
    printf("el secreto en un esquema (k, n).\n");
    printf("-i directorio El directorio donde se encuentran las imágenes en las que se distribuirá el\n");
    printf("secreto (en el caso de que se haya elegido la opción (-d)), o donde están las imágenes que\n");
    printf("contienen oculto el secreto ( en el caso de que se haya elegido la opción (-r)). Debe contener\n");
    printf("imágenes de extensión .bmp, de 24 bits por pixel.\n");
}

void printError(char * error)
{
    printf("%s", error);
}