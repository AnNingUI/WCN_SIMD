# IDE Setup Guide for WCN_SIMD

This guide helps you configure your IDE/editor for optimal development experience with WCN_SIMD.

> **📍 Current Version**: Phase 1.2 Complete - Unified cross-platform API with 94+ operations
>
> With full IDE support, you get IntelliSense for all 94+ unified `wcn_simd_*` operations that work identically across all platforms!

## Table of Contents

- [Prerequisites](#prerequisites)
- [Visual Studio Code](#visual-studio-code)
- [CLion](#clion)
- [Vim/Neovim](#vimneovim)
- [Emacs](#emacs)
- [Other Editors](#other-editors)

## Prerequisites

### Required: compile_commands.json

WCN_SIMD automatically generates `compile_commands.json` during build, which is required by most modern IDEs for accurate code intelligence.

**To generate it:**
```bash
# Build the project (compile_commands.json is auto-generated)
./build.sh          # Linux/macOS
build.bat           # Windows

# The file will be copied to the root directory automatically
```

**Verify it exists:**
```bash
ls compile_commands.json  # Should exist in project root
```

### Recommended: Install clangd

[clangd](https://clangd.llvm.org/) provides the best C/C++ language server experience.

**Installation:**
```bash
# Windows (with MSYS2)
pacman -S mingw-w64-ucrt-x86_64-clang-tools-extra

# Windows (with Chocolatey)
choco install llvm

# Linux (Debian/Ubuntu)
sudo apt install clangd

# Linux (Arch)
sudo pacman -S clang

# macOS (Homebrew)
brew install llvm
```

## Visual Studio Code

### Method 1: Using clangd (Recommended)

**Step 1: Install Extensions**
- Install [clangd extension](https://marketplace.visualstudio.com/items?itemName=llvm-vs-code-extensions.vscode-clangd)

**Step 2: Configure**

Copy example configs:
```bash
cp .vscode/settings.json.example .vscode/settings.json
cp .vscode/c_cpp_properties.json.example .vscode/c_cpp_properties.json
cp .vscode/tasks.json.example .vscode/tasks.json
```

Or create `.vscode/settings.json` manually:
```json
{
    "clangd.path": "clangd",
    "clangd.arguments": [
        "--compile-commands-dir=${workspaceFolder}",
        "--background-index"
    ],
    "C_Cpp.intelliSenseEngine": "disabled"
}
```

**Step 3: Build Project**
```bash
# Press Ctrl+Shift+B or use terminal:
./build.sh  # or build.bat on Windows
```

**Step 4: Reload Window**
- Press `Ctrl+Shift+P` → "Developer: Reload Window"

### Method 2: Using Microsoft C/C++ Extension

**Step 1: Install Extension**
- Install [C/C++ extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)

**Step 2: Configure**

Create `.vscode/c_cpp_properties.json`:
```json
{
    "version": 4,
    "configurations": [
        {
            "name": "Default",
            "includePath": [
                "${workspaceFolder}/include"
            ],
            "compileCommands": "${workspaceFolder}/compile_commands.json"
        }
    ]
}
```

### Features You Get

✅ **Auto-completion** - Context-aware code completion  
✅ **Go to definition** - Jump to function/type definitions (F12)  
✅ **Find references** - Find all usages (Shift+F12)  
✅ **Hover information** - See documentation on hover  
✅ **Error checking** - Real-time diagnostics  
✅ **Rename symbol** - Rename across entire project (F2)  

### Keyboard Shortcuts

| Action | Shortcut |
|--------|----------|
| Go to Definition | `F12` |
| Peek Definition | `Alt+F12` |
| Find References | `Shift+F12` |
| Rename Symbol | `F2` |
| Format Document | `Shift+Alt+F` |
| Build | `Ctrl+Shift+B` |
| Command Palette | `Ctrl+Shift+P` |

## CLion

CLion has built-in CMake support with excellent code intelligence.

**Step 1: Open Project**
- File → Open → Select WCN_SIMD directory
- CLion will automatically detect CMakeLists.txt

**Step 2: Configure CMake**
- Go to Settings → Build, Execution, Deployment → CMake
- Set Generator: **Ninja**
- Add CMake option: `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON`

**Step 3: Build**
- Click the Build button (Ctrl+F9)
- CLion automatically uses compile_commands.json

### Features

✅ Full CMake integration  
✅ Automatic code analysis  
✅ Refactoring tools  
✅ Integrated debugger  
✅ Code coverage  

## Vim/Neovim

### Using coc.nvim + clangd

**Step 1: Install coc.nvim**

Using vim-plug:
```vim
Plug 'neoclide/coc.nvim', {'branch': 'release'}
```

**Step 2: Install clangd extension**
```vim
:CocInstall coc-clangd
```

**Step 3: Configure coc-settings.json**
```vim
:CocConfig
```

Add:
```json
{
    "clangd.path": "clangd",
    "clangd.arguments": [
        "--compile-commands-dir=.",
        "--background-index",
        "--clang-tidy"
    ]
}
```

**Step 4: Build project**
```bash
./build.sh
```

### Using native LSP (Neovim 0.5+)

**Step 1: Install nvim-lspconfig**

Using packer.nvim:
```lua
use 'neovim/nvim-lspconfig'
use 'hrsh7th/nvim-cmp'  -- autocompletion
```

**Step 2: Configure init.lua**
```lua
local lspconfig = require('lspconfig')

lspconfig.clangd.setup{
    cmd = {
        "clangd",
        "--compile-commands-dir=.",
        "--background-index",
        "--clang-tidy"
    },
    root_dir = lspconfig.util.root_pattern(
        "compile_commands.json",
        ".git"
    )
}
```

### Vim Keybindings

```vim
" Go to definition
nnoremap <silent> gd <Plug>(coc-definition)

" Find references
nnoremap <silent> gr <Plug>(coc-references)

" Hover documentation
nnoremap <silent> K :call CocActionAsync('doHover')<CR>

" Rename symbol
nnoremap <leader>rn <Plug>(coc-rename)
```

## Emacs

### Using lsp-mode + clangd

**Step 1: Install packages**

Add to init.el:
```elisp
(use-package lsp-mode
  :hook (c-mode . lsp)
  :commands lsp)

(use-package lsp-ui
  :commands lsp-ui-mode)

(use-package company
  :config (global-company-mode))
```

**Step 2: Configure clangd**
```elisp
(setq lsp-clients-clangd-args
      '("--compile-commands-dir=."
        "--background-index"
        "--clang-tidy"))
```

**Step 3: Open a C file**
- LSP will automatically start when you open a .c or .h file

### Emacs Keybindings

| Action | Keybinding |
|--------|------------|
| Go to definition | `M-.` |
| Find references | `M-?` |
| Rename | `C-c r` |
| Format | `C-c f` |

## Other Editors

### Sublime Text

**Install LSP package:**
1. Install Package Control
2. Install "LSP" and "LSP-clangd"
3. Configure in Preferences → Package Settings → LSP → Settings

```json
{
    "clients": {
        "clangd": {
            "enabled": true,
            "command": ["clangd", "--compile-commands-dir=."]
        }
    }
}
```

### Kate/KDevelop

**Built-in LSP support:**
1. Settings → Configure Kate → LSP Client
2. Select clangd as C language server
3. Point to project root containing compile_commands.json

### QtCreator

**CMake project support:**
1. File → Open File or Project → Select CMakeLists.txt
2. QtCreator automatically detects and uses compile_commands.json
3. Set Generator to Ninja in Build Settings

## Troubleshooting

### clangd not finding headers

**Solution 1: Rebuild to regenerate compile_commands.json**
```bash
./build.sh clean
```

**Solution 2: Check .clangd config**

The project includes `.clangd` configuration. Verify it exists:
```bash
ls .clangd
```

### IntelliSense shows errors but code compiles

**Solution: Reload the window/restart LSP**

**VSCode:**
```
Ctrl+Shift+P → "Developer: Reload Window"
```

**Vim/Neovim:**
```vim
:CocRestart
```

### Performance issues with large codebase

**Solution: Limit background indexing**

Edit `.clangd`:
```yaml
Index:
  Background: Skip
```

### compile_commands.json not found

**Solution: Ensure CMAKE_EXPORT_COMPILE_COMMANDS is ON**

Check CMakeLists.txt has:
```cmake
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
```

Then rebuild:
```bash
rm -rf build
./build.sh
```

### Wrong compiler paths

**Solution: Update compile_commands.json**

Edit paths in `compile_commands.json` if needed, or set CMake compiler:
```bash
cmake -DCMAKE_C_COMPILER=/path/to/gcc ..
```

## Verifying Setup

### Test 1: Code Completion

1. Open `examples/main.c`
2. Type `wcn_simd_` and wait
3. You should see auto-completion suggestions for all 94+ unified operations:
   - `wcn_simd_load_f32()`, `wcn_simd_store_f32()`
   - `wcn_simd_add_i32()`, `wcn_simd_mul_f32()`
   - `wcn_simd_set1_i32()`, `wcn_simd_setzero_i128()`
   - And many more...

### Test 2: Go to Definition

1. Place cursor on `wcn_simd_init`
2. Press F12 (or equivalent)
3. Should jump to function definition in `include/WCN_SIMD.h`

### Test 3: Find References

1. Right-click on `wcn_v128f_t` (unified vector type)
2. Select "Find All References"
3. Should show all usages across the project, including:
   - Type definitions in `wcn_types.h`
   - Platform implementations (SSE2, NEON, LSX, etc.)
   - Usage in example code

### Test 4: Error Detection

1. Open `src/wcn_simd.c`
2. Add an intentional error (e.g., undefined variable)
3. IDE should highlight it with red squiggly underline

## Best Practices

### 1. Always Build Before Editing

```bash
# Generate fresh compile_commands.json
./build.sh
```

### 2. Use Project-Relative Includes

```c
// Good (user perspective)
#include <WCN_SIMD.h>

// Avoid (developer only)
#include "../include/WCN_SIMD.h"
```

### 3. Keep .clangd Configuration

The project includes a `.clangd` file optimized for this codebase. Don't delete it!

### 4. Regular LSP Restarts

If IntelliSense acts weird, restart the language server:
- VSCode: Reload window
- Vim: `:CocRestart` or `:LspRestart`
- Emacs: `M-x lsp-workspace-restart`

## IDE Feature Comparison

| Feature | VSCode | CLion | Vim/Neovim | Emacs | Sublime |
|---------|---------|-------|------------|-------|---------|
| Auto-completion | ✅ | ✅ | ✅ | ✅ | ✅ |
| Go to Definition | ✅ | ✅ | ✅ | ✅ | ✅ |
| Find References | ✅ | ✅ | ✅ | ✅ | ✅ |
| Rename | ✅ | ✅ | ✅ | ✅ | ⚠️ |
| Debugging | ✅ | ✅ | ✅* | ✅* | ❌ |
| CMake Integration | ⚠️ | ✅ | ⚠️ | ⚠️ | ❌ |
| Performance | ✅ | ⚠️ | ✅ | ✅ | ✅ |

*Requires additional configuration

## Getting Help

If you encounter issues:

1. Check [clangd documentation](https://clangd.llvm.org/)
2. Verify `compile_commands.json` exists and is valid
3. Check your editor's LSP logs
4. Try rebuilding: `./build.sh clean`

---

**Enjoy coding with full IDE support and 94+ unified cross-platform SIMD operations!** 🚀

*For complete API documentation, see [roadmap.md](../roadmap.md) and [QUICKSTART.md](QUICKSTART.md)*
