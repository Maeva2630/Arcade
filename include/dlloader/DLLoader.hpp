/*
** EPITECH PROJECT, 2025
** G-OOP-400-MPL-4-1-arcade-6
** File description:
** DLloader.hpp
*/

#ifndef DLLOADER_HPP_
#define DLLOADER_HPP_
    #include <dlfcn.h>
    #include <stdexcept>
    #include <string>
    #include <iostream>
    #include "IModule.hpp"

template <typename T>
class DLLoader {
    private:
        std::string _libPath;
        void *_handle;

    public:

        class DLLoaderException : public std::exception
        {
            private:
                std::string _message;
            public:
                DLLoaderException(std::string message, std::string context) : _message(message) {};
                const char* what() const noexcept override { return this->_message.c_str(); };
        };

        DLLoader<T>(const std::string &libraryPath) : _libPath(libraryPath) {
            _handle = dlopen(_libPath.c_str(), RTLD_GLOBAL | RTLD_NOW);
            if (!_handle)
                throw DLLoaderException("On dlopen of " + _libPath, "DLLoader");
        };

        T *getInstance(const std::string &entryPoint) const {
            T *(*fptr)(void);
            T *obj = NULL;

            void *entry = dlsym(_handle, entryPoint.c_str());
            if (!entry)
                throw DLLoaderException("On dlsym of " + _libPath, "DLLoader");
            fptr = reinterpret_cast<T*(*)(void)>(entry);
            obj = fptr();
            if (!obj)
                throw DLLoaderException("On cast of " + _libPath, "DLLoader");
            return (obj);
        };

        ~DLLoader() {
            if (_handle)
                dlclose(_handle);
        }
};

#endif
