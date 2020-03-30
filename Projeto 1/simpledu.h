
/** @defgroup Simpledu
 * @{
 *
 * Main functions  
 */



struct ArgumentFlags{
    int all; // can be either -a or -all, displays also the information about files
    int bytes; // can be either -b or -bytes, shoes the real number of bytes of data(files) or alocated (directories)
    int blockSize; // can be either -B or --block-size=SIZE, degines the size(bytes) of the block to representation effects
    int countLinks; // can be either -l or --count-links, counts multiple times the same file
    int simbolicLinks; // can be either -L or --dereference, follows the simbolic links
    int noSubDir; // can be either -S or --separate-dirs, the displayed info does not include the size of subdirectories
    int maxDepth; // --max-depth=N, limits the displayed info to N (0,1,..) levels of directories depth
}; 
/**
* @brief Functions that attributes an integer number to the argument type
* @param arg - argument 
* @return Returns 0 if OK, -1 otherwise
*/

int idArguments(char *arg);
/**
* @brief Functions that parses the arguments given on the command line and fills the struct of argument flags
* @param argc - number of arguments
* @param argv - arguments
* @param args - function to be filled with the info given by the user
* @return 0 if OK, -1 otherwise
*/
int parseArguments(int argc, char *argv[],struct ArgumentFlags *args);
/**
* @brief Function that verifies if the path is valid and if it is a file or a directory
* @param path - path given by the user
* @return 0 if OK, -1 otherwise
*/
int verifyPath(char * path);
/**
* @brief Function that initializes the argument flags struct
* @param args - struct to be initialized
*/
void initArgumentFlags(struct ArgumentFlags *args);
/**
* @brief Function that prints the flags
* @param args - arguments
*/
void checkFlags(struct ArgumentFlags* args);