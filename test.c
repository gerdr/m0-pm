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

		m0_constant constants[] = {
			{ M0_CONST_INT, { .as_int = A } },
			{ M0_CONST_INT, { .as_int = B } },
			{ M0_CONST_INT, { .as_int = C } }
		};

		m0_value code[42];
		size_t count = 0;

		m0_value *cursor = code;

		count = m0_compile(m0_op_rii(ADD, REG, 0, 1), constants, cursor);
		assert(count == 7);
		assert(cursor[0].as_cptr == m0_core_set_ia);
		assert(cursor[1].as_int == A);
		assert(cursor[2].as_cptr == m0_core_set_ib);
		assert(cursor[3].as_int == B);
		assert(cursor[4].as_cptr == m0_core_add_ia);
		assert(cursor[5].as_cptr == m0_core_put_ia);
		assert(cursor[6].as_uword == REG);

		cursor += count;

		count = m0_compile(m0_op_rri(MUL, REG, 0, 2), constants, cursor);
		assert(count == 7);
		assert(cursor[0].as_cptr == m0_core_get_ia);
		assert(cursor[1].as_uword == REG);
		assert(cursor[2].as_cptr == m0_core_set_ib);
		assert(cursor[3].as_int == C);
		assert(cursor[4].as_cptr == m0_core_mul_ia);
		assert(cursor[5].as_cptr == m0_core_put_ia);
		assert(cursor[6].as_uword == REG);

		cursor += count;

		count = m0_compile(m0_op_iii(HALT, 0, 0, 0), constants, cursor);
		assert(count == 1);
		assert(cursor[0].as_cptr == m0_core_yield);

		m0_value regs[1] = { { .as_int = 0 } };
		m0_vm vm = { regs, code };

		m0_core(&vm);
		assert(regs[0].as_int == (A + B) * C);
	}

	{
		m0_address address = { 0, NULL, 10, 4, 9, 1 };

		m0_constant constants[] = {
			{ M0_CONST_ADDRESS, { .as_cptr = &address } },
			{ M0_CONST_INT, { .as_int = 12 } },
			{ M0_CONST_INT, { .as_int = 42 } }
		};

		m0_disassemble(m0_op_mii(ADD, 0, 1, 2), constants, stdout);
	}

	return 0;
}
