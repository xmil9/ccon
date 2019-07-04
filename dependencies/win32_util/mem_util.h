//
// Win32 utilities library
// Memory utilities.
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once
#ifdef _WIN32
#include <objbase.h>
#include <algorithm>


namespace win32
{
///////////////////

// Base class for memory RAII classes.
// Uses CRTP to call functionality in derived classes.
template <typename T, typename Derived> class Mem
{
public:
   Mem() = default;
   explicit Mem(T* ptr);
   ~Mem();
   Mem(const Mem&) = delete;
   Mem(Mem&& other) noexcept;
   Mem& operator=(const Mem&) = delete;
   Mem& operator=(Mem&& other) noexcept;

   explicit operator bool() const;
   template <typename T, typename Derived>
   friend void swap(Mem<T, Derived>& a, Mem<T, Derived>& b) noexcept;

   T* ptr();
   const T* ptr() const;
   // To support allocating directly into the member.
   T** addr();

 private:
    // Access derived class that provides custom funtionality.
    static Derived& derived(Mem& base);

 protected:
   T* m_ptr = nullptr;
};


template <typename T, typename Derived> Mem<T, Derived>::Mem(T* ptr) : m_ptr{ptr}
{
}

template <typename T, typename Derived> Mem<T, Derived>::~Mem()
{
   derived(*this).free();
}

template <typename T, typename Derived> Mem<T, Derived>::Mem(Mem&& other) noexcept
{
   swap(*this, other);
}

template <typename T, typename Derived>
Mem<T, Derived>& Mem<T, Derived>::operator=(Mem&& other) noexcept
{
   derived(*this).free();
   m_ptr = other.m_ptr;
   derived(other).clear();
   return *this;
}

template <typename T, typename Derived> Mem<T, Derived>::operator bool() const
{
   return (m_ptr != nullptr);
}

template <typename T, typename Derived>
void swap(Mem<T, Derived>& a, Mem<T, Derived>& b) noexcept
{
   std::swap(a.m_ptr, b.m_ptr);
}

template <typename T, typename Derived> T* Mem<T, Derived>::ptr()
{
   return m_ptr;
}

template <typename T, typename Derived> const T* Mem<T, Derived>::ptr() const
{
   return m_ptr;
}

template <typename T, typename Derived> T** Mem<T, Derived>::addr()
{
   return &m_ptr;
}

template <typename T, typename Derived> Derived& Mem<T, Derived>::derived(Mem& base)
{
   return static_cast<Derived&>(base);
}


///////////////////

// RAII class for memory allocated by CoTaskMemAlloc or CoTaskMemRealloc.
template <typename T> class CoTaskMem : public Mem<T, CoTaskMem<T>>
{
 public:
   using Base_t = Mem<T, CoTaskMem<T>>;
   // Inherit ctors.
   using Base_t::Base_t;

   template <typename T> friend void swap(CoTaskMem<T>& a, CoTaskMem<T>& b) noexcept;
   void clear();
   void free();
};


template <typename T> void swap(CoTaskMem<T>& a, CoTaskMem<T>& b) noexcept
{
   using Base_t = Mem<T, CoTaskMem<T>>;
   std::swap(static_cast<Base_t&>(a), static_cast<Base_t&>(b));
}

template <typename T> void CoTaskMem<T>::clear()
{
   this->m_ptr = nullptr;
}

template <typename T> void CoTaskMem<T>::free()
{
   if (this->m_ptr)
   {
      CoTaskMemFree(this->m_ptr);
      clear();
   }
}


///////////////////

// RAII class for memory allocated by GlobalAlloc or GlobalReAlloc.
template <typename T> class GlobalMem : public Mem<T, GlobalMem<T>>
{
 public:
   using Base_t = Mem<T, GlobalMem<T>>;
   // Inherit ctors.
   using Base_t::Base_t;

   template <typename T> friend void swap(GlobalMem<T>& a, GlobalMem<T>& b) noexcept;
   void clear();
   void free();
};


template <typename T> void swap(GlobalMem<T>& a, GlobalMem<T>& b) noexcept
{
   using Base_t = Mem<T, GlobalMem<T>>;
   std::swap(static_cast<Base_t&>(a), static_cast<Base_t&>(b));
}

template <typename T> void GlobalMem<T>::clear()
{
   this->m_ptr = nullptr;
}

template <typename T> void GlobalMem<T>::free()
{
   if (this->m_ptr)
   {
      GlobalFree(this->m_ptr);
      clear();
   }
}


///////////////////

// RAII class for memory allocated by LocalAlloc or LocalReAlloc.
template <typename T> class LocalMem : public Mem<T, LocalMem<T>>
{
 public:
   using Base_t = Mem<T, LocalMem<T>>;
   // Inherit ctors.
   using Base_t::Base_t;

   template <typename T> friend void swap(LocalMem<T>& a, LocalMem<T>& b) noexcept;
   void clear();
   void free();
};


template <typename T> void swap(LocalMem<T>& a, LocalMem<T>& b) noexcept
{
   using Base_t = Mem<T, LocalMem<T>>;
   std::swap(static_cast<Base_t&>(a), static_cast<Base_t&>(b));
}

template <typename T> void LocalMem<T>::clear()
{
   this->m_ptr = nullptr;
}

template <typename T> void LocalMem<T>::free()
{
   if (this->m_ptr)
   {
      LocalFree(this->m_ptr);
      clear();
   }
}


///////////////////

// RAII class for memory allocated by HeapAlloc or HeapReAlloc.
template <typename T> class HeapMem : public Mem<T, HeapMem<T>>
{
 public:
   using Base_t = Mem<T, HeapMem<T>>;
 
   HeapMem() = default;
   explicit HeapMem(T* ptr);
   HeapMem(HANDLE heap, DWORD flags, T* ptr = nullptr);
   ~HeapMem() = default;
   HeapMem(const HeapMem&) = delete;
   HeapMem(HeapMem&& other) noexcept;
   HeapMem& operator=(const HeapMem&) = delete;
   HeapMem& operator=(HeapMem&& other) noexcept;

   template <typename T> friend void swap(HeapMem<T>& a, HeapMem<T>& b) noexcept;
   void clear();
   void free();

 private:
   HANDLE m_heap = NULL;
   DWORD m_flags = 0;
};


template <typename T> HeapMem<T>::HeapMem(T* ptr) : HeapMem{GetProcessHeap(), 0, ptr}
{
}

template <typename T>
HeapMem<T>::HeapMem(HANDLE heap, DWORD flags, T* ptr)
: Base_t{ptr}, m_heap{heap}, m_flags{flags}
{
}

template <typename T> HeapMem<T>::HeapMem(HeapMem&& other) noexcept
   : Base_t{std::move(other)}, m_heap{other.m_heap}, m_flags{other.m_flags}
{
   other.clear();
}

template <typename T> HeapMem<T>& HeapMem<T>::operator=(HeapMem&& other) noexcept
{
   free();
   this->m_ptr = other.m_ptr;
   m_heap = other.m_heap;
   m_flags = other.m_flags;
   other.clear();
   return *this;
}

template <typename T> void swap(HeapMem<T>& a, HeapMem<T>& b) noexcept
{
   using Base_t = Mem<T, HeapMem<T>>;
   swap(static_cast<Base_t&>(a), static_cast<Base_t&>(b));
   std::swap(a.m_heap, b.m_heap);
   std::swap(a.m_flags, b.m_flags);
}

template <typename T> void HeapMem<T>::clear()
{
   this->m_ptr = nullptr;
   m_heap = NULL;
   m_flags = 0;
}

template <typename T> void HeapMem<T>::free()
{
   if (this->m_ptr)
   {
      HeapFree(m_heap, m_flags, this->m_ptr);
      clear();
   }
}

} // namespace win32

#endif //_WIN32
