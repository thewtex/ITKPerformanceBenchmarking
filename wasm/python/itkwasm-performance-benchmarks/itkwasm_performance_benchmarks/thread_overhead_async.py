# Generated file. Do not edit.

import os
from typing import Dict, Tuple, Optional, List, Any

from itkwasm import (
    environment_dispatch,
)

async def thread_overhead_async(
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
    func = environment_dispatch("itkwasm_performance_benchmarks", "thread_overhead_async")
    output = await func(no_print_stdout=no_print_stdout, expanded_report=expanded_report, no_print_system_info=no_print_system_info, no_print_report_head=no_print_report_head, iterations=iterations, threads=threads)
    return output
