#ifndef __EIGENSCHAFT_H__
#define __EIGENSCHAFT_H__

#include "RulesPrerequisites.h"
#include "Tripel.h"

namespace rl
{
    static const int EIGENSCHAFT_COUNT = 8;

    static const int E_MUT = 0;
    static const int E_KLUGHEIT = 1;
    static const int E_INTUITION = 2;
    static const int E_CHARISMA = 3;
    static const int E_FINGERFERTIGKEIT = 4;
    static const int E_GEWANDTHEIT = 5;
    static const int E_KONSTITUTION = 6;
    static const int E_KOERPERKRAFT = 7;

    typedef Tripel<int> EigenschaftTripel;

    class _RlRulesExport Eigenschaft
    {
    private:
        const int mId;
        const std::string mName;
		const std::string mNameAbbreviation;
        const std::string mDescription;
    public:
        Eigenschaft(int id,
                    const std::string& name,
					const std::string& nameAbbrev,
                    const std::string& description);

        bool operator==(const Eigenschaft& rhs) const;
        bool operator<(const Eigenschaft& rhs) const;

        int getId() const;
        std::string getName() const;
        std::string getNameAbbreviation() const;
        std::string getDescription() const;
    };
}

#endif
