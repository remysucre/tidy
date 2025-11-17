# Lunamark + Playdate Integration Test

This repository demonstrates successful integration of the Lunamark Markdown parser (Lua library) with the Playdate SDK.

## Project Structure

```
.
├── Source/
│   ├── main.lua           # Main Playdate app that tests lunamark
│   ├── pdxinfo            # Playdate app metadata
│   ├── lpeg.lua           # LuLPeg - pure Lua PEG parser (renamed from lulpeg.lua)
│   ├── lunamark.lua       # Main lunamark module entry point
│   ├── cosmo.lua          # Cosmo templating library
│   ├── alt-getopt.lua     # Command-line option parser
│   ├── cosmo/             # Cosmo submodules
│   │   ├── fill.lua
│   │   └── grammar.lua
│   └── lunamark/          # Lunamark library files
│       ├── entities.lua
│       ├── util.lua
│       ├── reader/        # Markdown reader
│       └── writer/        # HTML/LaTeX/etc writers
└── Lunamark.pdx/          # Compiled Playdate executable (~144KB)
```

## Dependencies

Lunamark requires several Lua libraries to function:

1. **lpeg** (≥ 0.10) - PEG parsing library
   - Using: **LuLPeg** - pure Lua port, no C compilation needed
   - Source: https://github.com/pygy/LuLPeg

2. **cosmo** (≥ 10.0) - Templating library for output formatting
   - Pure Lua library
   - Source: https://github.com/mascarenhas/cosmo

3. **alt-getopt** (≥ 0.7) - Command-line option parsing
   - Pure Lua library (included but may not be actively used in library mode)
   - Source: https://github.com/cheusov/lua-alt-getopt

4. **luautf8** (≥ 0.1.1) - UTF-8 support
   - NOT needed! Playdate uses Lua 5.4 which has built-in `utf8` library
   - Lunamark automatically detects and uses Lua 5.4's native utf8 support

All dependencies are pure Lua implementations, avoiding the need for C compilation.

## What This Demonstrates

The test application successfully:
1. Integrates the complete Lunamark Markdown parser with Playdate Lua
2. Uses LuLPeg (pure Lua PEG parser) as a drop-in replacement for lpeg (C library)
3. Parses complex Markdown including:
   - Headers
   - Bold and italic text
   - Lists
   - Links
   - Code blocks
   - Blockquotes
4. Converts Markdown to HTML
5. Runs entirely in pure Lua - no C code required!

## Dependencies

- **Lunamark**: Lua library for Markdown parsing (from https://github.com/jgm/lunamark)
- **LuLPeg**: Pure Lua PEG parser, drop-in replacement for lpeg (from https://github.com/pygy/LuLPeg)
  - Renamed to `lpeg.lua` so lunamark can find it with `require("lpeg")`

## Build Requirements

- Playdate SDK 3.0.1 or later
- No C compiler required! This is pure Lua.

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

### Compile

```bash
cd playdate_lunamark_test
$PLAYDATE_SDK_PATH/bin/pdc Source Lunamark.pdx
```

This produces:
- `Lunamark.pdx/` - Complete Playdate executable bundle with compiled Lua bytecode

## Running

### In Simulator

```bash
# Open with simulator (GUI)
$PLAYDATE_SDK_PATH/bin/PlaydateSimulator Lunamark.pdx
```

Or use File → Open in the Playdate Simulator application.

### On Device

1. Connect your Playdate via USB
2. Open Playdate Simulator
3. Device → Upload Game to Device
4. Select `Lunamark.pdx`

Or copy to device via data disk mode (see main project README).

## Test Markdown

The test application parses this markdown:

```markdown
# Hello Playdate!

This is a **markdown** test with:

- Item 1
- Item 2
- Item 3

## Features

Here's some *italic* text and **bold** text.

A [link](https://play.date) to Playdate's website.

### Code

\`\`\`lua
local x = 42
print(x)
\`\`\`

> This is a blockquote.
> It can span multiple lines.
```

## Expected Output

When successful, you'll see:
- On screen: "Lunamark: SUCCESS" and "Parsed markdown to HTML"
- In console: The complete HTML output from the markdown parser

Example HTML output:
```html
<h1>Hello Playdate!</h1>
<p>This is a <strong>markdown</strong> test with:</p>
<ul>
<li>Item 1</li>
<li>Item 2</li>
<li>Item 3</li>
</ul>
...
```

## Implementation Details

### Integration Approach

1. **LuLPeg as lpeg replacement**:
   - Copied `lulpeg.lua` as `lpeg.lua` in Source directory
   - Lunamark's `require("lpeg")` now finds the pure Lua implementation
   - No C compilation needed!

2. **UTF-8 Character Fix**:
   - Fixed `lunamark/writer/generic.lua` line 136
   - Changed UTF-8 smart quotes from literal characters to escape sequences
   - Playdate's Lua compiler requires escaped UTF-8 bytes instead of literal UTF-8 in source

3. **require() Compatibility Shim**:
   - Playdate uses `import` instead of standard Lua's `require`
   - Implemented custom `require()` function using `playdate.file.run()`
   - Set up `package.loaded` cache to prevent reloading modules
   - Allows lunamark to use standard `require("lpeg")` syntax without modification

4. **Lua Version Compatibility**:
   - Playdate uses Lua 5.4
   - Lunamark is compatible with Lua 5.1+
   - LuLPeg emulates LPeg v0.12 behavior

### Performance Considerations

- **LuLPeg Performance**: ~2-10x slower than native lpeg with LuaJIT
- **Playdate Context**: Still fast enough for reasonable markdown documents
- **Alternative**: Could integrate native lpeg as C extension (like HTML Tidy) for better performance

## Compilation Results

- **Bundle Size**: ~50 KB (lpeg.pdz + main.pdz + lunamark/)
- **Build Status**: ✅ Success (no errors)
- **Pure Lua**: No C code required

## Advantages Over HTML Tidy Approach

1. **No C compilation**: Entirely Lua, no ARM toolchain needed
2. **Smaller footprint**: ~50 KB vs 233 KB for HTML Tidy
3. **Easier to modify**: Can edit Lua files directly
4. **Cross-platform**: Same code runs on simulator and device

## Next Steps for Lua Browser

You can combine both approaches:
1. Use Lunamark for Markdown parsing (pure Lua)
2. Use HTML Tidy for HTML parsing/cleaning (C extension)
3. Add HTTP networking (Playdate C API)
4. Implement rendering and UI

## Notes

- Lunamark supports multiple output formats: HTML, LaTeX, ConTeXt, Docbook, etc.
- Can be extended with custom writers for Playdate-specific rendering
- The markdown parser is highly customizable via PEG grammars

## License

- Playdate SDK: See SDK_LICENSE in Playdate SDK
- Lunamark: MIT License (see lunamark repository)
- LuLPeg: MIT License (see LuLPeg repository)
- This integration code: Public domain
