#ifndef __ITEM_H__
#define __ITEM_H__

#include "GameObject.h"
#include <set>

namespace rl
{
    /// Aufnehmbare Objekte in der Spielwelt.
    class _RlRulesExport Item : public GameObject
    {
    public:
        Item(int id, const std::string& name, const std::string& description);
        virtual ~Item(void);

        void setWeight(int weight);
        int getWeight() const;

    private:
        /// Masse in Unzen.
        int mWeight;
    };

    class ItemByIdOrdering : public std::binary_function<Item*, Item*, bool> {
    public:
        bool operator()(const Item* const lhs, const Item* const rhs)
        {
            return lhs->getId() < rhs->getId();
        }
    };

    class FindItemById : public std::binary_function<int, Item*, bool> {
    public:
        bool operator()(const int id, const Item* const item) const
        {
            return item->getId() == id;
        }
    };

    typedef std::set<Item*, ItemByIdOrdering> ItemSet;
}

#endif
