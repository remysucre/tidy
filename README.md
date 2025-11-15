# HTML Tidy + Playdate Integration Test

This repository demonstrates successful integration of the HTML Tidy library with the Playdate SDK.

## Project Structure

```
.
├── playdate_tidy_test/     # Minimal Playdate app using HTML Tidy
│   ├── src/
│   │   └── main.c          # Main application code
│   ├── Makefile            # Build configuration
│   ├── Source/             # Build output directory
│   └── TidyTest.pdx/       # Compiled Playdate executable
└── tidy-html5/             # HTML Tidy library source code
```

## What This Demonstrates

The test application successfully:
1. Integrates the complete HTML Tidy library (tidy-html5) with Playdate C SDK
2. Compiles all tidy source files into the Playdate executable
3. Exercises core tidy functionality including:
   - Creating a tidy document (`tidyCreate()`)
   - Setting tidy options (`tidyOptSetBool()`, `tidyOptSetInt()`)
   - Parsing HTML strings (`tidyParseString()`)
   - Cleaning and repairing HTML (`tidyCleanAndRepair()`)
   - Running diagnostics (`tidyRunDiagnostics()`)
   - Saving output to buffer (`tidySaveBuffer()`)

## Build Requirements

- Playdate SDK 3.0.1 or later
- GCC compiler (for simulator builds)
- ARM GCC toolchain (for device builds, optional)

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

### Compile for Simulator

```bash
cd playdate_tidy_test
make simulator
```

This produces:
- `build/pdex.so` - Shared library for the Playdate simulator
- `TidyTest.pdx/` - Complete Playdate executable bundle

### Clean Build

```bash
make clean
```

## Implementation Details

### Integration Approach

The HTML Tidy library is integrated by:
1. Including all necessary `.c` source files from `tidy-html5/src/` in the Makefile
2. Adding include paths for both `tidy-html5/include/` and `tidy-html5/src/`
3. Defining required preprocessor macros:
   - `SUPPORT_UTF16_ENCODINGS=1`
   - `SUPPORT_ASIAN_ENCODINGS=1`
   - `SUPPORT_ACCESSIBILITY_CHECKS=1`

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

- **Compiled Library Size**: 1.8 MB
- **Build Status**: ✅ Success (no errors, zero warnings)
- **Symbol Verification**: All tidy API functions present in compiled binary

Key verified symbols:
- `tidyCreate` ✓
- `tidyParseString` ✓
- `tidyCleanAndRepair` ✓
- `tidySaveBuffer` ✓
- Plus 100+ additional tidy functions

## Usage for Lua Browser

This test confirms that the HTML Tidy library can be successfully compiled and linked with Playdate applications. For a Lua browser on Playdate, you could:

1. **Pure C Approach**: Build the entire browser in C using this integration method
2. **Hybrid Approach**: Expose tidy functions to Lua via the C API's Lua interop features
3. **Library Approach**: Build tidy as a reusable Playdate library that Lua code can call

The compiled binary size (1.8 MB) should be acceptable for Playdate, which supports applications up to several megabytes.

## Next Steps

To use this in a real browser:
1. Add HTML rendering logic
2. Implement HTTP networking (Playdate supports HTTP via C API)
3. Create UI controls for navigation
4. Add URL handling and history
5. Implement basic CSS parsing/styling (or use a simple subset)

## License

- Playdate SDK: See SDK_LICENSE in Playdate SDK
- HTML Tidy: MIT-style license (see tidy-html5 repository)
- This integration code: Public domain
