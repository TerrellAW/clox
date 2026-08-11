<div align="center">

# Bytecode Reference

</div>

## Operation Code Table

| Opcode        | Offset | Binary       | Decimal |
| ------------- | ------ | ------------ | ------- |
| [`OP_CONSTANT`](#OP_CONSTANT) | 2      | 0b00000000   | 0       |
| [`OP_NIL`](#OP_NIL)      | 1      | 0b00000001   | 1       |
| [`OP_TRUE`](#OP_TRUE)     | 1      | 0b00000010   | 2       |
| [`OP_FALSE`](#OP_FALSE)    | 1      | 0b00000011   | 3       |
| [`OP_POP`](#OP_POP)      | 1      | 0b00000100   | 4       |
| [`OP_GET_LOCAL`](#OP_GET_LOCAL) | 2     | 0b00000101   | 5       |
| [`OP_SET_LOCAL`](#OP_SET_LOCAL) | 2     | 0b00000110   | 6       |
| [`OP_GET_GLOBAL`](#OP_GET_GLOBAL) | 2    | 0b00000111   | 7       |
| [`OP_DEFINE_GLOBAL`](#OP_DEFINE_GLOBAL) | 2 | 0b00001000   | 8       |
| [`OP_SET_GLOBAL`](#OP_SET_GLOBAL) | 2    | 0b00001001   | 9       |
| [`OP_EQUAL`](#OP_EQUAL)    | 1      | 0b00001010   | 10      |
| [`OP_GREATER`](#OP_GREATER)  | 1      | 0b00001011   | 11      |
| [`OP_LESS`](#OP_LESS)     | 1      | 0b00001100   | 12      |
| [`OP_ADD`](#OP_ADD)      | 1      | 0b00001101   | 13      |
| [`OP_SUBTRACT`](#OP_SUBTRACT) | 1      | 0b00001110   | 14      |
| [`OP_MULTIPLY`](#OP_MULTIPLY) | 1      | 0b00001111   | 15      |
| [`OP_DIVIDE`](#OP_DIVIDE)   | 1      | 0b00010000   | 16      |
| [`OP_NOT`](#OP_NOT)      | 1      | 0b00010001   | 17      |
| [`OP_NEGATE`](#OP_NEGATE)   | 1      | 0b00010010   | 18      |
| [`OP_PRINT`](#OP_PRINT)    | 1      | 0b00010011   | 19      |
| [`OP_RETURN`](#OP_RETURN)   | 1      | 0b00010100   | 20      |

## `OP_CONSTANT`

**Purpose:** Stores the given value to the stack.

**Inputs:** Index to a value stored in the constants pool, stored in the second byte.

**Outputs:** Value pushed onto the stack.

```asm
// Offset - Opcode - Index
0000    OP_CONSTANT     0
0002    OP_CONSTANT     1
// Stack: [constants[0]][constants[1]] stackTop
```

## `OP_NIL`

**Purpose:** Stores a nil literal on the stack.

**Inputs:** None.

**Outputs:** Nil pushed onto the stack.

```asm
// Offset - Opcode - Index
0000    OP_NIL
0001    OP_NIL
// Stack: [nil][nil] stackTop
```

## `OP_TRUE`

**Purpose:** Stores a true boolean literal on the stack.

**Inputs:** None.

**Outputs:** True pushed onto the stack.

```asm
// Offset - Opcode - Index
0000    OP_TRUE
0001    OP_TRUE
// Stack: [true][true] stackTop
```

## `OP_FALSE`

**Purpose:** Stores a false boolean literal on the stack.

**Inputs:** None.

**Outputs:** False pushed onto the stack.

```asm
// Offset - Opcode - Index
0000    OP_FALSE
0001    OP_FALSE
// Stack: [false][false] stackTop
```

## `OP_POP`

**Purpose:** Pops value from the stack. Appears after an expression with an unused result or a scope with local variables that need to be discarded.

**Inputs:** Value from the top of the stack.

**Outputs:** None.

```asm
// Offset - Opcode - Index
0000    OP_FALSE
0001    OP_POP
// Stack: stackTop
```

## `OP_GET_LOCAL`

**Purpose:** Get value associated with local variable and push to top of stack.

**Inputs:** Slot for variable value in stack.

**Outputs:** Value of local variable pushed to top of stack where it can be used.

```asm
// Offset - Opcode - Index
0000    OP_GET_LOCAL    3
// Stack: [stuff][constants[3]] stackTop
```

## `OP_SET_LOCAL`

**Purpose:** Associate value at top of stack with local variable.

**Inputs:** Slot for variable value in stack.

**Outputs:** Local variable added to list and tracked for removal when scope ends.

```asm
// Offset - Opcode - Index
0000    OP_CONSTANT     1
0002    OP_SET_LOCAL    0
// Stack: [constants[1]] stackTop
```

## `OP_GET_GLOBAL`

**Purpose:** Push value associated with a global variable to the stack.

**Inputs:** Identifier index from constants pool.

**Outputs:** Global variable's value pushed to stack.

```asm
// Offset - Opcode - Index
0000    OP_GET_GLOBAL   0
// Stack: [constants[0]->value] stackTop
```

## `OP_DEFINE_GLOBAL`

**Purpose:** Create a global variable.

**Inputs:** Value from top of stack and identifier from globals table.

**Outputs:** Pops value from the stack and stores it, identifier added to constants pool.

```asm
// Offset - Opcode - Index
0000    OP_NIL
0001    OP_DEFINE_GLOBAL 0
// Stack: stackTop
```

## `OP_SET_GLOBAL`

**Purpose:** Update value of an existing global variable.

**Inputs:** Value from top of stack and identifier from constants pool.

**Outputs:** Replaces an existing global's value with value from the top of the stack.

```asm
// Offset - Opcode - Index
0000    OP_CONSTANT     1
0002    OP_SET_GLOBAL   0
// Stack: [constants[1]] stackTop
```

## `OP_EQUAL`

**Purpose:** Pops two values from the stack and compares their equality.

**Inputs:** Two values from the top of the stack.

**Outputs:** True if two values are the same, false otherwise. Result pushed onto the stack.

```asm
// Offset - Opcode - Index
0000    OP_TRUE
0001    OP_FALSE
0002    OP_EQUAL
// Stack: [false] stackTop
```

## `OP_GREATER`

**Purpose:** Pops two values from the stack and compares their numeric value.

**Inputs:** Two values from the top of the stack.

**Outputs:** True if the first value is greater than the second, false otherwise. Result pushed onto the stack.

```asm
// Offset - Opcode - Index
0000    OP_CONSTANT     0
0002    OP_CONSTANT     1
0004    OP_GREATER
// Stack: [constants[0] > constants[1]] stackTop
```

## `OP_LESS`

**Purpose:** Pops two values from the stack and compares their numeric value.

**Inputs:** Two values from the top of the stack.

**Outputs:** True if the first value is less than the second, false otherwise. Result pushed onto the stack.

```asm
// Offset - Opcode - Index
0000    OP_CONSTANT     0
0002    OP_CONSTANT     1
0004    OP_LESS
// Stack: [constants[0] < constants[1]] stackTop
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
// Stack: [constants[0] + constants[1]] stackTop
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
// Stack: [constants[0] - constants[1]] stackTop
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
// Stack: [constants[0] * constants[1]] stackTop
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
// Stack: [constants[0] / constants[1]] stackTop
```

## `OP_NOT`

**Purpose:** Pops a value from the stack and falsifies it.

**Inputs:** One value from the top of the stack.

**Outputs:** Result from falsification pushed onto the stack.

```asm
// Offset - Opcode - Index
0000    OP_TRUE
0001    OP_NOT
// Stack: [false] stackTop
```

## `OP_NEGATE`

**Purpose:** Pops a value from the stack and negates it.

**Inputs:** One value from the top of the stack.

**Outputs:** Result from negation pushed onto the stack.

```asm
// Offset - Opcode - Index
0000    OP_CONSTANT     0
0002    OP_NEGATE
// Stack: [-constants[0]] stackTop
```

## `OP_PRINT`

**Purpose:** Pop a value from the stack and print it.

**Inputs:** One value from the top of the stack.

**Outputs:** The value from the top of the stack to stdout.

```asm
// Offset - Opcode - Index
0000    OP_TRUE
0001    OP_PRINT
// Stack: stackTop
// stdout: true
```

## `OP_RETURN`

**Purpose:** Exits the interpreter (temporary).

**Inputs:** None (temporary).

**Outputs:** None (temporary).

```asm
// Offset - Opcode - Index
0000    OP_CONSTANT     0
0002    OP_RETURN
// Stack: stackTop
```

