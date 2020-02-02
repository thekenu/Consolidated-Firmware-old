/* This file is auto-generated. DO NOT MODIFY! */

// How-to Use:
// 1. One "control block" is generated for each function we intend to override,
//    and it can be retrieved using get_override()
// 2. The original function should be replaced with get_override()->non_override
// 3. set_xxx_override_hook() is weakly defined as an empty function,
//    and the user is expected to provide a meaningful function definition.
// 4. The override function can be called using get_override()->override, BUT
//    the user is expected to call ->disable_override() after.

/* Headers */
#include <stdbool.h>
#include <stdint.h>

/* Macros */
// TODO: Auto-generate control blocks for every function in this ELF section
// using elftools
#define TL85 __attribute__((section(".lf.funcs"), used))

/* Function Pointer Typedefs */
typedef void (
    *setter_func)(); /* Note the variable parameter for this function pointer */
typedef void (*stop_override_func)(void);

/* Structs */
struct setter
{
    setter_func        normal_setter;
    setter_func        override_setter;
    stop_override_func disable_override;
    bool               is_override_enabled;
};

// TODO: Compile this as a static library and only expose get_override()
// and override_offset using xxx.version, or linker options.
