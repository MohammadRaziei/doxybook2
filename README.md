# doxybook2

Python bindings and CLI wrapper for the Doxybook2 toolkit. The package builds the bundled C++ sources from `src/third_party/doxybook2` and exposes a small Cython bridge to generate documentation directly from Python.

## Installation

Build locally with the bundled dependencies:

```bash
pip install . --no-build-isolation
```

## Usage

Generate documentation via Python:

```python
from doxybook2 import build_docs

build_docs(
    input_dir="./doxygen/xml",
    output_dir="./docs",
    config_path="./doxybook.json",
)
```

Or use the CLI entrypoint:

```bash
doxybook2 --input ./doxygen/xml --output ./docs --config ./doxybook.json
```

Use `--json` to emit JSON only, `--templates` to override templates, `--quiet` to silence logs, and `--debug-templates` to dump template debug JSON.

## Development

Tests are not provided yet. Build artifacts rely on CMake and Cython; you can trigger a rebuild with `pip install . --no-build-isolation`.

