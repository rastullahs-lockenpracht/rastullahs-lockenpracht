#ifndef __RL_DialogPredicates_H__
#define __RL_DialogPredicates_H__

/*
	This Class was for setting and getting the properties etc.pp. of a Chatterbot
	This is done through a Ruby-Interface now, but still "History" etc.pp might be useful
	later, so I've included this class unchanged from original jAlice-Sourcecode, but made it static
*/

#include "DialogPrerequisites.h"

#include <string>
#include <map>

using namespace std;

#define HISTORY_SIZE 5
namespace rl
{
	typedef struct 
	{
		int size;
		int top;
		map<int, string> elements;
	} History;

	typedef struct 
	{
		int number;
		int size;
		map<int, string> elements;
	} Array;

	typedef map<pair<string, string>, History> HistoryMap;
	typedef map<pair<string, string>, Array> ArrayMap;

	class _RlDialogExport Predicates
	{
	public:
		//Predicates();
		//~Predicates();
		static void loadPredicates();
		static void savePredicates();
	
		static void addValue(const string &property, const string &id, const string &value);
		static string setValue(const string &property, const string &id, const string &value, int index = 1);
		static void clearValue(const string &property, const string &id, int index = 1);
		static void clearValues(const string &property, const string &id);
		static string getValue(const string &property, const string &id, int index = 1);
		static string getValues(const string &property, const string &id, const string &conjunction = " and ");
	
		static void addHistory(const string &property, const string &id, const string &value);
		static string getHistory(const string &property, const string &id, int index = 1);
	
		static void setProperty(const string &property, const string &value);
		static string getProperty(const string &property);
	
		static void specializePredicate(const string &property, const string &value, bool returnName = false);
		static void setGlobalDefault(const string &value);
		static string lookupPredicate(const string &property, const string &defaultValue);
	private:
		static HistoryMap history;
		static ArrayMap predicates;
		static map<string, string> properties;
	
		static map<string, pair<string, bool> > specializedPredicates;
		static string defaultPredicate;
	};
}
#endif
