## Overview

This project demonstrates how the C++ `final` keyword can help the compiler optimize virtual function calls by enabling devirtualization, which removes the overhead of virtual dispatch.

---

## Code Summary

- **Classes:**
  - `Car`: A base class with a virtual method `Engine()`.
  - `Dodge`: A derived class marked with the `FINAL` macro (which expands to `final`), overriding `Engine()`.

- **`FINAL` Macro:**
  - Defined (in `"macro_final.h"`) as either `final` or empty depending on compile-time flags.
  - Controls whether the compiler treats classes and methods as `final`, prohibiting further overrides.

- **Benchmark Functions:**
  - Three timing loops measure the performance of calling `Engine()` in different ways:
    1. Via a `Car*` pointer to a dynamically allocated `Dodge` object (virtual call).
    2. Via a `Dodge*` pointer with a direct call to `Engine()` (non-virtual call, inlined).
    3. Via a `Dodge*` pointer passed to a function that calls `Engine()` through a base class pointer (`check_engine`).

---

## What Happens When Using `final`

- Marking the `Dodge` class and `Engine()` method as `final` signals to the compiler that:
  - No class can inherit from `Dodge`.
  - `Engine()` cannot be overridden further.
- This allows the compiler to:
  - **Devirtualize** calls to `Engine()` on `Dodge` objects.
  - Replace virtual calls (which use the vtable lookup at runtime) with **direct calls**.
  - **Inline** the method, eliminating call overhead entirely.
- Without `final`, the compiler must assume the pointer could point to a further derived class overriding `Engine()`, so it must emit a virtual call.

---

## Function Overhead Elimination

- Virtual calls incur a runtime cost due to indirection through the vtable.
- Using `final` enables the compiler and Link-Time Optimization (LTO) to:
  - Convert virtual calls into direct calls.
  - Inline the method body.
  - Reduce or eliminate function call overhead.
- This results in measurable performance improvements in the benchmark loops.

---

## Link-Time Optimization (LTO)
LTO optimizations happen at link time, not compile time, so your .obj files (or .o files on Linux) may appear empty or incomplete because the actual code generation hasn't happened yet — it's deferred to the link step.
LTO discards function symbols and debugging info by default in the final executable, making it hard to disassemble or inspect functions by name.

This happens because:

- Unused functions get inlined or removed entirely.

- Symbols may be merged, renamed, or stripped.

- You often end up with a flat blob of optimized machine code.

---

## How to Use This

1. Compile with the `FINAL` macro enabled or disabled to compare effects.
2. Run the program and observe the timing results:
   - Calls through `Car*` pointers will be slower without `final`.
   - Calls through `Dodge*` pointers are fast due to direct calls.
   - Calls via `check_engine` can be optimized if `final` is enabled, allowing devirtualization.
3. Use Link-Time Optimization (LTO) to maximize these optimizations.



## Example Output

```
PS C:\Users\MeriBadalyan\Devirtualization_Techniques> .\build\Release\final_test.exe 
Time for Car* calls: 37.4142 ms
Time for Dodge* calls: 36.1866 ms
Time for Dodge* calls: 30.2271 ms

PS C:\Users\MeriBadalyan\Devirtualization_Techniques> .\build\Release\no_final_test.exe 
Time for Car* calls: 25.9954 ms
Time for Dodge* calls: 26.3565 ms
Time for Dodge* calls: 36.4933 ms

PS C:\Users\MeriBadalyan\Devirtualization_Techniques> .\build\Release\final_test_lto.exe 
Time for Car* calls: 28.5577 ms
Time for Dodge* calls: 40.6317 ms
Time for Dodge* calls: 25.5727 ms
```


## Build Instructions

1. **Clone the repository**:
    ```bash
    git clone https://github.com/marybadalyan/Devirtualization_Techniques
    ```

2. **Navigate to the repository**:
    ```bash
    cd Devirtualization_Techniques
    ```

3. **Generate build files**:
    ```bash
    cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
    ```

4. **Build the project**:
    ```bash
    cmake --build build --config Release
    ```

5. **Run the executable** from the build directory:
    ```bash
    ./build/Devirtualization_Techniques
    ```

---

## Summary

This project illustrates how careful use of the `final` keyword, combined with compiler optimizations like LTO, can greatly reduce the overhead of virtual function calls by enabling devirtualization and inlining, leading to faster and more efficient C++ programs.
