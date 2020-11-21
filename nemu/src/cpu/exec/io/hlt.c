#include "cpu/exec/helper.h"


make_helper(hlt){
    if (!cpu.INTR){
        cpu.eip--;
    }
    return -1;
}