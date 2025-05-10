# 6-6 & 6-5 Elementary Preprocessor

## Installation
To install simply run make. This will create a program file called 6-6. 

## How to run
To run the program simply run ./6-6. This will now wait record everything entered on stdin and will continue until it reaches EOF (triggered by CTRL+D). If you would like to enter from a file simply redirect the file into it by running ./6-6 <INFILE where INFILE is the name of your program. Once EOF is reached the program prints what the program code would look after the preprocessor had run. This only works with simple #define statements (i.e. no functional macros just text identifier and token replacement) as well as #undef statements. If you would like to save the output simply run ./6-6 >OUTFILE where OUTFILE is the file you would like to save to. To have file inputs and outputs run ./6-6 <INFIlE >OUTFILE.
