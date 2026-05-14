#ifndef ASTFRI_IMPL_TOOLS_UNINITIALIZED_EXCEPTION_HPP
#define ASTFRI_IMPL_TOOLS_UNINITIALIZED_EXCEPTION_HPP

#include <exception>


namespace astfri {


class UninitializedException : public std::exception {
public:
    const char *what() const noexcept override {
        return "Using something not properly initialized.";
    }
};


class ValuelessException : public std::exception {
    const char *what() const noexcept override {
        return "Using something that was explicitly set not to contain any value.";
    }
};


} // namespace astfri


#endif
