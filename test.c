#include "m0.h"

#undef NDEBUG
#include <assert.h>

int main(void)
{
	{
		m0_value code[] = {
			{ .as_cptr = m0_core_set_ia },
			{ .as_uint = 2 },
			{ .as_cptr = m0_core_set_ib },
			{ .as_uint = 40 },
			{ .as_cptr = m0_core_add_ia },
			{ .as_cptr = m0_core_put_ia },
			{ .as_uword = 0 },
			{ .as_cptr = m0_core_yield }
		};

		m0_value regs[1];

		m0_vm vm = { regs, code };
		m0_core(&vm);

		assert(regs[0].as_uint == 42);
	}

	{
		enum { A = -5, B = 1024, C = 13, REG = 0 };

		uint32_t out = 42;

		m0_constant constants[] = {
			{ M0_CONST_INT, { .as_int = A } },
			{ M0_CONST_INT, { .as_int = B } },
			{ M0_CONST_INT, { .as_int = C } },
			{ M0_CONST_ADDRESS, { .as_cptr = &(m0_address){
				.type = M0_MEM_U32,
				.base_symbol = &(m0_symbol){ .name = "out", .value = &out }
			} } },
		};

		m0_value code[42];
		size_t count = 0;
		m0_op op;

		m0_value *cursor = code;

		op = m0_op_rii(ADD, REG, 0, 1);
		assert(m0_disassemble(op, constants, stdout));
		count = m0_compile(op, constants, cursor);
		assert(count == 7);
		assert(cursor[0].as_cptr == m0_core_set_ia);
		assert(cursor[1].as_int == A);
		assert(cursor[2].as_cptr == m0_core_set_ib);
		assert(cursor[3].as_int == B);
		assert(cursor[4].as_cptr == m0_core_add_ia);
		assert(cursor[5].as_cptr == m0_core_put_ia);
		assert(cursor[6].as_uword == REG);

		cursor += count;

		op = m0_op_mri(MUL, 3, REG, 2);
		assert(m0_disassemble(op, constants, stdout));
		count = m0_compile(op, constants, cursor);
		assert(count == 9);
		assert(cursor[0].as_cptr == m0_core_get_ia);
		assert(cursor[1].as_uword == REG);
		assert(cursor[2].as_cptr == m0_core_set_ib);
		assert(cursor[3].as_uword == C);
		assert(cursor[4].as_cptr == m0_core_mul_ia);
		assert(cursor[5].as_cptr == m0_core_set_pb);
		assert(cursor[6].as_cptr == &out);
		assert(cursor[7].as_cptr == m0_core_store_u32a_pb);
		assert(cursor[8].as_word == 0);

		cursor += count;

		op = m0_op_iii(HALT, 0, 0, 0);
		assert(m0_disassemble(op, constants, stdout));
		count = m0_compile(op, constants, cursor);
		assert(count == 1);
		assert(cursor[0].as_cptr == m0_core_yield);

		m0_value regs[1] = { { .as_int = 0 } };
		m0_vm vm = { regs, code };

		m0_core(&vm);
		assert(out == (A + B) * C);
	}

	return 0;
}
