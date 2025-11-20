# cmark CommonMark + Playdate Integration Test

This repository demonstrates integration of the cmark CommonMark parser with the Playdate SDK.

## Project Structure

```
.
├── Source/
│   ├── main.lua           # Main Playdate app that tests cmark
│   ├── pdxinfo            # Playdate app metadata
│   └── cmark.lua          # cmark module (stub implementation)
└── README.md
```

## About cmark

cmark is the reference implementation of CommonMark, a strongly defined, highly compatible specification of Markdown. Unlike traditional Markdown parsers, cmark:

- Follows the CommonMark spec precisely
- Provides an AST (Abstract Syntax Tree) representation
- Is typically a C library with language bindings

## Current Implementation

This test includes a **stub implementation** of cmark that provides basic CommonMark-to-HTML conversion. The stub demonstrates the expected API:

- `cmark.parse_document(text)` - Parse CommonMark text into a document node
- `cmark.render_html(node)` - Render a document node to HTML string

### Stub Features

The current stub supports:
- Headers (h1, h2, h3)
- Bold and italic text
- Unordered lists
- Links
- Code blocks
- Blockquotes

## What This Demonstrates

The test application:
1. Sets up Playdate's require() compatibility shim
2. Loads the cmark module
3. Parses CommonMark test content
4. Converts to HTML
5. Displays success/failure status on screen
6. Outputs HTML to console

## Dependencies

For a full cmark implementation, you would need either:

1. **lua-cmark** - Lua bindings for libcmark
   - Requires C compilation
   - Source: https://github.com/jgm/cmark-lua

2. **Pure Lua CommonMark parser** - Alternative implementations
   - No C compilation needed
   - Various implementations available

## Build Requirements

- Playdate SDK 3.0.1 or later
- For full cmark: ARM toolchain for C compilation

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
cd playdate_cmark_test
$PLAYDATE_SDK_PATH/bin/pdc Source cmark.pdx
```

This produces:
- `cmark.pdx/` - Complete Playdate executable bundle

## Running

### In Simulator

```bash
# Open with simulator (GUI)
$PLAYDATE_SDK_PATH/bin/PlaydateSimulator cmark.pdx
```

Or use File -> Open in the Playdate Simulator application.

### On Device

1. Connect your Playdate via USB
2. Open Playdate Simulator
3. Device -> Upload Game to Device
4. Select `cmark.pdx`

## Test CommonMark

The test application parses this CommonMark:

```markdown
# Hello Playdate!

This is a **CommonMark** test with:

- Item 1
- Item 2
- Item 3

## Features

Here's some *italic* text and **bold** text.

A [link](https://play.date) to Playdate's website.

### Code

```lua
local x = 42
print(x)
```

> This is a blockquote.
> It can span multiple lines.
```

## Expected Output

When successful, you'll see:
- On screen: "cmark: SUCCESS" and "Parsed CommonMark to HTML"
- In console: The complete HTML output from the CommonMark parser

Example HTML output:
```html
<h1>Hello Playdate!</h1>
<p>This is a <strong>CommonMark</strong> test with:</p>
<ul>
<li>Item 1</li>
<li>Item 2</li>
<li>Item 3</li>
</ul>
...
```

## Implementation Details

### Integration Approach

1. **require() Compatibility Shim**:
   - Playdate uses `import` instead of standard Lua's `require`
   - Implemented custom `require()` function using `playdate.file.run()`
   - Set up `package.loaded` cache to prevent reloading modules

2. **Module Structure**:
   - cmark.lua provides the main entry point
   - API follows lua-cmark conventions

### Comparison with Lunamark

| Feature | cmark | Lunamark |
|---------|-------|----------|
| Spec | CommonMark (strict) | Markdown (flexible) |
| Implementation | C library (or stub) | Pure Lua |
| Performance | Fast (C) | Moderate (Lua/LPeg) |
| AST | Full AST support | Writer-based |
| Dependencies | None (C) or minimal | lpeg, cosmo |

## Using Full lua-cmark

To use the actual lua-cmark library:

1. Get lua-cmark source from https://github.com/jgm/cmark-lua
2. Compile libcmark for ARM (similar to HTML Tidy approach)
3. Create Lua bindings for Playdate
4. Replace the stub cmark.lua with the actual bindings

## Next Steps

- Replace stub with actual cmark Lua bindings
- Add full CommonMark spec compliance testing
- Implement additional render targets (LaTeX, etc.)
- Performance benchmarking

## License

- Playdate SDK: See SDK_LICENSE in Playdate SDK
- cmark: BSD 2-Clause License (see cmark repository)
- This integration code: Public domain
