#include "m0.h"

#define label(NAME) \
	static const void *const NAME ## _ = &&NAME; (void)NAME ## _; NAME: \
	__asm__ __volatile__ (".global m0_core_" #NAME "\nm0_core_" #NAME ":")

typedef union { m0_int s; m0_uint u; } ireg;
typedef union { char *p; } preg;
typedef union { m0_float f; } freg;

void m0_core(m0_vm *restrict vm)
{
	register const m0_value *restrict ip = vm->ip;
	register m0_value *restrict rp = vm->rp;

	register ireg ia, ib;
	register preg pa, pb;
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

	label(set_pa);
	pa.p = (++ip)->as_ptr;
	goto *(++ip)->as_cptr;

	label(set_pb);
	pb.p = (++ip)->as_ptr;
	goto *(++ip)->as_cptr;

	label(get_ia);
	ia.u = rp[(++ip)->as_uword].as_uint;
	goto *(++ip)->as_cptr;

	label(get_ib);
	ib.u = rp[(++ip)->as_uword].as_uint;
	goto *(++ip)->as_cptr;

	label(get_pa);
	pa.p = rp[(++ip)->as_uword].as_ptr;
	goto *(++ip)->as_cptr;

	label(get_pb);
	pb.p = rp[(++ip)->as_uword].as_ptr;
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

	label(load_i8a_ia);
	ia.s = (m0_int)*(int8_t *)(pa.p + (++ip)->as_word);
	goto *(++ip)->as_cptr;

	label(load_i8b_ib);
	ib.s = (m0_int)*(int8_t *)(pb.p + (++ip)->as_word);
	goto *(++ip)->as_cptr;

	label(load_u8a_ia);
	ia.u = (m0_uint)*(uint8_t *)(pa.p + (++ip)->as_word);
	goto *(++ip)->as_cptr;

	label(load_u8b_ib);
	ib.u = (m0_uint)*(uint8_t *)(pb.p + (++ip)->as_word);
	goto *(++ip)->as_cptr;

	label(store_i8a_pb);
	*(int8_t *)(pb.p + (++ip)->as_word) = (int8_t)ia.s;
	goto *(++ip)->as_cptr;

	label(store_i8b_pa);
	*(int8_t *)(pa.p + (++ip)->as_word) = (int8_t)ib.s;
	goto *(++ip)->as_cptr;

	label(store_u8a_pb);
	*(uint8_t *)(pb.p + (++ip)->as_word) = (uint8_t)ia.u;
	goto *(++ip)->as_cptr;

	label(store_u8b_pa);
	*(uint8_t *)(pa.p + (++ip)->as_word) = (uint8_t)ib.u;
	goto *(++ip)->as_cptr;

	label(load_i16a_ia);
	ia.s = (m0_int)*(int16_t *)(pa.p + (++ip)->as_word);
	goto *(++ip)->as_cptr;

	label(load_i16b_ib);
	ib.s = (m0_int)*(int16_t *)(pb.p + (++ip)->as_word);
	goto *(++ip)->as_cptr;

	label(load_u16a_ia);
	ia.u = (m0_uint)*(uint16_t *)(pa.p + (++ip)->as_word);
	goto *(++ip)->as_cptr;

	label(load_u16b_ib);
	ib.u = (m0_uint)*(uint16_t *)(pb.p + (++ip)->as_word);
	goto *(++ip)->as_cptr;

	label(store_i16a_pb);
	*(int16_t *)(pb.p + (++ip)->as_word) = (int16_t)ia.s;
	goto *(++ip)->as_cptr;

	label(store_i16b_pa);
	*(int16_t *)(pa.p + (++ip)->as_word) = (int16_t)ib.s;
	goto *(++ip)->as_cptr;

	label(store_u16a_pb);
	*(uint16_t *)(pb.p + (++ip)->as_word) = (uint16_t)ia.u;
	goto *(++ip)->as_cptr;

	label(store_u16b_pa);
	*(uint16_t *)(pa.p + (++ip)->as_word) = (uint16_t)ib.u;
	goto *(++ip)->as_cptr;

	label(load_i32a_ia);
	ia.s = (m0_int)*(int32_t *)(pa.p + (++ip)->as_word);
	goto *(++ip)->as_cptr;

	label(load_i32b_ib);
	ib.s = (m0_int)*(int32_t *)(pb.p + (++ip)->as_word);
	goto *(++ip)->as_cptr;

	label(load_u32a_ia);
	ia.u = (m0_uint)*(uint32_t *)(pa.p + (++ip)->as_word);
	goto *(++ip)->as_cptr;

	label(load_u32b_ib);
	ib.u = (m0_uint)*(uint32_t *)(pb.p + (++ip)->as_word);
	goto *(++ip)->as_cptr;

	label(store_i32a_pb);
	*(int32_t *)(pb.p + (++ip)->as_word) = (int32_t)ia.s;
	goto *(++ip)->as_cptr;

	label(store_i32b_pa);
	*(int32_t *)(pa.p + (++ip)->as_word) = (int32_t)ib.s;
	goto *(++ip)->as_cptr;

	label(store_u32a_pb);
	*(uint32_t *)(pb.p + (++ip)->as_word) = (uint32_t)ia.u;
	goto *(++ip)->as_cptr;

	label(store_u32b_pa);
	*(uint32_t *)(pa.p + (++ip)->as_word) = (uint32_t)ib.u;
	goto *(++ip)->as_cptr;

	label(load_i64a_ia);
	ia.s = (m0_int)*(int64_t *)(pa.p + (++ip)->as_word);
	goto *(++ip)->as_cptr;

	label(load_i64b_ib);
	ib.s = (m0_int)*(int64_t *)(pb.p + (++ip)->as_word);
	goto *(++ip)->as_cptr;

	label(load_u64a_ia);
	ia.u = (m0_uint)*(uint64_t *)(pa.p + (++ip)->as_word);
	goto *(++ip)->as_cptr;

	label(load_u64b_ib);
	ib.u = (m0_uint)*(uint64_t *)(pb.p + (++ip)->as_word);
	goto *(++ip)->as_cptr;

	label(store_i64a_pb);
	*(int64_t *)(pb.p + (++ip)->as_word) = (int64_t)ia.s;
	goto *(++ip)->as_cptr;

	label(store_i64b_pa);
	*(int64_t *)(pa.p + (++ip)->as_word) = (int64_t)ib.s;
	goto *(++ip)->as_cptr;

	label(store_u64a_pb);
	*(uint64_t *)(pb.p + (++ip)->as_word) = (uint64_t)ia.u;
	goto *(++ip)->as_cptr;

	label(store_u64b_pa);
	*(uint64_t *)(pa.p + (++ip)->as_word) = (uint64_t)ib.u;
	goto *(++ip)->as_cptr;

	label(offset_pa);
	pa.p += (m0_word)ia.s * (++ip)->as_word;
	goto *(++ip)->as_cptr;

	label(offset_pb);
	pb.p += (m0_word)ib.s * (++ip)->as_word;
	goto *(++ip)->as_cptr;

	label(yield);
	vm->ip = ip;
	vm->rp = rp;
}
