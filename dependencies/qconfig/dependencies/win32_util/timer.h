//
// Win32 utilities library
// Timer utilities.
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#ifdef _WIN32
#include "win32_util_api.h"
#include "win32_windows.h"
#include <functional>
#include <utility>


namespace win32
{

// RAII class for timers that are associated with a window.
// The dtor will kill the timer.
class WIN32UTIL_API Timer
{
 public:
   Timer() = default;
   Timer(HWND hwnd, UINT_PTR id);
   ~Timer();
   Timer(const Timer&) = delete;
   Timer(Timer&& other) noexcept;
   Timer& operator=(const Timer&) = delete;
   Timer& operator=(Timer&& other) noexcept;

   HWND hwnd() const;
   UINT_PTR id() const;
   explicit operator bool() const;
   friend void swap(Timer& a, Timer& b) noexcept;

   bool start(unsigned int timeOutMs);
   bool stop();

 private:
   bool isValid() const;

 private:
   HWND m_hwnd = NULL;
   UINT_PTR m_id = 0;
};


inline Timer::Timer(HWND hwnd, UINT_PTR id) : m_hwnd{hwnd}, m_id{id}
{
}

inline Timer::~Timer()
{
   stop();
}

inline Timer::Timer(Timer&& other) noexcept
{
   swap(*this, other);
}

inline HWND Timer::hwnd() const
{
   return m_hwnd;
}

inline UINT_PTR Timer::id() const
{
   return m_id;
}

inline Timer::operator bool() const
{
   return isValid();
}

inline void swap(Timer& a, Timer& b) noexcept
{
   std::swap(a.m_hwnd, b.m_hwnd);
   std::swap(a.m_id, b.m_id);
}

inline bool Timer::isValid() const
{
   return (m_hwnd != NULL && m_id != 0);
}


///////////////////

// Calls a given function after a time-out interval.
// Needs message loop to work even though it is not attached to a window.
class WIN32UTIL_API TimedCallback
{
 public:
   // Type of the callback function. Is passed the system time
   // when the timer expired.
   using Callback_t = std::function<void(DWORD)>;

 public:
   TimedCallback() = default;
   explicit TimedCallback(Callback_t callback);
   ~TimedCallback();
   TimedCallback(const TimedCallback&) = delete;
   TimedCallback(TimedCallback&& other) noexcept;
   TimedCallback& operator=(const TimedCallback&) = delete;
   TimedCallback& operator=(TimedCallback&& other) noexcept;

   explicit operator bool() const;
   friend void swap(TimedCallback& a, TimedCallback& b) noexcept;

   bool start(unsigned int timeOutMs);
   void stop();

   // Only useful for testing.
   UINT_PTR id() const;

 private:
   void setId(UINT_PTR id);

   static void timerProc(HWND hwnd, UINT msgId, UINT timerId, DWORD sysTime);
   void onTimerElapsed(DWORD sysTime);

 private:
   Callback_t m_callback;
   UINT_PTR m_id = 0;
};


inline TimedCallback::TimedCallback(Callback_t callback) : m_callback{callback}
{
}

inline TimedCallback::operator bool() const
{
   return m_callback.operator bool();
}

inline void swap(TimedCallback& a, TimedCallback& b) noexcept
{
   std::swap(a.m_callback, b.m_callback);
   std::swap(a.m_id, b.m_id);
}

inline UINT_PTR TimedCallback::id() const
{
   return m_id;
}

} // namespace win32

#endif //_WIN32
