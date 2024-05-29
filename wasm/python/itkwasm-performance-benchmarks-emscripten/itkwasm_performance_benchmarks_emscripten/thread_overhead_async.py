# Generated file. To retain edits, remove this comment.

from pathlib import Path
import os
from typing import Dict, Tuple, Optional, List, Any

from .js_package import js_package

from itkwasm.pyodide import (
    to_js,
    to_py,
    js_resources
)
from itkwasm import (
    InterfaceTypes,
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
    js_module = await js_package.js_module
    web_worker = js_resources.web_worker

    kwargs = {}
    if no_print_stdout:
        kwargs["noPrintStdout"] = to_js(no_print_stdout)
    if expanded_report:
        kwargs["expandedReport"] = to_js(expanded_report)
    if no_print_system_info:
        kwargs["noPrintSystemInfo"] = to_js(no_print_system_info)
    if no_print_report_head:
        kwargs["noPrintReportHead"] = to_js(no_print_report_head)
    if iterations:
        kwargs["iterations"] = to_js(iterations)
    if threads:
        kwargs["threads"] = to_js(threads)

    outputs = await js_module.threadOverhead(webWorker=web_worker, noCopy=True, **kwargs)

    output_web_worker = None
    output_list = []
    outputs_object_map = outputs.as_object_map()
    for output_name in outputs.object_keys():
        if output_name == 'webWorker':
            output_web_worker = outputs_object_map[output_name]
        else:
            output_list.append(to_py(outputs_object_map[output_name]))

    js_resources.web_worker = output_web_worker

    if len(output_list) == 1:
        return output_list[0]
    return tuple(output_list)
