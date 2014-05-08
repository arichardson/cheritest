/*-
 * Copyright (c) 2012-2014 Robert N. M. Watson
 * Copyright (c) 2014 SRI International
 * All rights reserved.
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory under DARPA/AFRL contract (FA8750-10-C-0237)
 * ("CTSRD"), as part of the DARPA CRASH research programme.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>

#if !__has_feature(capabilities)
#error "This code requires a CHERI-aware compiler"
#endif

#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/time.h>

#include <machine/cheri.h>
#include <machine/cheric.h>
#include <machine/cpuregs.h>

#include <cheri/cheri_fd.h>
#include <cheri/sandbox.h>

#include <cheritest-helper.h>
#include <err.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>

#include "cheritest.h"
#include "cheritest_sandbox.h"

static void
usage(void)
{

	fprintf(stderr, "cheritest creturn\n");
	fprintf(stderr, "cheritest ccall_creturn\n");
	fprintf(stderr, "cheritest ccall_nop_creturn\n");
	fprintf(stderr, "cheritest copyregs\n");
	fprintf(stderr, "cheritest invoke_abort\n");
	fprintf(stderr, "cheritest invoke_clock_gettime\n");
	fprintf(stderr, "cheritest invoke_cp2_bound\n");
	fprintf(stderr, "cheritest invoke_cp2_perm\n");
	fprintf(stderr, "cheritest invoke_cp2_seal\n");
	fprintf(stderr, "cheritest invoke_cp2_tag\n");
	fprintf(stderr, "cheritest invoke_divzero\n");
	fprintf(stderr, "cheritest invoke_fd_fstat_c\n");
	fprintf(stderr, "cheritest invoke_fd_lseek_c\n");
	fprintf(stderr, "cheritest invoke_fd_read_c\n");
	fprintf(stderr, "cheritest invoke_fd_write_c\n");
	fprintf(stderr, "cheritest invoke_helloworld\n");
	fprintf(stderr, "cheritest invoke_md5\n");
	fprintf(stderr, "cheritest invoke_malloc\n");
	fprintf(stderr, "cheritest invoke_printf\n");
	fprintf(stderr, "cheritest invoke_putchar\n");
	fprintf(stderr, "cheritest invoke_puts\n");
	fprintf(stderr, "cheritest invoke_spin\n");
	fprintf(stderr, "cheritest invoke_syscall\n");
	fprintf(stderr, "cheritest invoke_syscap\n");
	fprintf(stderr, "cheritest invoke_vm_rfault\n");
	fprintf(stderr, "cheritest invoke_vm_wfault\n");
	fprintf(stderr, "cheritest invoke_vm_xfault\n");
	fprintf(stderr, "cheritest listregs\n");
	fprintf(stderr, "cheritest test_fault_cgetcause\n");
	fprintf(stderr, "cheritest test_fault_ccheck_user_fail\n");
	fprintf(stderr, "cheritest test_nofault_ccheck_user_pass\n");
	fprintf(stderr, "cheritest test_fault_overrun\n");
	fprintf(stderr, "cheritest test_fault_read_kr1c\n");
	fprintf(stderr, "cheritest test_fault_read_kr2c\n");
	fprintf(stderr, "cheritest test_fault_read_kcc\n");
	fprintf(stderr, "cheritest test_fault_read_kdc\n");
	fprintf(stderr, "cheritest test_fault_read_epcc\n");
	fprintf(stderr, "cheritest revoke_fd\n");
	fprintf(stderr, "cheritest sleep\n");
	exit(EX_USAGE);
}

int
main(__unused int argc, __unused char *argv[])
{
	int i, opt;

	while ((opt = getopt(argc, argv, "")) != -1) {
		switch (opt) {
		default:
			usage();
		}
	}
	argc -= optind;
	argv += optind;
	if (argc == 0)
		usage();

	cheritest_libcheri_setup();
	for (i = 0; i < argc; i++) {
		if (strcmp(argv[i], "listregs") == 0)
			cheritest_listregs();
		else if (strcmp(argv[i], "ccall_creturn") == 0)
			cheritest_ccall_creturn();
		else if (strcmp(argv[i], "ccall_nop_creturn") == 0)
			cheritest_ccall_nop_creturn();
		else if (strcmp(argv[i], "creturn") == 0)
			cheritest_creturn();
		else if (strcmp(argv[i], "copyregs") == 0)
			cheritest_copyregs();
		else if (strcmp(argv[i], "invoke_abort") == 0)
			cheritest_invoke_simple_op(CHERITEST_HELPER_OP_ABORT);
		else if (strcmp(argv[i], "invoke_clock_gettime") == 0)
			cheritest_invoke_simple_op(
			    CHERITEST_HELPER_OP_CS_CLOCK_GETTIME);
		else if (strcmp(argv[i], "invoke_cp2_bound") == 0)
			cheritest_invoke_simple_op(
			    CHERITEST_HELPER_OP_CP2_BOUND);
		else if (strcmp(argv[i], "invoke_cp2_perm") == 0)
			cheritest_invoke_simple_op(
			    CHERITEST_HELPER_OP_CP2_PERM);
		else if (strcmp(argv[i], "invoke_cp2_tag") == 0)
			cheritest_invoke_simple_op(
			    CHERITEST_HELPER_OP_CP2_TAG);
		else if (strcmp(argv[i], "invoke_cp2_seal") == 0)
			cheritest_invoke_simple_op(
			    CHERITEST_HELPER_OP_CP2_SEAL);
		else if (strcmp(argv[i], "invoke_divzero") == 0)
			cheritest_invoke_simple_op(
			    CHERITEST_HELPER_OP_DIVZERO);
		else if (strcmp(argv[i], "invoke_fd_fstat_c") == 0)
			cheritest_invoke_fd_op(
			     CHERITEST_HELPER_OP_FD_FSTAT_C);
		else if (strcmp(argv[i], "invoke_fd_lseek_c") == 0)
			cheritest_invoke_fd_op(
			     CHERITEST_HELPER_OP_FD_LSEEK_C);
		else if (strcmp(argv[i], "invoke_fd_read_c") == 0)
			cheritest_invoke_fd_op(
			     CHERITEST_HELPER_OP_FD_READ_C);
		else if (strcmp(argv[i], "invoke_fd_write_c") == 0)
			cheritest_invoke_fd_op(
			     CHERITEST_HELPER_OP_FD_WRITE_C);
		else if (strcmp(argv[i], "invoke_helloworld") == 0)
			cheritest_invoke_simple_op(
			    CHERITEST_HELPER_OP_CS_HELLOWORLD);
		else if (strcmp(argv[i], "invoke_md5") == 0)
			cheritest_invoke_md5();
		else if (strcmp(argv[i], "invoke_malloc") == 0)
			cheritest_invoke_simple_op(
			    CHERITEST_HELPER_OP_MALLOC);
		else if (strcmp(argv[i], "invoke_printf") == 0)
			cheritest_invoke_simple_op(
			    CHERITEST_HELPER_OP_PRINTF);
		else if (strcmp(argv[i], "invoke_putchar") == 0)
			cheritest_invoke_simple_op(
			    CHERITEST_HELPER_OP_CS_PUTCHAR);
		else if (strcmp(argv[i], "invoke_puts") == 0)
			cheritest_invoke_simple_op(
			    CHERITEST_HELPER_OP_CS_PUTS);
		else if (strcmp(argv[i], "invoke_spin") == 0)
			cheritest_invoke_simple_op(
			    CHERITEST_HELPER_OP_SPIN);
		else if (strcmp(argv[i], "invoke_syscall") == 0)
			cheritest_invoke_syscall();
		else if (strcmp(argv[i], "invoke_syscap") == 0)
			cheritest_invoke_simple_op(
			    CHERITEST_HELPER_OP_SYSCAP);
		else if (strcmp(argv[i], "invoke_vm_rfault") == 0)
			cheritest_invoke_simple_op(
			    CHERITEST_HELPER_OP_VM_RFAULT);
		else if (strcmp(argv[i], "invoke_vm_wfault") == 0)
			cheritest_invoke_simple_op(
			    CHERITEST_HELPER_OP_VM_WFAULT);
		else if (strcmp(argv[i], "invoke_vm_xfault") == 0)
			cheritest_invoke_simple_op(
			    CHERITEST_HELPER_OP_VM_XFAULT);
		else if (strcmp(argv[i], "revoke_fd") == 0)
			cheritest_revoke_fd();
		else if (strcmp(argv[i], "sleep") == 0)
			sleep(10);
		else if (strcmp(argv[i], "test_fault_cgetcause") == 0)
			test_fault_cgetcause();
		else if (strcmp(argv[i], "test_fault_overrun") == 0)
			test_fault_overrun();
		else if (strcmp(argv[i], "test_fault_ccheck_user_fail") == 0)
			test_fault_ccheck_user_fail();
		else if (strcmp(argv[i], "test_nofault_ccheck_user_pass") == 0)
			test_nofault_ccheck_user_pass();
		else if (strcmp(argv[i], "test_fault_read_kr1c") == 0)
			test_fault_read_kr1c();
		else if (strcmp(argv[i], "test_fault_read_kr2c") == 0)
			test_fault_read_kr2c();
		else if (strcmp(argv[i], "test_fault_read_kcc") == 0)
			test_fault_read_kcc();
		else if (strcmp(argv[i], "test_fault_read_kdc") == 0)
			test_fault_read_kdc();
		else if (strcmp(argv[i], "test_fault_read_epcc") == 0)
			test_fault_read_epcc();
		else if (strcmp(argv[i], "") == 0)
			test_fault_read_epcc();
		else
			usage();
	}
	cheritest_libcheri_destroy();
	exit(EX_OK);
}
