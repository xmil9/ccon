//
// Win32 utilities library
// Timer utilities.
//
// Jun-2019, Michael Lindner
// MIT license
//
#ifdef _WIN32
#include "timer.h"
#include "tstring.h"
#include <cassert>
#include <mutex>
#include <unordered_map>


namespace
{

using namespace win32;


///////////////////

// Keeps track of all TimedCallback objects.
// Thread-safe.
class TimedCallbackRegistry
{
 public:
   static void registerTimer(UINT_PTR id, TimedCallback* timer);
   static void unregisterTimer(UINT_PTR id);
   static TimedCallback* getTimer(UINT_PTR id);

 private:
   static std::unordered_map<UINT_PTR, TimedCallback*> m_timers;
   static std::mutex m_guard;
};


std::unordered_map<UINT_PTR, TimedCallback*> TimedCallbackRegistry::m_timers;
std::mutex TimedCallbackRegistry::m_guard;


void TimedCallbackRegistry::registerTimer(UINT_PTR id, TimedCallback* timer)
{
   std::lock_guard<std::mutex> lock(m_guard);
   m_timers[id] = timer;
}

void TimedCallbackRegistry::unregisterTimer(UINT_PTR id)
{
   std::lock_guard<std::mutex> lock(m_guard);
   m_timers.erase(id);
}

TimedCallback* TimedCallbackRegistry::getTimer(UINT_PTR id)
{
   std::lock_guard<std::mutex> lock(m_guard);
   auto pos = m_timers.find(id);
   if (pos != m_timers.end())
      return pos->second;
   return nullptr;
}

} // namespace


namespace win32
{

///////////////////

Timer& Timer::operator=(Timer&& other) noexcept
{
   m_hwnd = other.m_hwnd;
   m_id = other.m_id;
   // Make sure dtor of moved-from object does nothing.
   other.m_hwnd = NULL;
   other.m_id = 0;
   return *this;
}


bool Timer::start(unsigned int timeOutMs)
{
   if (isValid())
      return (::SetTimer(m_hwnd, m_id, timeOutMs, NULL) != 0);
   return false;
}


bool Timer::stop()
{
   if (isValid())
      return ::KillTimer(m_hwnd, m_id);
   return false;
}


///////////////////

TimedCallback::~TimedCallback()
{
   stop();
}


TimedCallback::TimedCallback(TimedCallback&& other) noexcept
{
   swap(*this, other);
}


TimedCallback& TimedCallback::operator=(TimedCallback&& other) noexcept
{
   stop();

   m_callback = other.m_callback;
   m_id = other.m_id;
   // Make sure dtor of moved-from timer does nothing.
   other.m_callback = {};
   other.m_id = 0;
   return *this;
}


bool TimedCallback::start(unsigned int timeOutMs)
{
   if (!m_callback)
      return false;

   const UINT_PTR newId =
      ::SetTimer(NULL, m_id, timeOutMs, reinterpret_cast<TIMERPROC>(timerProc));
   setId(newId);

   return (newId != 0);
}


void TimedCallback::stop()
{
   if (m_id != 0)
   {
      ::KillTimer(NULL, m_id);
      setId(0);
   }
}


void TimedCallback::setId(UINT_PTR id)
{
   if (m_id != id)
   {
      if (m_id != 0)
         TimedCallbackRegistry::unregisterTimer(m_id);
      if (id != 0)
         TimedCallbackRegistry::registerTimer(id, this);
      m_id = id;
   }
}


void TimedCallback::timerProc(HWND hwnd, UINT msgId, UINT timerId, DWORD sysTime)
{
   assert(msgId == WM_TIMER);
   if (msgId != WM_TIMER)
      return;

   TimedCallback* timer = TimedCallbackRegistry::getTimer(timerId);
   if (timer)
      timer->onTimerElapsed(sysTime);
}


void TimedCallback::onTimerElapsed(DWORD sysTime)
{
   if (m_callback)
      m_callback(sysTime);
}

} // namespace win32

#endif //_WIN32
