#pragma once

#include <chrono>

namespace perftest
{
  class Timer
  {
  public:
    void Start()
    {
      // Timestamp
      start = std::chrono::high_resolution_clock::now();
    }

    void Stop()
    {
      // Timestamps
      stop = std::chrono::high_resolution_clock::now();
      poststop = std::chrono::high_resolution_clock::now();

      // Calculate duration
      std::chrono::duration<double> span =
        std::chrono::duration_cast<std::chrono::duration<double>>(stop - start - (poststop - stop));

      // Store
      lastspan = span.count();
      spans.push_back(lastspan);
    }

    double GetLastDurationSec()
    {
      return lastspan;
    }

    double GetLastDurationUs()
    {
      return GetLastDurationSec() * 1000000.0;
    }

    double GetMedianDurationSec()
    {
      double median = 0;
      if (spans.size() > 0)
      {
        sort(spans.begin(), spans.end());
        median = spans.at(spans.size() / 2);
      }

      return median;
    }

    double GetMedianDurationUs()
    {
      return GetMedianDurationSec() * 1000000.0;
    }

    void ReportMedian()
    {
      std::cout << "Test median duration                           Elapsed   ";
      std::cout << static_cast<long long>(round(GetMedianDurationUs())) << " us\n";
    }

  private:
    std::chrono::high_resolution_clock::time_point start;
    std::chrono::high_resolution_clock::time_point stop;
    std::chrono::high_resolution_clock::time_point poststop;
    double lastspan;
    std::vector<double> spans;
  };
}
