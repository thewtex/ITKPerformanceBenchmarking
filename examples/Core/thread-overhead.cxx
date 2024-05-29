/*=========================================================================
 *
 *  Copyright NumFOCUS
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         https://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#include "itkImage.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkTimeProbesCollectorBase.h"
#include "itkHighPriorityRealTimeProbe.h"
#include "itkHighPriorityRealTimeProbesCollector.h"

#if defined(PERFORMANCE_BENCHMARKING_USE_ITKWASM)
#  include "itkPipeline.h"
#  include "itkOutputTextStream.h"
#endif

#include <sstream>
#include <fstream>
#include "PerformanceBenchmarkingUtilities.h"

// This benchmark estimate the overhead for using an additional thread
// in an ImageFilter a.k.a the time it takes to "spawn" a thread.
//
// The overhead for spawning threads is computed by measuring the
// time it takes an Functor Filter to run with 1 thread on 1 pixel,
// and the time it takes to run with N threads on N pixels. Each
// thread does the one pixel trivial operation. The difference in
// execution time is considered the overhead for spawning the
// threads. Dividing by the number of additional threads gives us the
// overhead cost of “spawning” or dispatching a single thread.


using ProbeType = itk::HighPriorityRealTimeProbe;
using CollectorType = itk::HighPriorityRealTimeProbesCollector;

// using CollectorType = itk::TimeProbesCollectorBase;
// using ProbeType = itk::TimeProbe;


namespace
{

CollectorType collector;

template <typename TInput, typename TOutput>
class Op
{
public:
  Op() = default;
  virtual ~Op() = default;
  bool
  operator!=(const Op &) const
  {
    return false;
  }

  bool
  operator==(const Op & other) const
  {
    return !(*this != other);
  }

  inline TOutput
  operator()(const TInput & A) const
  {
    return static_cast<TOutput>(A + 1);
  }
};
} // namespace

static ProbeType
time_it(unsigned int threads, unsigned int iterations)
{

  constexpr unsigned int Dimension = 1;
  using PixelType = float;

  using ImageType = itk::Image<PixelType, Dimension>;

  ImageType::Pointer image = ImageType::New();

  ImageType::SizeType imageSize = { threads };
  image->SetRegions(ImageType::RegionType(imageSize));
  image->Allocate();
  image->FillBuffer(0);


  using FilterType = itk::UnaryFunctorImageFilter<ImageType, ImageType, Op<PixelType, PixelType>>;

  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(image);
  filter->SET_PARALLEL_UNITS(threads);

  // execute one time out of the loop to allocate memory
  filter->UpdateLargestPossibleRegion();

  std::ostringstream ss;
  ss << "FilterWithThreads-" << threads;

  const std::string name = ss.str();

  for (unsigned ii = 0; ii < iterations; ++ii)
  {
    image->Modified();
    collector.Start(name.c_str());
    filter->UpdateLargestPossibleRegion();
    collector.Stop(name.c_str());
  }

  return collector.GetProbe(name.c_str());
}


int
main(int argc, char * argv[])
{
  bool          noPrintStdout = false;
  bool          expandedReport = false;
  bool          noPrintSystemInfo = false;
  bool          noPrintReportHead = false;
  constexpr int iterationsDefault = 500;
  const int     threadsDefault = MultiThreaderName::GetGlobalDefaultNumberOfThreads();

#if defined(PERFORMANCE_BENCHMARKING_USE_ITKWASM)
  itk::wasm::Pipeline pipeline("thread-overhead", "Estimate the overhead cost per-thread.", argc, argv);

  itk::wasm::OutputTextStream timingsJson;
  pipeline.add_option("timings", timingsJson, "Internal timings")->type_name("OUTPUT_JSON");

  pipeline.add_flag("--no-print-stdout", noPrintStdout, "Do not print to stdout.");
  pipeline.add_flag("--expanded-report", expandedReport, "Print an expanded report.");
  pipeline.add_flag("--no-print-system-info", noPrintSystemInfo, "Do not print system information.");
  pipeline.add_flag("--no-print-report-head", noPrintReportHead, "Do not print the report header.");

  int iterations = iterationsDefault;
  pipeline.add_option("--iterations", iterations, "Number of iterations to run.");

  int threads = threadsDefault;
  pipeline.add_option("--threads", threads, "Number of threads to use.");

  ITK_WASM_PARSE(pipeline);

  std::ostream & timingsStream = timingsJson.Get();
#else
  if (argc > 5)
  {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " timingsFile [--iterations iterations [--threads threads]]" << std::endl;
    return EXIT_FAILURE;
  }

  const std::string timingsFileName = ReplaceOccurrence(argv[1], "__DATESTAMP__", PerfDateStamp());
  auto              timingsStream = std::ofstream(timingsFileName, std::ios_base::out);
  const int         iterations = (argc > 3) ? std::stoi(argv[3]) : iterationsDefault;
  const int         threads = (argc > 5) ? std::stoi(argv[5]) : threadsDefault;
#endif

  if (threads == 1)
  {
    std::cout << "Unable to estimate the cost with only one thread!" << std::endl;
    return EXIT_FAILURE;
  }

  ProbeType t1 = time_it(1, iterations);
  ProbeType t2 = time_it(threads, iterations);

  WriteExpandedReport(
    timingsStream, collector, !noPrintStdout, expandedReport, !noPrintSystemInfo, !noPrintReportHead, false);

  double cost = (t2.GetMinimum() - t1.GetMinimum()) / (threads - 1.0);

  if (!noPrintStdout)
  {
    std::cout << "\n\nEstimated overhead cost per thread: " << cost * 1e6 << " micro-seconds\n\n";
  }

  return EXIT_SUCCESS;
}
