#include "prefix/prefix.h"

#include "data-mov/mov.h"
#include "data-mov/movext.h"
#include "data-mov/xchg.h"
#include "data-mov/push.h"
#include "data-mov/pusha.h"
#include "data-mov/pop.h"
#include "data-mov/cltd.h"
#include "data-mov/leave.h"
#include "data-mov/lgdt.h"
#include "data-mov/lidt.h"
#include "data-mov/std.h"
#include "data-mov/cld.h"

#include "con-tran/call.h"
#include "con-tran/ret.h"
#include "con-tran/ja.h"
#include "con-tran/jna.h"
#include "con-tran/jae.h"
#include "con-tran/jb.h"
#include "con-tran/jbe.h"
#include "con-tran/je.h"
#include "con-tran/jne.h"
#include "con-tran/jg.h"
#include "con-tran/jge.h"
#include "con-tran/jl.h"
#include "con-tran/jle.h"
#include "con-tran/jmp.h"
#include "con-tran/js.h"
#include "con-tran/jns.h"

#include "arith/dec.h"
#include "arith/inc.h"
#include "arith/neg.h"
#include "arith/imul.h"
#include "arith/mul.h"
#include "arith/idiv.h"
#include "arith/div.h"
#include "arith/sub.h"
#include "arith/sbb.h"
#include "arith/cmp.h"
#include "arith/add.h"
#include "arith/adc.h"

#include "logic/and.h"
#include "logic/or.h"
#include "logic/not.h"
#include "logic/xor.h"
#include "logic/sar.h"
#include "logic/shl.h"
#include "logic/shr.h"
#include "logic/shrd.h"
#include "logic/test.h"
#include "logic/setne.h"
#include "logic/seta.h"
#include "logic/setc.h"
#include "logic/sete.h"
#include "logic/setna.h"
#include "logic/setnc.h"

#include "string/rep.h"
#include "string/stos.h"
#include "string/lods.h"
#include "string/cmps.h"
#include "string/movs.h"
#include "string/scas.h"

#include "system/int.h"

#include "misc/misc.h"

#include "special/special.h"
