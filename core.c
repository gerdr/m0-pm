#include "m0.h"

#define label(NAME) \
	static const void *const NAME ## _ = &&NAME; NAME: \
	__asm__ __volatile__ (".global m0_core_" #NAME "\nm0_core_" #NAME ":")

typedef union { m0_int s; m0_uint u; } ireg;
typedef union { char *p; m0_word s; m0_uword u; } wreg;
typedef union { m0_float f; } freg;

void m0_core(m0_vm *restrict vm)
{
	register const m0_value *restrict ip = vm->ip;
	register m0_value *restrict rp = vm->rp;

	register ireg ia, ib;
	register wreg wa, wb;
	register freg fa, fb;

	goto *ip->as_cptr;

	label(put_ia);
	rp[(++ip)->as_uword].as_uint = ia.u;
	goto *(++ip)->as_cptr;

	label(put_ib);
	rp[(++ip)->as_uword].as_uint = ib.u;
	goto *(++ip)->as_cptr;

	label(set_ia);
	ia.u = (++ip)->as_uint;
	goto *(++ip)->as_cptr;

	label(set_ib);
	ib.u = (++ip)->as_uint;
	goto *(++ip)->as_cptr;

	label(get_ia);
	ia.u = rp[(++ip)->as_uword].as_uint;
	goto *(++ip)->as_cptr;

	label(get_ib);
	ib.u = rp[(++ip)->as_uword].as_uint;
	goto *(++ip)->as_cptr;

	label(add_ia);
	ia.u += ib.u;
	goto *(++ip)->as_cptr;

	label(mul_ia);
	ia.u *= ib.u;
	goto *(++ip)->as_cptr;

	label(add_fa);
	fa.f += fb.f;
	goto *(++ip)->as_cptr;

	label(load_u32_ia);
	ia.u = (m0_uint)*(uint32_t *)(wa.p + (++ip)->as_word);
	goto *(++ip)->as_cptr;

	label(load_u32_ib);
	ib.u = (m0_uint)*(uint32_t *)(wb.p + (++ip)->as_word);
	goto *(++ip)->as_cptr;

	label(offset_wa);
	wa.p += (m0_word)ia.s * (++ip)->as_word;
	goto *(++ip)->as_cptr;

	label(offset_wb);
	wb.p += (m0_word)ib.s * (++ip)->as_word;
	goto *(++ip)->as_cptr;

	label(yield);
	vm->ip = ip;
	vm->rp = rp;
}
