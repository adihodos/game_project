#pragma once

#include <Windows.h>
#include <MMSystem.h>

namespace base {

template<typename Real>
class TimerPolicyWMMTimer {
public :
  TimerPolicyWMMTimer() {}

  void Reset() {
    ref_time_ = ::timeGetTime();
  }
  
  Real DeltaNowMs() const {
    return static_cast<Real>(::timeGetTime() - ref_time_);
  }

  Real DeltaNowMsWithUpdate() {
    DWORD time_now = ::timeGetTime();
    Real time_delta = static_cast<Real>(time_now - ref_time_);
    ref_time_ = time_now;
    return time_delta;
  }

protected :
  ~TimerPolicyWMMTimer() {}

private :
  //
  // Safe to copy assign / construct
  DWORD   ref_time_;
};

template<typename Real>
class TimerPolicyQPerfCounter {
public :
  TimerPolicyQPerfCounter() {
    ::QueryPerformanceFrequency(&perf_frequency_);
  }
  
  void Reset() {
    ::QueryPerformanceCounter(&ref_time_);
  }
  
  Real DeltaNowMs() const {
    LARGE_INTEGER tm_now;
    ::QueryPerformaceFrequency(&tm_now);
    return static_cast<Real>(((tm_now.QuadPart - ref_time_.QuadPart) * 1000)
                             / perf_frequency_.QuadPart);  
  }

  Real DeltaNowMsWithUpdate() {
    LARGE_INTEGER time_now;
    ::QueryPerformanceCounter(&time_now);
    Real result = static_cast<Real>(
      (time_now.QuadPart - ref_time_.QuadPart) * 1000 / perf_frequency_.QuadPart);
    ref_time_ = time_now;
    return result;
  }

protected :
  ~TimerPolicyQPerfCounter() {}

private :
  //
  // Safe to copy assign construct
  LARGE_INTEGER ref_time_;
  LARGE_INTEGER perf_frequency_;
};

template<typename Real, 
         template<typename> class TimerPolicy = TimerPolicyWMMTimer
> class Timer : public TimerPolicy<Real> {
public :
  typedef Real    TimeType;
};

typedef Timer<float, TimerPolicyWMMTimer>   WMMSimplePrecisionTimer;
typedef Timer<double, TimerPolicyWMMTimer>  WMMExtendedPrecisionTimer;

typedef Timer<float, TimerPolicyQPerfCounter> HRTSimplePrecisionTimer;
typedef Timer<double, TimerPolicyQPerfCounter> HRTExtendedPrecisionTimer;

} // namespace base