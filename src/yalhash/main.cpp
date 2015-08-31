
#include <yal/yal.h>
#include <cstdio>
#include <yalvm/yalvm_hashing.h>

int main(const int argc,
         const char** argv)
{

    if (argc < 2)
    {
        printf("yalhash - 0.1.0\n");
        printf("Print variable name hashes.\n\n");
        printf("Usage: %s <name 0> <name 1> ... <name n>\n\n", argv[0]);
        return EXIT_FAILURE;
    }


    for (int i = 1; i < argc; ++i)
    {
        printf("%08x : %s\n", yalvm_one_at_a_time_hash(argv[i]), argv[i]);
    }
    return EXIT_SUCCESS;
}
