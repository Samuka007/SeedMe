#ifndef ErrorHandler_hpp
#define ErrorHandler_hpp

#include <exception>
#include <string>
#include <cstring>
#include <errno.h>
#include <format>
using std::string;
using std::exception;
using std::move;

class FileError : public std::exception {
    public:
        FileError(std::string filename) 
        :   m_filename { move(filename) }, 
            m_message { std::format("File error: {}.\nErrno: {}", m_filename, std::strerror(errno)) } {}
        const char* what() const noexcept override { return m_message.c_str();}
        virtual const string& getFilename() const noexcept { return m_filename;}
    
    protected:
        virtual void setMessage(string message) { m_message = move(message);}

    private:
        std::string m_filename;
        std::string m_message;
};

class FileOpenError : public FileError{
    public:
        FileOpenError(string filename) : FileError { move(filename) } {
            setMessage(std::format("Unable to open {}.\nErrno: {}", getFilename(), what()));
        }
};

class FileReadError : public FileError{
    public:
        FileReadError(string filename)
            : FileError {move(filename)} {
            setMessage(std::format("Unable to read file {}.\nErrno: {}", 
                                    getFilename(), std::strerror(errno)));
        }
    
    //private:
    //    size_t m_cursor;

};

#endif //Errorhandler.hpp