#ifndef _SGPCET_VAR_IMPL_H_
#define _SGPCET_VAR_IMPL_H_

#ifndef _SGPCET_READ_IMPL_H_
#include "read_impl.h"
#endif
#ifndef _SGPCET_VAL_INIT_H_
#include "val_init.h"
#endif

namespace validate {
    namespace reader {
        using namespace generator;

        class _BasicVar {
        protected:
            std::string _name;
        public:
            _BasicVar(const std::string& name = _setting::_empty_var_name) : _name(name) {};
            virtual ~_BasicVar() {};
            _BasicVar(const _BasicVar& other) : _name(other._name) {};
            _BasicVar& operator=(const _BasicVar& other) {
                _name = other._name;
                return *this;
            }
            _BasicVar(_BasicVar&& other) : _name(std::move(other._name)) {};
            _BasicVar& operator=(_BasicVar&& other) {
                _name = std::move(other._name);
                return *this;
            }

            virtual void read_and_cache() = 0;
            virtual _BasicVar* clone() = 0;
            // clone and rename
            virtual _BasicVar* rename(const std::string& name) = 0;
            
            _SET_VALUE(std::string, name)
            std::string name() {
                if (_name == _setting::_empty_var_name) _name = __default_name();
                return _name;
            }
        
        protected:
            virtual std::string __default_name() {
                return _setting::_empty_var_name;
            }
        };

        template<typename T, typename Derived>
        class _VarCRTP : public _BasicVar {
        protected:
            T _value;
        public:
            using Target = T;
            _VarCRTP(const std::string& name = _setting::_empty_var_name) : _BasicVar(name) {};
            virtual ~_VarCRTP() {};
            _VarCRTP(const _VarCRTP& other) : _BasicVar(other), _value(other._value) {};
            _VarCRTP& operator=(const _VarCRTP& other) {
                _BasicVar::operator=(other);
                _value = other._value;
                return *this;
            }
            _VarCRTP(_VarCRTP&& other) : _BasicVar(std::move(other)), _value(std::move(other._value)) {};
            _VarCRTP& operator=(_VarCRTP&& other) {
                _BasicVar::operator=(std::move(other));
                _value = std::move(other._value);
                return *this;
            }

            _GET_VALUE(T, value)

            virtual void read_and_cache() override {
                _value = static_cast<Derived*>(this)->read();
                static_cast<Derived*>(this)->cache();
            }

            virtual _BasicVar* clone() override {
                return new Derived(static_cast<const Derived&>(*this));;
            }

            virtual _BasicVar* rename(const std::string& name) override {
                _BasicVar* result = clone();
                result->set_name(name);
                return result;
            }

            virtual T read() {
                _msg::__error_msg(_msg::_defl, 
                    tools::string_format("No reader for %s", name().c_str()));
                return T();
            }

            operator T() const {
                return _value;
            }
        
        protected:
            virtual std::string __default_name() override {
               _AutoName<T> name;
               return name.name();
            }

            virtual void cache() {}
        };

        template<typename T, typename Derived>
        class _Var : public _VarCRTP<T, Derived> {
        public:
            using Target = T;
            _Var(const std::string& name = _setting::_empty_var_name) : _VarCRTP<T, Derived>(name) {};
            virtual ~_Var() {};
            _Var(const _Var& other) : _VarCRTP<T, Derived>(other) {};
            _Var& operator=(const _Var& other) {
                _VarCRTP<T, Derived>::operator=(other);
                return *this;
            }
            _Var(_Var&& other) : _VarCRTP<T, Derived>(std::move(other)) {};
            _Var& operator=(_Var&& other) {
                _VarCRTP<T, Derived>::operator=(std::move(other));
                return *this;
            }
        };

    }
}

#endif
