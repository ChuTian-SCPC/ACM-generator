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

        template<typename T>
        class Var : public _BasicVar {
        public:
            using Target = T;
            Var(const std::string& name = _setting::_empty_var_name) : _BasicVar(name) {};
            virtual ~Var() {};
            Var(const Var& other) : _BasicVar(other) {};
            Var& operator=(const Var& other) {
                _BasicVar::operator=(other);
                return *this;
            }
            Var(Var&& other) : _BasicVar(std::move(other)) {};
            Var& operator=(Var&& other) {
                _BasicVar::operator=(std::move(other));
                return *this;
            }

            virtual T read() = 0;

            std::string type() = 0;
        
        protected:
            virtual std::string __default_name() override {
               _AutoName<T> name;
               return name.name();
            }
        };

    }
}

#endif
