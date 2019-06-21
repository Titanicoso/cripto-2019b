# Summary 

This a verifiable image secret sharing program.

## Steps to execute

1. Open terminal in the programs root folder.
2. Execute commmand: make all 
   (In case you already executed the program and want to recompile it use "make clean" and "make all")
3. Run the program with `./app`:
 * To see a list of possible commands use option `-h`: `./app -h`.

 * A distribution example would be: `./app -d -s "path/to/secret.bmp" -m "path/to/watermark.bmp" -k 4 -n 8 -i "path/to/shares"`.

 * The options to run are:
   -d: sets distribution mode.
   -r: sets recovery mode.
   -s [PATH_TO_IMAGE]: the path of the image of extension .bmp.
                       In case of distribution the file must exist
                       and it must be black and white (8 bits/pixel)
                       Otherwise the path will be used as a output file to save the secret.
   -m [PATH_TO_IMAGE]: the path of the image of extension .bmp
                      In case of distribution the file must exist
                      it must be black and white (8 bits/pixel)
                      and it will serve as a watermark verification
                      Otherwise the image will contain de transformation of the watermark.
   -k [NUMBER]: the minimum quantity of necessary shadows to recover the secret.
   -n [NUMBER]: total ammount of shadows that will be distributed.
   -i [PATH_TO_DIRECTORY]: in case of distribution the directory where the images that
                           will be use to hide the secret are.
                           Otherwise is the path to directory where the images with the secret are.
