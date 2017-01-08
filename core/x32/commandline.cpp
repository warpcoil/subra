#include "commandline.h"
#include "kernel.h"

//Globals in the scope of the kernel
int numArguments;
char** arguments;
char** keys;
char** values;

int CommandLine::getSplitPoint(char *argv) {

    //Get the position of the =
    //Replace = with \0
    //Return the point at which the string splits

    int len = strlen(argv);

    for (int i=0; i<len; i++) {
        if (argv[i] == '=') {
            argv[i] = '\0';
            return i;
        }
    }

    return -1;

}

void CommandLine::RegisterCommandLine(int argc, char *argv[]) {
    numArguments = argc;
    arguments = argv;

    //Count length of keys
    //Count length of values

    keys = (char **)dlmalloc((sizeof (char *)) * numArguments);
    values = (char **)dlmalloc((sizeof (char *)) * numArguments);

    for (int i=0; i<numArguments; i++) {

        //Lower case early [faster lookups]
        argv[i] = strlwr(argv[i]);

        int splitPoint = getSplitPoint(argv[i]);

        if (splitPoint > -1) {

            keys[i] = &(argv[i][0]);
            values[i] = (&argv[i][splitPoint + 1]);

        }
    }
}

int CommandLine::HasArgument(char *arg) {

    //Not boolean, return an index or -1
    //It's logical that if you check for an argument, in short course you may want to
    //get the actual value of the argument 1 or more times

    arg = strlwr(arg);

    //CaSe InSensiTivE
    for (int i=0; i<numArguments; i++) {

        if (strcmp(keys[i], arg) == 0) {
            return i;
        }

    }

    return -1;
}

char * CommandLine::GetValue(uint16_t index) {

    if (index > numArguments) {
        return 0;
    }

    return values[index];

}
