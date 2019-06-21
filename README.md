### Summary 

This a verifiable image secret sharing program.

## Steps to execute

1. Open terminal in the programs root folder.
2. Execute commmand: make all 
   (In case you already executed the program and want to recompile it use "make clean" and "make all")
3. Run the program with "./app"
   3.1. To see a list of possible commands use option '-h': "./app -h".
   3.2. A distribution example would be: "./app -d -s "path/to/secret.bmp" -m "path/to/watermark.bmp" -k 4 -n 8 -i "path/to/shares".
