#ifndef __CONTAINER_H__
#define __CONTAINER_H__

#include "GameObject.h"
#include "Item.h"

namespace rl
{
    /// Behaelter für Items.
    class _RlRulesExport Container : public GameObject
    {
    public:
        Container(int id, const std::string& name, const std::string& description);
        virtual ~Container(void);

        /// Fassungsvermoegen in Unzen
        int getCapacity() const;

        /// Fassungsvermoegen in Unzen
        void setCapacity(int capacity);

        /// Liefert Gesamtgewicht des Inhalts.
        int getContentWeight() const;

        void addItem(Item* item);
        Item* removeItem(int itemId);

        ItemSet getItems() const;

    private:
        int mCapacity;
        ItemSet mItems;
    };
}

#endif
