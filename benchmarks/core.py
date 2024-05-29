
class CoreSuite:
    """
    Benchmarks for ITK's Core group modules.
    """

    def setup(self):
        pass

    def time_thread_overhead(self):
        from itkwasm_performance_benchmarks import thread_overhead

        thread_overhead(threads=10, no_print_stdout=True)