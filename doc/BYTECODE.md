<div align="center">

# Bytecode Reference

</div>

## Operation Code Table

| Opcode        | Offset | Binary       | Decimal |
| ------------- | ------ | ------------ | ------- |
| [`OP_CONSTANT`](#OP_CONSTANT) | 2      | 0b00000000   | 0       |
| [`OP_ADD`](#OP_ADD)      | 1      | 0b00000001   | 1       |
| [`OP_SUBTRACT`](#OP_SUBTRACT) | 1      | 0b00000010   | 2       |
| [`OP_MULTIPLY`](#OP_MULTIPLY) | 1      | 0b00000011   | 3       |
| [`OP_DIVIDE`](#OP_DIVIDE)   | 1      | 0b00000100   | 4       |
| [`OP_NEGATE`](#OP_NEGATE)   | 1      | 0b00000101   | 5       |
| [`OP_RETURN`](#OP_RETURN)   | 1      | 0b00000110   | 6       |

## `OP_CONSTANT`

**Purpose:** Stores the given value to the stack.

**Inputs:** Index to a value stored in the constants pool, stored in the second byte.

**Outputs:** Value pushed onto the stack.

```asm
// Offset - Opcode - Index
0000    OP_CONSTANT     0
0002    OP_CONSTANT     1
// Stack: [constants[0]][constants[1]]
```

## `OP_ADD`

**Purpose:** Pops two values from the stack and adds them together.

**Inputs:** Two values from the top of the stack.

**Outputs:** Result from addition pushed onto the stack.

```asm
// Offset - Opcode - Index
0000    OP_CONSTANT     0
0002    OP_CONSTANT     1
0004    OP_ADD
// Stack: [constants[0] + constants[1]]
```

## `OP_SUBTRACT`

**Purpose:** Pops two values from the stack and subtracts them.

**Inputs:** Two values from the top of the stack.

**Outputs:** Result from subtraction pushed onto the stack.

```asm
// Offset - Opcode - Index
0000    OP_CONSTANT     0
0002    OP_CONSTANT     1
0004    OP_SUBTRACT
// Stack: [constants[0] - constants[1]]
```

## `OP_MULTIPLY`

**Purpose:** Pops two values from the stack and multiplies them.

**Inputs:** Two values from the top of the stack.

**Outputs:** Result from multiplication pushed onto the stack.

```asm
// Offset - Opcode - Index
0000    OP_CONSTANT     0
0002    OP_CONSTANT     1
0004    OP_MULTIPLY
// Stack: [constants[0] * constants[1]]
```

## `OP_DIVIDE`

**Purpose:** Pops two values from the stack and divides them.

**Inputs:** Two values from the top of the stack.

**Outputs:** Result from division pushed onto the stack.

```asm
// Offset - Opcode - Index
0000    OP_CONSTANT     0
0002    OP_CONSTANT     1
0004    OP_DIVIDE
// Stack: [constants[0] / constants[1]]
```

## `OP_NEGATE`

**Purpose:** Pops a value from the stack and negates it.

**Inputs:** One value from the top of the stack.

**Outputs:** Result from negation pushed onto the stack.

```asm
// Offset - Opcode - Index
0000    OP_CONSTANT     0
0002    OP_NEGATE
// Stack: [-constants[0]]
```

## `OP_RETURN`

**Purpose:** Pop a value from the stack and return it.

**Inputs:** One value from the top of the stack.

**Outputs:** The value from the top of the stack.

```asm
// Offset - Opcode - Index
0000    OP_CONSTANT     0
0002    OP_RETURN
// Stack:
```

