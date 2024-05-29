# itkwasm-performance-benchmarks-emscripten

[![PyPI version](https://badge.fury.io/py/itkwasm-performance-benchmarks-emscripten.svg)](https://badge.fury.io/py/itkwasm-performance-benchmarks-emscripten)

Benchmark ITK performance. Emscripten implementation.

This package provides the Emscripten WebAssembly implementation. It is usually not called directly. Please use the [`itkwasm-performance-benchmarks`](https://pypi.org/project/itkwasm-performance-benchmarks/) instead.


## Installation

```sh
import micropip
await micropip.install('itkwasm-performance-benchmarks-emscripten')
```

## Development

```sh
pip install hatch
hatch run download-pyodide
hatch run test
```
