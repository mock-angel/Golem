/*
 * SharedRef.h
 *
 *  Created on: 22-Nov-2020
 *      Author: anantha
 */

#ifndef CORE_SHAREDREF_H_
#define CORE_SHAREDREF_H_

#include <memory>

namespace Golem {

template<class T>
class SharedRef {
public:
    SharedRef(std::weak_ptr<T> t_weakPtr){
        m_sharedPtr = t_weakPtr;
        m_pointer = t_weakPtr.lock().get();
    }
    SharedRef(std::shared_ptr<T> t_sharedPtr){
        m_sharedPtr = t_sharedPtr;
        m_pointer = t_sharedPtr.get();
    }

    ~SharedRef(){}

    T& operator=(const T& b) {

        return this;
    }

    constexpr SharedRef() noexcept = default;
    SharedRef(const SharedRef&) noexcept = default;
    SharedRef(SharedRef&&) noexcept = default;
//    SharedRef(nullptr_t) noexcept;

    SharedRef&
          operator=(const SharedRef& __r) noexcept = default;

    SharedRef&
          operator=(const std::weak_ptr<T>& __r) noexcept{
        m_sharedPtr = __r.lock();
        m_pointer = __r.lock().get();
        return *this;
    }

    SharedRef&
          operator=(const std::shared_ptr<T>& __r) noexcept
    {
        m_sharedPtr = __r;
        m_pointer = __r.get();
        return *this;
    }
//    SharedRef&
//          operator=(nullptr_t)
//    {
//        m_sharedPtr = nullptr;
//        m_pointer = nullptr;
//        return *this;
//    }

private:
    T* m_pointer = nullptr;
    std::shared_ptr<T> m_sharedPtr;
};

} /* namespace Golem */

#endif /* CORE_SHAREDREF_H_ */
