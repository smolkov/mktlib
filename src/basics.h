#ifndef _DEUTSCH_
	#define _DEUTSCH_
#endif

#if defined basics_USE

/* Diese Datei wird zwar über 'mktlib.h' immer einbezogen, hat aber nur
-- dann auch eine Auswirkung, wenn basics_USE entsprechend definiert
-- wird.

-- basics.h kann mit verschiedenen Definitionen von basics_USE auch
-- mehrfach einbezogen werden. Die noch nicht erfolgten Definitionen/
-- Deklarationen werden hinzugenommen.

-- #define basics_USE BASICS_ALL
-- #include "basics.h"
-- führt alle Definitionen/Deklarationen aus 'basics.h' durch.
*/

#define BASICS_parentheses    1
#define BASICS_NEXT           2
#define BASICS_MASK           4
#define BASICS_WIDTH          8
#define BASICS_cat           16
#define BASICS_str           32
#define BASICS_id            64
#define BASICS_arrSize      128
#define BASICS_struct       256
#define BASICS_gboolean     512
#define BASICS_freeToNull  1024
#define BASICS_new         2048
#define BASICS_trc         4096
#define BASICS_boolStr     8192
#define BASICS_setLabel   16384
#define BASICS_ALL        32767

#if (basics_USE) & BASICS_parentheses
	#ifndef BASICS_DONE_parentheses
		#define BASICS_DONE_parentheses
		#pragma GCC diagnostic ignored "-Wparentheses"
	#endif
#endif

#if (basics_USE) & BASICS_NEXT
	#ifndef BASICS_DONE_NEXT
		#define BASICS_DONE_NEXT
		/* Manchmal ist es nützlich, wenn man zu einer Zahl a die kleinste Zahl
		-- ermitteln kann, in der nur ein Bit gesetzt ist welches in a nicht
		-- gesetzt ist.

		--   Zum Beispiel (in Binärdarstellung):
		-- Für 110      --> 1000
		-- Für 10000101 --> 100000000

		-- In einer Bitmaske kann man auf diese Weise abzählungen unterbringen.
		-- Man zählt z. B.:
		-- Ding1, Bing2, Ding3 ... DingN
		-- DingN hat einen Wert, der vielleicht nicht den Kompletten Platz in
		-- einem 32Bit Speicherplatz verbraucht. So will man vielleicht noch
		-- einzelne Eigenschaften hinzufügen:
		--   Eigenschaft1 = NEXT31(DingN)
		--   Eigenschaft2 = Eigenschaft1 << 1

		-- Das hier festgelegte Makro funktioniert für 32-Bit Speicherplätze.
		*/
		#define NEXT31(a) ((a) & 0xFFFF0000 ? NEXT15((a)>>16)<<16 : NEXT15(a))
		#define NEXT15(a) ((a) & 0xFF00     ? NEXT7 ((a)>> 8)<< 8 : NEXT7 (a))
		#define NEXT7(a)  ((a) & 0xF0       ? NEXT3 ((a)>> 4)<< 4 : NEXT3 (a))
		#define NEXT3(a)  ((a) & 0xC        ? NEXT2 ((a)>> 2)<< 2 : NEXT2 (a))
		#define NEXT2(a)  ((a) & 2          ? NEXT1 ((a)>> 1)<< 1 : NEXT1 (a))
		#define NEXT1(a)  ((a) & 1          ? 2                   : 1)
	#endif
#endif

#if (basics_USE) & BASICS_MASK
	#ifndef BASICS_DONE_MASK
		#define BASICS_DONE_MASK

		#define MASK31(a) ((a) & 0xFFFF0000 ? (MASK15((a)>>16)<<16) | 0xFFFF : MASK15(a))
		#define MASK15(a) ((a) & 0xFF00     ? (MASK7 ((a)>> 8)<< 8) | 0xFF   : MASK7 (a))
		#define MASK7(a)  ((a) & 0xF0       ? (MASK3 ((a)>> 4)<< 4) | 0xF    : MASK3 (a))
		#define MASK3(a)  ((a) & 0xC        ? (MASK1 ((a)>> 2)<< 2) | 3      : MASK1 (a))
		#define MASK1(a)  ((a) & 2          ? (MASK0 ((a)>> 1)<< 1) | 1      : MASK0 (a))
		#define MASK0(a)  ((a) & 1)
	#endif
#endif


#if (basics_USE) & BASICS_WIDTH
	#ifndef BASICS_DONE_WIDTH
		#define BASICS_DONE_WIDTH

		#define WIDTH10(a) ((a)>999999999 ? 10                      : WIDTH9(a))
		#define WIDTH9(a)  ((a)>999999    ? WIDTH3((a)/1000000) + 6 : WIDTH6(a))
		#define WIDTH6(a)  ((a)>999       ? WIDTH3((a)/1000)    + 3 : WIDTH3(a))
		#define WIDTH3(a)  ((a)>99        ? 3                       : WIDTH2(a))
		#define WIDTH2(a)  ((a)>9         ? 2                       : 1)
	#endif
#endif

#if (basics_USE) & BASICS_cat
	#ifndef BASICS_DONE_cat
		#define BASICS_DONE_cat

		#define catpre(a,b) a ## b
		#define cat(a,b)    catpre(a,b)

		#define cat6pre(a,b,c,d,e,f) a ## b ## c ## d ## e ## f
		#define cat6(a,b,c,d,e,f)    cat6pre(a,b,c,d,e,f)
	#endif
#endif


#if (basics_USE) & BASICS_str
	#ifndef BASICS_DONE_str
		#define BASICS_DONE_str

		#define strpre(a) #a
		#define str(a)    strpre(a)
	#endif
#endif

#if (basics_USE) & BASICS_id
	#ifndef BASICS_DONE_id
		#define BASICS_DONE_id
		#define id(a) a
	#endif
#endif

#if (basics_USE) & BASICS_arrSize
	#ifndef BASICS_DONE_arrSize
		#define arrSize(arr) (sizeof (arr) / sizeof (arr) [0])
	#endif
#endif

#if (basics_USE) & BASICS_struct
	#ifndef BASICS_DONE_struct
		#define BASICS_DONE_struct

		#define bitOrDELIM  |
		#define commaDELIM  ,
		#define noDELIM

		////////////////////////////////////////////////////////////////////////
		// - Makros für Datentypen                                            //
		////////////////////////////////////////////////////////////////////////

		/* Um  für einen 'struct' immer die richtige Initialisierungsliste be-
		-- reit zu haben, ist es nützlich, wenn Typ, Eintragsname und Initiali-
		-- sierung immer in einer Zeile stehen. In der c-Schreibweise ist dies
		-- nicht so, an einer Stelle (in der Deklaration) stehen Typ und Ein-
		-- tragsname und an einer anderen Stelle steht die Initialisierung.
		--   Abhilfe soll hier eine Konstruktion von Makros schaffen. Zunächst
		-- wird eine Liste definiert, in der wie gewünscht Typ, Eintragsname und
		-- Initialisierung in einer Zeile stehen:
		--   #define LISTE(f,d) \
		--      f(ErsterTyp,  ersterName,  ersteInitialisierung)  d##DELIM \
		--      f(ZweiterTyp, ZweiterName, ZweiteInitialisierung) d##DELIM \
		--      ...
		--      f(LetzterTyp, LetzterName, LetzteInitialisierung)
		--   f heißt 'Funktion' und enthält ein Makro mit drei Argumenten. Die
		-- für die Löung unseres Problems erforderlichen Funktionen stehen un-
		-- ten: 'decl' für die Deklaration und 'init' für die Initialisierung.
		--   d heißt 'Delimiter' und enthält den ersten Teile eines Makronamens,
		-- der den Trenner definiert. Die möglichen Argumente sind 'comma' (für
		-- den Trenner Komma) und 'no' (für keinen Trenner). 'no' wird für die
		-- Deklaration und 'comma' für die Initialisierung verwendet.
		--   Für die Deklaration ist gibt es eine Zusammenfassung in dem Makro
		-- struct bzw. für getrennte Typdefinition und Strukturdefinition in den
		-- Makros 'struct0' für die Typdefinition und 'struct1' für die Struk-
		-- turdefinition.
		--   Nach der Definition der Liste 'LISTE' kann der Datentype mit
		--   struct (Typename, LISTE);
		-- sofort definert werde. Bei der Trennung von Typdefinition und Struk-
		-- turdefinition, die typischerweise erforderlich wird, wenn ein Eintrag
		-- innerhalb einer Struktur einen Zeiger auf die Struktur selbst enthal-
		-- ten soll (verkettete Liste), werden die Makros 'struct0' und
		-- 'struct1' verwendet.
		--
		--   Beispiele:
		--   #define ADRESSE(f,d) \
		--      f(char*,    name,     NULL) d##DELIM \
		--      f(char*,    strasse,  NULL) d##DELIM \
		--      f(unsigned, hausnr,   NULL) d##DELIM \
		--      f(unsigned, postlz,   0)    d##DELIM \
		--      f(char*,    ort,      NULL) d##DELIM \
		--      f(Adresse*, naechste, NULL)
		--
		--   struct0 (Adresse);
		--   struct1 (Adresse, ADRESSE);
		--   #define INIT_Adresse {ADRESSE (init, comma)}
		--
		-- Eine Variable vom Typ 'Adresse' kann dann so definiert werden:
		--   Adresse a = INIT_Adresse;
		*/
		#define decl(l,m,r) l m;  // s/decl *(\([^,][^,]*\),\(  *\)\([^ ,][^,]*\),\(  *\)\([^ )][^)]*\))/\1\2\3;/
		#define init(l,m,r) r     // s/init *(\([^,][^,]*\),\(  *\)\([^ ,][^,]*\),\(  *\)\([^ )][^)]*\))/\5/

		#define struct0(Typename)       typedef struct Typename Typename
		#define struct1(Typename,LISTE) struct Typename  {LISTE (decl, no)}
		#define struct(Typename,LISTE)  typedef struct Typename {LISTE (decl, no)} Typename
	#endif
#endif

#if (basics_USE) & BASICS_gboolean
	#ifndef BASICS_DONE_gboolean
		#define BASICS_DONE_gboolean

		/* Wenn FALSE und TRUE als Makros definiert sind, so werden sie
		-- bereits beim Preprozessorlauf verfremdet. Da ich zur Fehler-
		-- suche gerne den reinen Preprozessorlauf verwende, ziehe ich
		-- eine Definition durch 'enum' vor. */
		#undef TRUE
		#undef FALSE

		enum {FALSE, TRUE};
	#endif
#endif

#if (basics_USE) & BASICS_freeToNull
	#ifndef BASICS_DONE_freeToNull
		#define BASICS_DONE_freeToNull

		#define freeToNull(free,obj) \
			do {\
				if (obj)\
					free (obj);\
				\
				(obj) = NULL;\
			}\
			while (0)

		#define g_object_unref(obj)  freeToNull (g_object_unref, obj)
		#define g_variant_unref(obj) freeToNull (g_variant_unref,obj)
		#define g_free(ptr)          freeToNull (g_free,         ptr)
		#define g_list_free(list)    freeToNull (g_list_free,    list)
		#define g_slist_free(list)   freeToNull (g_slist_free,   list)
	#endif
#endif

#if (basics_USE) & BASICS_new
	#ifndef BASICS_DONE_new
		#define BASICS_DONE_new

		#define newArr(ptr,size) (ptr=g_malloc(sizeof*ptr*size))
		#define newObj(ptr)      (ptr=g_malloc(sizeof*ptr))
	#endif
#endif

#if (basics_USE) & BASICS_trc
	#ifndef BASICS_DONE_trc
		#define BASICS_DONE_trc

		#ifdef _TRACE_
			#if 1 <= _TRACE_
				gboolean trc = TRUE;
			#else
				extern gboolean trc;
			#endif

			#pragma GCC diagnostic ignored "-Wunused-value"
			#include <glib/gprintf.h>
			#define trace trc && g_printf
		#else
			#define trace(fmt, ...)
		#endif
	#endif
#endif

#if (basics_USE) & BASICS_boolStr
	#ifndef BASICS_DONE_boolStr
		#define BASICS_DONE_boolStr
		#define boolStr(a) ((a)?"TRUE":"FALSE")
	#endif
#endif

#if (basics_USE) & BASICS_setLabel
	#ifndef BASICS_DONE_setLabel
		#define BASICS_DONE_setLabel

		#define setLabel(text, label)\
			gtk_label_set_text (label, text)

		#define setString(text, label)\
			do {\
				gchar* value = text;\
				gtk_label_set_text (label, value);\
				g_free             (value);\
			}\
			while (0)

		#define setCString(obj, func, label)\
			do {\
				if (obj) \
					setLabel (func(obj), label);\
			}\
			while (0)

		#define setBoolean(obj, getFunc, falseValue, trueValue, label)\
			do {\
				if (obj)\
					if (getFunc(obj))\
						gtk_label_set_text (label, trueValue);\
					else\
						gtk_label_set_text (label, falseValue);\
				else\
					gtk_label_set_text (label, "");\
			}\
			while (0)

		#define setCEntry(obj, func, label)\
			do {\
				if (obj) \
					gtk_entry_set_text (label, func(obj));\
			}\
			while (0)
	#endif
#endif

#undef basics_USE

#endif
