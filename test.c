#include "m0.h"

#define NDEBUG
#include <assert.h>

int main(void)
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

	assert(regs[0] == 42);

	m0_constant constants[] = {
		{ M0_CONST_INT, { .as_int = -5 } },
		{ M0_CONST_INT, { .as_int = 1024 } }
	};

	m0_op op = m0_op_rii(ADD, 0, 0, 1);
	m0_disassemble(op, constants, stdout);

	return 0;
}
