
/*--------------------------------------------------------------------*/
/*--- Emugrind: cheap CPU emulation.                     eg_main.c ---*/
/*--------------------------------------------------------------------*/

/*
   Copyright (C) 2017 Sebastien Tricaud
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

#include "eg_syscall_names.h"

static ULong n_nop = 0;
static UInt full_debugs = 0;
static UInt file_line_debugs = 0;
static UInt fn_debugs = 0;
static UInt no_debugs = 0;

static void get_debug_info(Addr instr_addr, const HChar **dir,
                           const HChar **file, const HChar **fn, UInt* line)
{
   Bool found_file_line = VG_(get_filename_linenum)(
                             instr_addr, 
                             file, dir,
                             line
                          );
   Bool found_fn        = VG_(get_fnname)(instr_addr, fn);

   if (!found_file_line) {
      *file = "???";
      *line = 0;
   }
   if (!found_fn) {
      *fn = "???";
   }

   if (found_file_line) {
      if (found_fn) full_debugs++;
      else          file_line_debugs++;
   } else {
      if (found_fn) fn_debugs++;
      else          no_debugs++;
   }
}

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
  Addr cia;
  UInt size;
  const HChar *fn, *file, *dir;
  UInt    line;
  int syscall_id;
  //  return sbIn;
  
  i = 0;

  /* ppIRSB(sbIn); */
 /* for (; i < sbIn->stmts_used; i++) { */
 /*    st = sbIn->stmts[i]; */

  for (; i < sbIn->stmts_used; i++) {
    st = sbIn->stmts[i];

    switch(st->tag) {
    case Ist_Put:
      //      VG_(printf)("Put\n");
      if (st->Ist.Put.offset == 16) {
  	/* RAX */
  	const IRExpr* e;
  	const IRConst* con;
	//	VG_(printf)("Put in RAX\n");
  	e = st->Ist.Put.data;
  	if (e->tag == Iex_Const) {
  	  con = e->Iex.Const.con;
  	  if (con->tag == Ico_U64) {
  	    syscall_id = (int *)con->Ico.U64;
  	  }
  	}
  	// ppIRConst(e->Iex.Const.con);
  	//
      }
      //      s->Ist.Put.offset
      // Int offset
      // IRExpr* data
      //st->Ist.Put.data;

      break;
    }
  } // for (; i < sbIn->stmts_used; i++) {

  
  /* ppIRJumpKind(sbIn->jumpkind); */
  switch(sbIn->jumpkind) {
  case Ijk_Call:
    /* VG_(printf)("Call\n"); */
    break;
  case Ijk_Ret:
    /* VG_(printf)("Return\n"); */
    break;
  case Ijk_Sys_syscall:
    VG_(printf)("syscall %s\n", syscall_names[syscall_id]);
    //    VG_(printf)("Syscall: %s\n", syscall_names[syscall_id]);
    	    //  	    VG_(printf)("syscall name=%s\n", syscall_names[syscall_id]);

  /* if (sbIn->offsIP == 184) { */
  /*   ppIRExpr( sbIn->next ); */
  /* } */
    /* VG_(printf)("\n"); */
  break;
  case Ijk_Sys_sysenter:
    /* VG_(printf)("Sys enter\n"); */
    break;
  }

  /* st = IRStmt_NoOp();   */
  /* ret_irsb = emptyIRSB(); */
  /* addStmtToIRSB(ret_irsb, st); */
  /* return ret_irsb; */
  
  return sbIn;
}

static void eg_fini(Int exitcode)
{
  //  VG_(umsg)("NOP count: %'llu\n", n_nop);
}

static void eg_pre_clo_init(void)
{
   VG_(details_name)            ("Emugrind");
   VG_(details_version)         (NULL);
   VG_(details_description)     ("Instrumenting the emulation valgrind does from multiple architectures");
   VG_(details_copyright_author)(
      "Copyright (c) 2017 Sebastien Tricaud");
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
