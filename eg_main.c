
/*--------------------------------------------------------------------*/
/*--- Emugrind: instrument emulation.                    eg_main.c ---*/
/*--------------------------------------------------------------------*/

/*
   This file is part of Nulgrind, the minimal Valgrind tool,
   which does no instrumentation or analysis.

   Copyright (C) 2015 Sebastien Tricaud
      sebastien@honeynet.org

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307, USA.

   The GNU General Public License is contained in the file COPYING.
*/

#include "pub_tool_basics.h"
#include "pub_tool_tooliface.h"


static ULong n_nop = 0;

static void eg_post_clo_init(void)
{
}

static
IRSB* eg_instrument ( VgCallbackClosure* closure,
                      IRSB* sbIn,
                      VexGuestLayout* layout, 
                      VexGuestExtents* vge,
                      VexArchInfo* archinfo_host,
                      IRType gWordTy, IRType hWordTy )
{
  Int i;
  IRSB *ret_irsb;
  IRStmt *st;

  i = 0;

  //  ppIRSB(sbIn);
  ret_irsb = deepCopyIRSB(sbIn);

  VG_(printf)("stmts_used:%d\n", sbIn->stmts_used);
  for (; i < sbIn->stmts_used; i++) {
    st = sbIn->stmts[i];

    switch(st->tag) {
    case Ist_NoOp:
      n_nop++;
      VG_(printf)("NoOp\n");
      break;
    case Ist_IMark:
    case Ist_AbiHint:
      //      VG_(printf)("(optional) extra info about the code\n");
      break;
    case Ist_Put:
      //      VG_(printf)("Put\n");
      // Int offset
      // IRExpr* data
      //st->Ist.Put.data;

      break;
    case Ist_PutI:
      VG_(printf)("PutI\n");
      break;
    case Ist_WrTmp:
      VG_(printf)("WrTmp\n");
      break;
    case Ist_Store:
      VG_(printf)("Store\n");
      break;
    case Ist_LoadG:
      VG_(printf)("LoadG\n");
      break;
    case Ist_StoreG:
      VG_(printf)("StoreG\n");
      break;
    case Ist_CAS:
      VG_(printf)("CAS\n");
      break;
    case Ist_LLSC:
      VG_(printf)("LLSC\n");
      break;
    case Ist_Dirty:
      VG_(printf)("Dirty\n");
      break;
    case Ist_MBE:
      VG_(printf)("MBE\n");
      break;
    case Ist_Exit:
      VG_(printf)("Exit\n");
      break;
    default:
      VG_(printf)("st->tag:%d\n", st->tag);
    }
  }

#if 0
  for (; i < ret_irsb->stmts_used; i++) {
    st = ret_irsb->stmts[i];
    st->tag = Ist_NoOp;
  }
  return ret_irsb;
#endif

  return sbIn;
}

static void eg_fini(Int exitcode)
{
  VG_(umsg)("NOP count: %'llu\n", n_nop);
}

static void eg_pre_clo_init(void)
{
   VG_(details_name)            ("Emugrind");
   VG_(details_version)         (NULL);
   VG_(details_description)     ("Instrumenting the emulation valgrind does from multiple architectures");
   VG_(details_copyright_author)(
      "Copyright (c) 2015 Sebastien Tricaud");
   VG_(details_bug_reports_to)  (VG_BUGS_TO);

   VG_(details_avg_translation_sizeB) ( 275 );

   VG_(basic_tool_funcs)        (eg_post_clo_init,
                                 eg_instrument,
                                 eg_fini);

   /* No needs, no core events to track */
}

VG_DETERMINE_INTERFACE_VERSION(eg_pre_clo_init)

/*--------------------------------------------------------------------*/
/*--- end                                                          ---*/
/*--------------------------------------------------------------------*/
