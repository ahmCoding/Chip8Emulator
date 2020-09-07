
#include "include/chip8.h"

int main()
{
    map<Uchar, bool> tst;
    typedef std::pair<Uchar, bool> p;
    tst.insert(p('1', true));
    bool eing = false;
    while (!eing)
    {
        Uchar g;
        cin >> g;
        eing = tst(g);
    }

    //Chip8 tst;
    //tst.initialize();
    //tst.loadGame();
}
