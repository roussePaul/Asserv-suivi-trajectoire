#include "webplot.h"

int main(int argcc, char* argv[])
{
    int i=100;

    while(i--)
        plot("test", "%i %i", i, i*i);


    return 0;
}
