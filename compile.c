#include "m0.h"

enum { UNKNOWN, INT, FLOAT };

static const void *const OPS[] = {
	[M0_OP_HALT] = m0_core_yield,
	[M0_OP_ADD] = m0_core_add_ia,
	[M0_OP_MUL] = m0_core_mul_ia,
	[M0_OP_FADD] = m0_core_add_fa,
};

static const void *const SET[] = {
	[1] = m0_core_set_ia,
	[2] = m0_core_set_ib
};

static const void *const GET[] = {
	[1] = m0_core_get_ia,
	[2] = m0_core_get_ib
};

static const void *const LOAD32_I[] = {
	[1] = m0_core_load_u32a_ia,
	[2] = m0_core_load_u32b_ib
};

static const void *const STORE[] = {
	[M0_MEM_I8] = m0_core_store_i8a_pb,
	[M0_MEM_U8] = m0_core_store_u8a_pb,
	[M0_MEM_I16] = m0_core_store_i16a_pb,
	[M0_MEM_U16] = m0_core_store_u16a_pb,
	[M0_MEM_I32] = m0_core_store_i32a_pb,
	[M0_MEM_U32] = m0_core_store_u32a_pb,
	[M0_MEM_I64] = m0_core_store_i64a_pb,
	[M0_MEM_U64] = m0_core_store_u64a_pb,
};

size_t m0_compile(m0_op op, const m0_constant *constants, m0_value *buffer)
{
	unsigned code = op.code;
	unsigned mode = UNKNOWN;
	size_t count = 0;

	switch(code)
	{
		case M0_OP_HALT:
		buffer->as_cptr = OPS[code];
		return 1;

		case M0_OP_ADD:
		case M0_OP_MUL:
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

			case M0_ARG_REGISTER:
			switch(mode)
			{
				case INT:
				{
					buffer[count++].as_cptr = GET[i];
					buffer[count++].as_uword = op.args[i];
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

		case M0_ARG_MEMORY:
		{
			unsigned id = op.args[0];
			if(!constants[id].type == M0_CONST_ADDRESS)
				return 0;

			const m0_address *address = constants[id].value.as_cptr;

			if(address->base_symbol)
			{
				buffer[count++].as_cptr = m0_core_set_pb;
				buffer[count++].as_ptr = address->base_symbol->value;
			}
			else
			{
				buffer[count++].as_cptr = m0_core_get_pb;
				buffer[count++].as_uword = address->base_register;
			}

			if(address->offset_multiplier)
			{
				buffer[count++].as_cptr = m0_core_get_ib;
				buffer[count++].as_uword = address->offset_register;
				buffer[count++].as_cptr = m0_core_offset_pb;
				buffer[count++].as_word = address->offset_multiplier;
			}

			switch(mode)
			{
				case INT:
				switch(address->type)
				{
					case M0_MEM_I8:
					case M0_MEM_U8:
					case M0_MEM_I16:
					case M0_MEM_U16:
					case M0_MEM_I32:
					case M0_MEM_U32:
					case M0_MEM_I64:
					case M0_MEM_U64:
					buffer[count++].as_cptr = STORE[address->type];
					buffer[count++].as_word = address->displacement;
					break;

					default:
					return 0;
				}
				break;

				default:
				return 0;
			}

			break;
		}

		default:
		return 0;
	}

	return count;
}
