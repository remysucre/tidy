# HTML Tidy & Lunamark + Playdate Integration Tests

This repository demonstrates successful integration of parsing libraries with the Playdate SDK for building a browser:
1. **HTML Tidy** (C library) - HTML parsing and cleaning
2. **Lunamark** (Lua library) - Markdown parsing

## Project Structure

```
.
├── playdate_tidy_test/       # C-based HTML Tidy integration
│   ├── src/
│   │   ├── main.c            # Main application code
│   │   └── syscalls.c        # Syscall stubs for device
│   ├── Makefile              # Build configuration
│   └── TidyTest.pdx/         # Compiled executable (233 KB device, 1.8 MB sim)
│
├── playdate_lunamark_test/   # Lua-based Markdown parser integration
│   ├── Source/
│   │   ├── main.lua          # Main Lua application
│   │   ├── lpeg.lua          # LuLPeg (pure Lua PEG parser)
│   │   └── lunamark/         # Lunamark library
│   └── Lunamark.pdx/         # Compiled executable (~50 KB)
│
└── tidy-html5/               # HTML Tidy library source code
```

## What This Demonstrates

### HTML Tidy Test (C-based)

The HTML Tidy test application successfully:
1. Integrates the complete HTML Tidy library (tidy-html5) with Playdate C SDK
2. Compiles all tidy source files into the Playdate executable
3. Exercises core tidy functionality including:
   - Creating a tidy document (`tidyCreate()`)
   - Setting tidy options (`tidyOptSetBool()`, `tidyOptSetInt()`)
   - Parsing HTML strings (`tidyParseString()`)
   - Cleaning and repairing HTML (`tidyCleanAndRepair()`)
   - Running diagnostics (`tidyRunDiagnostics()`)
   - Saving output to buffer (`tidySaveBuffer()`)

### Lunamark Test (Pure Lua)

The Lunamark test application successfully:
1. Integrates the complete Lunamark Markdown parser (pure Lua)
2. Uses LuLPeg as a drop-in replacement for lpeg (no C code needed)
3. Parses complex Markdown including:
   - Headers (H1, H2, H3)
   - Bold and italic text
   - Ordered and unordered lists
   - Links and URLs
   - Code blocks (fenced and indented)
   - Blockquotes
4. Converts Markdown to HTML
5. Runs entirely in Lua - no C compilation required!

## Build Requirements

### For HTML Tidy (C-based)
- Playdate SDK 3.0.1 or later
- GCC compiler (for simulator builds)
- ARM GCC toolchain `gcc-arm-none-eabi` (for device builds)

### For Lunamark (Lua-based)
- Playdate SDK 3.0.1 or later
- No C compiler required!

## Building

### Setup

1. Install the Playdate SDK:
   ```bash
   # Download from https://play.date/dev/
   # Extract and run setup.sh
   ```

2. Set the SDK path:
   ```bash
   export PLAYDATE_SDK_PATH=/path/to/PlaydateSDK
   ```

3. Install ARM toolchain (for device builds):
   ```bash
   # Ubuntu/Debian
   apt-get install gcc-arm-none-eabi

   # macOS (installed by SDK)
   # Included with Playdate SDK installer
   ```

### Compile for Simulator

```bash
cd playdate_tidy_test
make simulator
```

This produces:
- `build/pdex.so` - Shared library for the Playdate simulator (1.8 MB)
- `TidyTest.pdx/pdex.so` - Simulator library in PDX bundle

### Compile for Device (Playdate Hardware)

```bash
cd playdate_tidy_test
make device
```

This produces:
- `build/pdex.elf` - ELF executable with debug symbols (3.6 MB)
- `TidyTest.pdx/pdex.bin` - Device firmware binary (233 KB)
- `Source/pdex.elf` - Copy of ELF for distribution

### Compile Both (Simulator + Device)

```bash
cd playdate_tidy_test
make all
```

This builds both simulator and device versions in one command.

### Clean Build

```bash
make clean
```

## Implementation Details

### Integration Approach

The HTML Tidy library is integrated by:
1. Including all necessary `.c` source files from `tidy-html5/src/` in the Makefile (excluding `mappedio.c` which requires POSIX)
2. Adding include paths for both `tidy-html5/include/` and `tidy-html5/src/`
3. Defining required preprocessor macros:
   - `SUPPORT_UTF16_ENCODINGS=1`
   - `SUPPORT_ASIAN_ENCODINGS=1`
   - `SUPPORT_ACCESSIBILITY_CHECKS=1`
   - `SUPPORT_POSIX_MAPPED_FILES=0` (disabled for Playdate)
4. Providing minimal syscall stubs in `src/syscalls.c` for device builds (newlib requirements)

### Test Application

The test app (`src/main.c`):
1. Defines a sample malformed HTML string
2. In the `eventHandler` during `kEventInit`:
   - Creates a tidy document
   - Configures it for XHTML output
   - Parses the test HTML
   - Cleans and repairs it
   - Logs all output and errors to the Playdate console
3. Displays success/failure status on the Playdate screen

### Compilation Results

**Simulator Build:**
- **Compiled Library Size**: 1.8 MB (pdex.so)
- **Build Status**: ✅ Success (warnings only, no errors)

**Device Build:**
- **Device Binary Size**: 233 KB (pdex.bin) - fits easily on Playdate!
- **Debug Symbols**: 3.6 MB (pdex.elf) - for development/debugging only
- **Build Status**: ✅ Success (warnings only, no errors)
- **Symbol Verification**: All tidy API functions present in compiled binary

Key verified symbols:
- `tidyCreate` ✓
- `tidyParseString` ✓
- `tidyCleanAndRepair` ✓
- `tidySaveBuffer` ✓
- Plus 100+ additional tidy functions

## Comparison: C vs Lua Approach

| Feature | HTML Tidy (C) | Lunamark (Lua) |
|---------|---------------|----------------|
| **Language** | C | Pure Lua |
| **Size (Device)** | 233 KB | ~50 KB |
| **Size (Simulator)** | 1.8 MB | ~50 KB |
| **Build Complexity** | Requires ARM toolchain, Makefile, syscalls | Just `pdc` command |
| **Performance** | Native C speed | Lua (slower but sufficient) |
| **Modifiability** | Requires C recompilation | Edit Lua files directly |
| **Cross-platform** | Different builds for sim/device | Same code everywhere |
| **Dependencies** | None (self-contained) | LuLPeg (pure Lua) |
| **Use Case** | HTML parsing/cleaning | Markdown parsing |
| **Integration Effort** | High (C integration, syscalls, linking) | Low (copy files, fix UTF-8) |

### Recommendations

**Use C approach when:**
- Need maximum performance
- Working with existing C libraries
- Require complex low-level operations
- Can handle C compilation toolchain

**Use Lua approach when:**
- Library available in pure Lua
- Rapid development/iteration needed
- Want simpler builds
- Performance is acceptable

**Hybrid approach (Best for browser):**
- Use C for HTML Tidy (performance-critical HTML parsing)
- Use Lua for Lunamark (Markdown parsing)
- Use Lua for UI, navigation, and app logic
- Expose C functions to Lua where needed

## Usage for Lua Browser

This test confirms that the HTML Tidy library can be successfully compiled and linked with Playdate applications. For a Lua browser on Playdate, you could:

1. **Pure C Approach**: Build the entire browser in C using this integration method
2. **Hybrid Approach**: Expose tidy functions to Lua via the C API's Lua interop features
3. **Library Approach**: Build tidy as a reusable Playdate library that Lua code can call

The compiled binary size (1.8 MB) should be acceptable for Playdate, which supports applications up to several megabytes.

## Next Steps for a Playdate Browser

Combining both approaches, a full browser could include:

### Core Parsing
1. **HTML**: Use HTML Tidy (C) for robust HTML parsing and error correction
2. **Markdown**: Use Lunamark (Lua) for rendering Markdown pages

### Networking
3. Implement HTTP/HTTPS requests using Playdate C API
4. Handle redirects, headers, cookies

### Rendering
5. Create a simple layout engine in Lua
6. Implement basic CSS support (subset: fonts, colors, margins)
7. Render to Playdate's 400x240 1-bit display

### UI/Navigation
8. URL bar and controls (Playdate crank for scrolling?)
9. History and bookmarks
10. Tab support (if memory allows)

### Optimizations
11. Caching (memory and disk)
12. Progressive rendering
13. Image dithering for 1-bit display

## License

- Playdate SDK: See SDK_LICENSE in Playdate SDK
- HTML Tidy: MIT-style license (see tidy-html5 repository)
- Lunamark: MIT License (see lunamark repository)
- LuLPeg: MIT License (see LuLPeg repository)
- This integration code: Public domain

## See Also

- [playdate_tidy_test/README.md](playdate_tidy_test/README.md) - Detailed HTML Tidy integration docs
- [playdate_lunamark_test/README.md](playdate_lunamark_test/README.md) - Detailed Lunamark integration docs
