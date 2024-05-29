from itkwasm_performance_benchmarks import thread_overhead

def test_thread_overhead(benchmark):
    result = benchmark(thread_overhead, threads=2, no_print_stdout=True)
    assert 'ITKBuildInformation' in result