/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Clarified Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Clarified Artistic License for more details.
 *
 *  You should have received a copy of the Clarified Artistic License
 *  along with this program; if not you can get it here
 *  http://www.jpaulmorrison.com/fbp/artistic2.htm.
 */

#ifndef __RL_EXCEPTION_H__
#define __RL_EXCEPTION_H__
/**
 * \file Exception.h
 * Einige Definitionen fuer Asserts und Exception Handling
 * und Meldungsausgabe.
 */

#include "CommonPrerequisites.h"
#include <iostream>
#include <string>

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include <Windows.h>
#undef min
#undef max
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
// visual studio >= 7.0 oder gar kein VS
#if !defined(_MSC_VER) || (_MSC_VER >= 1300)
#define __FUNC__ __FUNCTION__
#else
// VS < 7.0 kennt kein __FUNCTION__
#define __FUNC__ ""
#endif
#else
#define __FUNC__ __PRETTY_FUNCTION__
#endif

#ifndef RL_NO_EXCEPTIONS
/**@brief Makro fuer das Werfen von Exceptions.
 *
 * Benutzung:
 * @code
 * Throw(Exception, "Ein Fehler ist aufgetreten.");
 * @endcode
 * @param exception Klassenname der zu werfenden Exception.
 * @param msg Die Ursache fuer die Exception.
 */
#define Throw(exception, msg)                                                                                          \
    {                                                                                                                  \
        LOG_ERROR_SHORT(msg);                                                                                          \
        throw(exception(msg, __FILE__, __FUNC__, __LINE__));                                                           \
    }
/**@brief Makro zur Absicherung von Annahmen.
 *
 * Wenn der Ausdruck \c test zu \c false ausgewertet wird, wird eine
 * \c AssertionFailedException geworfen mit \c msg als Grund.
 *
 * @param test Boolescher Ausdruck der ausgewertet werden soll.
 * @param msg Meldung, die als Beschreibung an die Exception gegeben
 * werden soll, falls \c test \c false ist.
 */
#define RlAssert(test, msg)                                                                                            \
    if (!(test))                                                                                                       \
    Throw(AssertionFailedError, msg)
#define RlAssert1(test) RlAssert(test, #test)
/**@brief Makro zur Absicherung von Annahmen.
 *
 * Wirft eine \c AssertionFailedException. N?tzlich um
 * Zust?nde die nicht auftreten sollen abzusichern, etwa in
 * \c switch Bl?cken f?r den \c default, falls es keinen
 * solchen geben soll.
 *
 * @param test Boolescher Ausdruck der ausgewertet werden soll.
 * @param msg Meldung, die als Beschreibung an die Exception gegeben
 * werden soll, falls \c test \c false ist.
 */
#define RlFail(msg) Throw(AssertionFailedError, msg)
#endif
#ifdef RL_NO_ASSERTS
#define RlAssert(test, msg)
#define RlFail(msg)
#endif

namespace rl
{

    /**@brief Das ist die Basis-Exceptionklasse fuer das Projekt.
     *
     * Sie ist entgegen den Richtlinien inline definiert, weil sie keinem
     * Projekt zugehoert und nicht exportiert wird.
     * So ist es dann am einfachsten.
     *
     * @warning Nie direkt ueber throw() werfen,
     *    sondern mittels des Makros Throw().
     */
    class Exception : public std::exception
    {

    protected:
        std::string mMessage;
        std::string mFile;
        std::string mFunction;
        std::string mWhat;
        int mLine;

    public:
        Exception(const char* message, const std::string& file, const std::string& function, int line)
            : mMessage(message)
            , mFile(file)
            , mFunction(function)
            , mLine(line)
            , mWhat("")
            , std::exception()
        {
            update();
        }
        Exception(const std::string& message, const std::string& file, const std::string& function, int line)
            : mMessage(message)
            , mFile(file)
            , mFunction(function)
            , mLine(line)
            , mWhat("")
            , std::exception()
        {
            update();
        }
#if CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UNICODE
        Exception(const CeGuiString& message, const std::string& file, const std::string& function, int line)
            : mMessage(message.c_str())
            , mFile(file)
            , mFunction(function)
            , mLine(line)
            , mWhat("")
            , std::exception()
        {
            update();
        }
#endif

        Exception(const Exception& rhs)
            : mMessage(rhs.mMessage)
            , mFile(rhs.mFile)
            , mFunction(rhs.mFunction)
            , mLine(rhs.mLine)
            , mWhat("")
            , std::exception()
        {
            update();
        }

        virtual ~Exception() = default;

        Exception& operator=(const Exception& rhs)
        {
            if (this != &rhs)
            {
                mMessage = rhs.mMessage;
                mFile = rhs.mFile;
                mFunction = rhs.mFunction;
                mLine = rhs.mLine;
                update();
            }
            return *this;
        }

        virtual std::string getMessage() const
        {
            return mMessage;
        }
        virtual std::string getFile() const
        {
            return mFile;
        }
        virtual std::string getFunction() const
        {
            return mFunction;
        }
        virtual std::string getType() const
        {
            return "Exception";
        }
        virtual int getLine() const
        {
            return mLine;
        }

        const char* what() const throw()
        {
            return mWhat.c_str();
        }

        void update()
        {
            std::stringstream lineStream;

            lineStream << "In file " << getFile().c_str() << ",  line " << getLine() << ", function "
                       << getFunction().c_str() << "\nan exception of type " << getType().c_str()
                       << " was raised.\n-----------------------------------------------------------------------\n"
                       << getMessage().c_str();

            mWhat = lineStream.str();
        }
    };

    class Error : public Exception
    {
    public:
        Error(const char* message, const std::string& file, const std::string& function, int line)
            : Exception(message, file, function, line)
        {
        }
        Error(const std::string& message, const std::string& file, const std::string& function, int line)
            : Exception(message, file, function, line)
        {
        }
#if CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UNICODE
        Error(const CeGuiString& message, const std::string& file, const std::string& function, int line)
            : Exception(message, file, function, line)
        {
        }
#endif

        Error(const Error& rhs)
            : Exception(rhs)
        {
        }

        virtual std::string getType() const
        {
            return "Error";
        }
    };

    class RuntimeException : public Exception
    {
    public:
        RuntimeException(const char* message, const std::string& file, const std::string& function, int line)
            : Exception(message, file, function, line)
        {
        }
        RuntimeException(const std::string& message, const std::string& file, const std::string& function, int line)
            : Exception(message, file, function, line)
        {
        }
#if CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UNICODE
        RuntimeException(const CeGuiString& message, const std::string& file, const std::string& function, int line)
            : Exception(message, file, function, line)
        {
        }
#endif

        RuntimeException(const RuntimeException& rhs)
            : Exception(rhs)
        {
        }

        virtual std::string getType() const
        {
            return "RuntimeException";
        }
    };

    class NullPointerException : public RuntimeException
    {
    public:
        NullPointerException(const char* message, const std::string& file, const std::string& function, int line)
            : RuntimeException(message, file, function, line)
        {
        }
        NullPointerException(const std::string& message, const std::string& file, const std::string& function, int line)
            : RuntimeException(message, file, function, line)
        {
        }
#if CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UNICODE
        NullPointerException(const CeGuiString& message, const std::string& file, const std::string& function, int line)
            : RuntimeException(message, file, function, line)
        {
        }
#endif

        NullPointerException(const NullPointerException& rhs)
            : RuntimeException(rhs)
        {
        }

        virtual std::string getType() const
        {
            return "NullPointerException";
        }
    };

    class IllegalArgumentException : public RuntimeException
    {
    public:
        IllegalArgumentException(const char* message, const std::string& file, const std::string& function, int line)
            : RuntimeException(message, file, function, line)
        {
        }
        IllegalArgumentException(
            const std::string& message, const std::string& file, const std::string& function, int line)
            : RuntimeException(message, file, function, line)
        {
        }
#if CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UNICODE
        IllegalArgumentException(
            const CeGuiString& message, const std::string& file, const std::string& function, int line)
            : RuntimeException(message, file, function, line)
        {
        }
#endif

        IllegalArgumentException(const IllegalArgumentException& rhs)
            : RuntimeException(rhs)
        {
        }

        virtual std::string getType() const
        {
            return "IllegalArgumentException";
        }
    };

    class OperationNotSupportedException : public RuntimeException
    {
    public:
        OperationNotSupportedException(
            const char* message, const std::string& file, const std::string& function, int line)
            : RuntimeException(message, file, function, line)
        {
        }
        OperationNotSupportedException(
            const std::string& message, const std::string& file, const std::string& function, int line)
            : RuntimeException(message, file, function, line)
        {
        }
#if CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UNICODE
        OperationNotSupportedException(
            const CeGuiString& message, const std::string& file, const std::string& function, int line)
            : RuntimeException(message, file, function, line)
        {
        }
#endif

        OperationNotSupportedException(const OperationNotSupportedException& rhs)
            : RuntimeException(rhs)
        {
        }

        virtual std::string getType() const
        {
            return "OperationNotSupportedException";
        }
    };

    class IllegalStateException : public RuntimeException
    {
    public:
        IllegalStateException(const char* message, const std::string& file, const std::string& function, int line)
            : RuntimeException(message, file, function, line)
        {
        }
        IllegalStateException(
            const std::string& message, const std::string& file, const std::string& function, int line)
            : RuntimeException(message, file, function, line)
        {
        }
#if CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UNICODE
        IllegalStateException(
            const CeGuiString& message, const std::string& file, const std::string& function, int line)
            : RuntimeException(message, file, function, line)
        {
        }
#endif

        IllegalStateException(const IllegalStateException& rhs)
            : RuntimeException(rhs)
        {
        }

        virtual std::string getType() const
        {
            return "IllegalStateException";
        }
    };

    class AssertionFailedError : public Error
    {
    public:
        AssertionFailedError(const char* message, const std::string& file, const std::string& function, int line)
            : Error(message, file, function, line)
        {
        }
        AssertionFailedError(const std::string& message, const std::string& file, const std::string& function, int line)
            : Error(message, file, function, line)
        {
        }
#if CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UNICODE
        AssertionFailedError(const CeGuiString& message, const std::string& file, const std::string& function, int line)
            : Error(message, file, function, line)
        {
        }
#endif

        AssertionFailedError(const AssertionFailedError& rhs)
            : Error(rhs)
        {
        }

        virtual std::string getType() const
        {
            return "AssertionFailedError";
        }
    };

    class OutOfRangeException : public RuntimeException
    {
    public:
        OutOfRangeException(const char* message, const std::string& file, const std::string& function, int line)
            : RuntimeException(message, file, function, line)
        {
        }
        OutOfRangeException(const std::string& message, const std::string& file, const std::string& function, int line)
            : RuntimeException(message, file, function, line)
        {
        }
#if CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UNICODE
        OutOfRangeException(const CeGuiString& message, const std::string& file, const std::string& function, int line)
            : RuntimeException(message, file, function, line)
        {
        }
#endif

        OutOfRangeException(const OutOfRangeException& rhs)
            : RuntimeException(rhs)
        {
        }

        virtual std::string getType() const
        {
            return "OutOfRangeException";
        }
    };

    class DuplicateIdException : public RuntimeException
    {
    public:
        DuplicateIdException(const char* message, const std::string& file, const std::string& function, int line)
            : RuntimeException(message, file, function, line)
        {
        }
        DuplicateIdException(const std::string& message, const std::string& file, const std::string& function, int line)
            : RuntimeException(message, file, function, line)
        {
        }
#if CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UNICODE
        DuplicateIdException(const CeGuiString& message, const std::string& file, const std::string& function, int line)
            : RuntimeException(message, file, function, line)
        {
        }
#endif

        DuplicateIdException(const DuplicateIdException& rhs)
            : RuntimeException(rhs)
        {
        }

        virtual std::string getType() const
        {
            return "DuplicateIdException";
        }
    };

    class FileNotFoundException : public RuntimeException
    {
    public:
        FileNotFoundException(const char* message, const std::string& file, const std::string& function, int line)
            : RuntimeException(message, file, function, line)
        {
        }
        FileNotFoundException(
            const std::string& message, const std::string& file, const std::string& function, int line)
            : RuntimeException(message, file, function, line)
        {
        }
#if CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UNICODE
        FileNotFoundException(
            const CeGuiString& message, const std::string& file, const std::string& function, int line)
            : RuntimeException(message, file, function, line)
        {
        }
#endif

        FileNotFoundException(const FileNotFoundException& rhs)
            : RuntimeException(rhs)
        {
        }

        virtual std::string getType() const
        {
            return "FileNotFoundException";
        }
    };

    class WrongFormatException : public RuntimeException
    {
    public:
        WrongFormatException(const char* message, const std::string& file, const std::string& function, int line)
            : RuntimeException(message, file, function, line)
        {
        }
        WrongFormatException(const std::string& message, const std::string& file, const std::string& function, int line)
            : RuntimeException(message, file, function, line)
        {
        }
#if CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UNICODE
        WrongFormatException(const CeGuiString& message, const std::string& file, const std::string& function, int line)
            : RuntimeException(message, file, function, line)
        {
        }
#endif

        WrongFormatException(const WrongFormatException& rhs)
            : RuntimeException(rhs)
        {
        }

        virtual std::string getType() const
        {
            return "WrongFormatException";
        }
    };

    /**@brief Funktion zum ausgeben einer Nachricht an den Benutzer.
     *
     * Gibt ein Meldung an den Benutzer aus. Entweder ueber STDOUT oder
     * in einem Meldungsfenster aus.
     *
     * @todo Definition an einen passenderen Ort verschieben.
     */
    static void showMessage(const std::string& message)
    {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBox(NULL, message.c_str(), "Meldung", MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
#else
        std::cout << message << std::endl;
#endif
    }

    /**@brief Funktion zum ausgeben eines Fehlers an den Benutzer.
     *
     * Gibt ein Fehlermeldung an den Benutzer aus. Entweder ueber STDERR oder
     * in einem Meldungsfenster aus.
     *
     * @todo Definition an einen passenderen Ort verschieben.
     */
    static void showError(const std::string& message)
    {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBox(NULL, message.c_str(), "Fehler", MB_OK | MB_ICONERROR | MB_TOPMOST);
#else
        std::cerr << message << std::endl;
#endif
    }
}
#endif
