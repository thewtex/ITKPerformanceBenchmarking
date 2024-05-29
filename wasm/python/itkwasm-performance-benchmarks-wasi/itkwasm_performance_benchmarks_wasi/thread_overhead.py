# Generated file. To retain edits, remove this comment.

from pathlib import Path, PurePosixPath
import os
from typing import Dict, Tuple, Optional, List, Any

from importlib_resources import files as file_resources

_pipeline = None

from itkwasm import (
    InterfaceTypes,
    PipelineOutput,
    PipelineInput,
    Pipeline,
)

def thread_overhead(
    no_print_stdout: bool = False,
    expanded_report: bool = False,
    no_print_system_info: bool = False,
    no_print_report_head: bool = False,
    iterations: int = 500,
    threads: int = 1,
) -> Any:
    """Estimate the overhead cost per-thread.

    :param no_print_stdout: Do not print to stdout.
    :type  no_print_stdout: bool

    :param expanded_report: Print an expanded report.
    :type  expanded_report: bool

    :param no_print_system_info: Do not print system information.
    :type  no_print_system_info: bool

    :param no_print_report_head: Do not print the report header.
    :type  no_print_report_head: bool

    :param iterations: Number of iterations to run.
    :type  iterations: int

    :param threads: Number of threads to use.
    :type  threads: int

    :return: Internal timings
    :rtype:  Any
    """
    global _pipeline
    if _pipeline is None:
        _pipeline = Pipeline(file_resources('itkwasm_performance_benchmarks_wasi').joinpath(Path('wasm_modules') / Path('thread-overhead.wasi.wasm')))

    pipeline_outputs: List[PipelineOutput] = [
        PipelineOutput(InterfaceTypes.JsonCompatible),
    ]

    pipeline_inputs: List[PipelineInput] = [
    ]

    args: List[str] = ['--memory-io',]
    # Inputs
    # Outputs
    timings_name = '0'
    args.append(timings_name)

    # Options
    input_count = len(pipeline_inputs)
    if no_print_stdout:
        args.append('--no-print-stdout')

    if expanded_report:
        args.append('--expanded-report')

    if no_print_system_info:
        args.append('--no-print-system-info')

    if no_print_report_head:
        args.append('--no-print-report-head')

    if iterations:
        args.append('--iterations')
        args.append(str(iterations))

    if threads:
        args.append('--threads')
        args.append(str(threads))


    outputs = _pipeline.run(args, pipeline_outputs, pipeline_inputs)

    result = outputs[0].data
    return result

