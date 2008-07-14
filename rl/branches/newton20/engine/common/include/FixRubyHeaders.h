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

/**
 * Einige Undefines, f¸r unfreundliche defines in den Ruby-Headers.
 * Dort werden viele C-Funktionen ¸berdeckt, die in Ogre aber benˆtigt werden.
 * Diese Datei muss nach einem Ruby-Header inkludiert werden, mˆglicherweise auch
 * noch davor, um vorherige Makros zu lˆschen.
 */
#undef accept
#undef bind
#undef close
#undef select
#undef shutdown
#undef snprintf
#undef vsnprintf
#undef Sleep
#undef sleep
#undef strcasecmp
#undef rename
#undef write
#undef max
#undef min

#undef PACKAGE_VERSION
#undef PACKAGE_STRING
#undef PACKAGE_NAME
#undef PACKAGE_TARNAME
#undef HAVE_VSNPRINTF

#undef EWOULDBLOCK	
#undef EINPROGRESS	
#undef EALREADY	
#undef ENOTSOCK	
#undef EDESTADDRREQ	
#undef EMSGSIZE	
#undef EPROTOTYPE	
#undef ENOPROTOOPT	
#undef EPROTONOSUPPORT	
#undef ESOCKTNOSUPPORT	
#undef EOPNOTSUPP	
#undef EPFNOSUPPORT	
#undef EAFNOSUPPORT	
#undef EADDRINUSE	
#undef EADDRNOTAVAIL	
#undef ENETDOWN	
#undef ENETUNREACH	
#undef ENETRESET	
#undef ECONNABORTED	
#undef ECONNRESET	
#undef ENOBUFS		
#undef EISCONN		
#undef ENOTCONN	
#undef ESHUTDOWN	
#undef ETOOMANYREFS	
#undef ETIMEDOUT	
#undef ECONNREFUSED	
#undef ELOOP		
/*#undef ENAMETOOLONG	*/
#undef EHOSTDOWN	
#undef EHOSTUNREACH	
/*#undef ENOTEMPTY	*/
#undef EPROCLIM	
#undef EUSERS		
#undef EDQUOT		
#undef ESTALE		
#undef EREMOTE		

#undef CONNECT
#undef T_CONNECT
#undef T_DATA