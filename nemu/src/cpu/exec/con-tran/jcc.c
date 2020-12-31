#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "jcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jcc-template.h"
#undef DATA_BYTE

make_helper_v(jcc_e)
make_helper_v(jcc_a)
make_helper_v(jcc_ae)
make_helper_v(jcc_b)
make_helper_v(jcc_be)
make_helper_v(jcc_g)
make_helper_v(jcc_ge)
make_helper_v(jcc_l)
make_helper_v(jcc_le)
make_helper_v(jcc_ne)
make_helper_v(jcc_no)
make_helper_v(jcc_np)
make_helper_v(jcc_ns)
make_helper_v(jcc_o)
make_helper_v(jcc_p)
make_helper_v(jcc_s)
