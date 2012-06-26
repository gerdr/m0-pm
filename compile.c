#include "m0.h"

size_t m0_compile(m0_op op, const m0_constant *constants, m0_value *buffer)
{
	enum { UNKNOWN, INT, FLOAT };

	static const void *const OPS[] = {
		[M0_OP_HALT] = m0_core_yield,
		[M0_OP_ADD] = m0_core_add_ia,
		[M0_OP_FADD] = m0_core_add_fa,
	};

	unsigned code = op.code;
	unsigned mode = UNKNOWN;
	size_t count = 0;

	switch(code)
	{
		case M0_OP_HALT:
		buffer->as_cptr = OPS[code];
		return 1;

		case M0_OP_ADD:
		mode = INT;
		goto BINOP;

		case M0_OP_FADD:
		mode = FLOAT;
		goto BINOP;

		default:
		return 0;
	}

BINOP:
	for(unsigned i = 1; i <= 2; ++i)
	{
		static const void *const SET[] = {
			[1] = m0_core_set_ia,
			[2] = m0_core_set_ib
		};

		static const void *const LOAD32_I[] = {
			[1] = m0_core_load_u32_ia,
			[2] = m0_core_load_u32_ib
		};

		switch(m0_argtype(op, i))
		{
			case M0_ARG_IMMEDIATE:
			switch(mode)
			{
				case INT:
				{
					unsigned id = op.args[i];
					if(constants[id].type != M0_CONST_INT)
						return 0;

					buffer[count++].as_cptr = SET[i];
					buffer[count++].as_uint = constants[id].value.as_uint;
					break;
				}

				default:
				return 0;
			}
			break;

			default:
			return 0;
		}
	}

	buffer[count++].as_cptr = OPS[code];

	switch(m0_argtype(op, 0))
	{
		case M0_ARG_REGISTER:
		switch(mode)
		{
			case INT:
			buffer[count++].as_cptr = m0_core_put_ia;
			buffer[count++].as_uword = op.args[0];
			break;

			default:
			return 0;
		}
		break;

		default:
		return 0;
	}

	return count;
}
