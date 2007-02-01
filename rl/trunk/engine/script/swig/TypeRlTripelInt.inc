/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

/* IN Typemaps fuer Tripel<int>.
 * Ein Tripel wird einfach auf ein dreielementiges Array abgebildet.
 * 
 * Eine Laengenueberpruefung findet nicht statt.
 * Nil wird zu 0. Indizes >= 3 werden ignoriert.
 * TODO eben das aendern
 */
%typemap(in) Tripel<int>
{
   Check_Type($input, T_ARRAY);
   Tripel<int> tripel(0, 0, 0);
   int length = RARRAY($input)->len;
   VALUE* it = RARRAY($input)->ptr;
   if (length > 0) {
      tripel.first = FIX2INT(*it);
      it++;
   }
   if (length > 1) {
      tripel.second = FIX2INT(*it);
      it++;
   }
   if (length > 2) {
      tripel.third = FIX2INT(*it);
   }
   $1 = tripel;
}

%typemap(in) Tripel<int>*, Tripel<int>&,
   const Tripel<int>*, const Tripel<int>&
{
   Check_Type($input, T_ARRAY);
   Tripel<int>* tripel = new Tripel<int>(0, 0, 0);
   int length = RARRAY($input)->len;
   VALUE* it = RARRAY($input)->ptr;
   if (length > 0) {
      tripel->first = FIX2INT(*it);
      it++;
   }
   if (length > 1) {
      tripel->second = FIX2INT(*it);
      it++;
   }
   if (length > 2) {
      tripel->third = FIX2INT(*it);
   }
   $1 = tripel;
}

%typemap(freearg) Tripel<int>*, Tripel<int>&, 
const Tripel<int>*, const Tripel<int>&
"   delete $1;"

/* OUT Typemaps fuer Tripel<int>.
 * Ein Tripel wird einfach auf ein dreielementiges Array abgebildet.
 * 
 */
%typemap(out) Tripel<int> {
   VALUE array = rb_ary_new();
   rb_ary_push(array, INT2FIX($1.first));
   rb_ary_push(array, INT2FIX($1.second));
   rb_ary_push(array, INT2FIX($1.third));
   $result = array;
}

