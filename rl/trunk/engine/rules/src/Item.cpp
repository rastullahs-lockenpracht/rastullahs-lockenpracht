#include "Item.h"

using namespace std;

namespace rl
{
    Item::Item(int id, const CeGuiString& name, const CeGuiString& description)
        : GameObject(id, name, description)
    {
    }

    Item::~Item(void)
    {
    }

    void Item::setWeight(int weight)
    {
        mWeight = weight;
    }

    int Item::getWeight() const
    {
        return mWeight;
    }

}
