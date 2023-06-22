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

class FileOpenError : public FileError {
    public:
        FileOpenError(string filename) : FileError { move(filename) } {
            setMessage(std::format("Unable to open {}.\nErrno: {}", getFilename(), what()));
        }
};

class FileReadError : public FileError {
    public:
        FileReadError(string filename)
            : FileError {move(filename)} {
            setMessage(std::format("Unable to read file {}.\nErrno: {}", 
                                    getFilename(), std::strerror(errno)));
        }
    
    //private:
    //    size_t m_cursor;

};

class SourceError : exception {
    public:
        SourceError(unsigned id) : m_id {id} {
            setMessage(std::format("Source Error! ID: {}", m_id));
        }
        const char* what() const noexcept override { return m_message.c_str();}
        virtual const unsigned getErrorID() const noexcept { return m_id;}

    protected:
        virtual void setMessage(const string& message) { m_message = message;}
    private:
        unsigned m_id;
        string m_message;
};

class SourceNotFoundError : public SourceError {
    public:
        SourceNotFoundError(unsigned id = 0) : SourceError {id} {
            setMessage(std::format("Source not found! ID: {}", getErrorID()));
        }
};

class LoginError : public exception {
    public:
        LoginError(string username) : m_username {move(username)} {
            setMessage(std::format("Login Error! Username: {}", m_username));
        }
        const char* what() const noexcept override { return m_message.c_str();}
        virtual const string& getErrorUsername() const noexcept { return m_username;}

    protected:
        virtual void setMessage(const string& message) { m_message = message;}
    private:
        string m_username;
        string m_message;
};

class UsernameNotFoundError : public LoginError {
    public:
        UsernameNotFoundError(string username) : LoginError {move(username)} {
            setMessage(std::format("User not found! Username: {}", getErrorUsername()));
        }
};

class UseridNotFoundError : public exception {
    public:
        UseridNotFoundError(unsigned id) : m_id {id} {
            setMessage(std::format("Userid not found! ID: {}", m_id));
        }
        const char* what() const noexcept override { return m_message.c_str();}
        virtual const unsigned getErrorID() const noexcept { return m_id;}

    protected:
        virtual void setMessage(const string& message) { m_message = message;}
    private:
        unsigned m_id;
        string m_message;
};

class PasswordError : public exception {
    public:
        PasswordError() {
            setMessage("Password Error!");
        }
        const char* what() const noexcept override { return m_message.c_str();}
    protected:
        virtual void setMessage(const string& message) { m_message = message;}
    private:
        string m_message;
};

class PasswordTooShortError : public PasswordError {
    public:
        PasswordTooShortError() {
            setMessage("Password too short!");
        }
};

class PasswordIncorrectError : public PasswordError {
    public:
        PasswordIncorrectError() {
            setMessage("Password incorrect!");
        }
};

class RegisterError : public exception {
    public:
        RegisterError(string username) : m_username {move(username)} {
            setMessage(std::format("Register Error! Username: {}", m_username));
        }
        const char* what() const noexcept override { return m_message.c_str();}
        virtual const string& getErrorUsername() const noexcept { return m_username;}

    protected:
        virtual void setMessage(const string& message) { m_message = message;}
    private:
        string m_username;
        string m_message;
};

class UserAlreadyExistsError : public RegisterError {
    public:
        UserAlreadyExistsError(string username) : RegisterError {move(username)} {
            setMessage(std::format("User already exists! Username: {}", getErrorUsername()));
        }
};

class PasswordsDontMatchError : public RegisterError {
    public:
        PasswordsDontMatchError(string username) : RegisterError {move(username)} {
            setMessage(std::format("Passwords don't match! Username: {}", getErrorUsername()));
        }
};

class InvalidPasswordError : public RegisterError {
    public:
        InvalidPasswordError(string username) : RegisterError {move(username)} {
            setMessage(std::format("Invalid password! Username: {}", getErrorUsername()));
        }
};

class InvalidUsernameError : public RegisterError {
    public:
        InvalidUsernameError(string username) : RegisterError {move(username)} {
            setMessage(std::format("Invalid username! Username: {}", getErrorUsername()));
        }
};

class TagError : public exception {
    public:
        TagError(string tag) : m_tag {move(tag)} {
            setMessage(std::format("Tag Error! Tag: {}", m_tag));
        }
        const char* what() const noexcept override { return m_message.c_str();}
        virtual const string& getErrorTag() const noexcept { return m_tag;}

    protected:
        virtual void setMessage(const string& message) { m_message = message;}
    private:
        string m_tag;
        string m_message;
};

class TagNotFoundError : public TagError {
    public:
        TagNotFoundError(string tag) : TagError {move(tag)} {
            setMessage(std::format("Tag not found! Tag: {}", getErrorTag()));
        }
};

class TagAlreadyExistsError : public TagError {
    public:
        TagAlreadyExistsError(string tag) : TagError {move(tag)} {
            setMessage(std::format("Tag already exists! Tag: {}", getErrorTag()));
        }
};

class TagInvalidError : public TagError {
    public:
        TagInvalidError(string tag) : TagError {move(tag)} {
            setMessage(std::format("Invalid tag! Tag: {}", getErrorTag()));
        }
};

#endif //Errorhandler.hpp