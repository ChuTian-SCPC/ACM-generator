#include "../../testlib.h"
int main(int argc, char* argv[]) {
    registerValidation(argc, argv);

    inf.readInt(1, 1000000);
    inf.readSpace();
    inf.readInt(1, 1000000);
    inf.readEoln();

    inf.readEof();
    return 0;
}