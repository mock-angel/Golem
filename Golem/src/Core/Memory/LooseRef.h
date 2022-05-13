/*
 * SmartRef.h
 *
 *  Created on: 19-Nov-2020
 *      Author: anantha
 */

#ifndef CORE_LOOSEREF_H_
#define CORE_LOOSEREF_H_

#include <memory>

namespace Golem {

template<class T>
class LooseRef {
public:
    LooseRef(std::weak_ptr<T> t_weakPtr){
        m_weakPtr = t_weakPtr;
        m_pointer = t_weakPtr.lock().get();
    }
    LooseRef(std::shared_ptr<T> t_sharedPtr){
        m_weakPtr = t_sharedPtr;
        m_pointer = t_sharedPtr.get();
    }

    ~LooseRef(){}

    T& operator=(const T& b) {

        return this;
    }

    constexpr LooseRef() noexcept = default;
    LooseRef(const LooseRef&) noexcept = default;
    LooseRef(LooseRef&&) noexcept = default;
    LooseRef(nullptr_t) {};

    LooseRef&
          operator=(const LooseRef& __r) noexcept{
        m_weakPtr = __r.m_weakPtr;
        m_pointer = __r.m_pointer;
    }

    LooseRef&
          operator=(const std::weak_ptr<T>& __r) noexcept{
        m_weakPtr = __r;
        m_pointer = __r.lock().get();
        return *this;
    }

    LooseRef&
          operator=(const std::shared_ptr<T>& __r) noexcept
    {
        m_weakPtr = __r;
        m_pointer = __r.get();
        return *this;
    }
//    LooseRef&
//          operator=(nullptr_t) noexcept
//    {
//        m_weakPtr = nullptr;
//        m_pointer = nullptr;
//        return *this;
//    }

    T* get(){
        return m_pointer;
    }
private:
    T* m_pointer = nullptr;
    std::weak_ptr<T> m_weakPtr;
};



} /* namespace Golem */

#endif /* CORE_LOOSEREF_H_ */
