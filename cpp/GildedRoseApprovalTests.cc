#include "ApprovalTests.v.2.0.0.hpp"
#include <gtest/gtest.h>
#include "GildedRose.h"

struct ItemUpdateValidator
{
#if 1
    Item item;
    ItemUpdateValidator(string name, int sellIn, int quality) : item(name, sellIn, quality) {}

    void updateQuality() {
        vector<Item> items = {item};
        GildedRose app(items);
        app.updateQuality();
    }

    friend std::ostream& operator<<(std::ostream& os, const ItemUpdateValidator& obj)
    {
        return os
            << "name: " << obj.item.name
            << ", sellIn: " << obj.item.sellIn
            << ", quality: " << obj.item.quality;
    }
#else
    string name;
    int sellIn;
    int quality;

    friend std::ostream& operator<<(std::ostream& os, const ItemUpdateValidator& obj)
    {
        return os
            << "name: " << obj.name
            << ", sellIn: " << obj.sellIn
            << ", quality: " << obj.quality;
    }
#endif

};

TEST(GildedRoseApprovalTests, Two)
{
    std::vector<string> names { "Foo" };
    std::vector<int> sellIns { 1 };
    std::vector<int> qualities { 1 };
#if 1
    CombinationApprovals::verifyAllCombinations<
        std::vector<string>, std::vector<int>, std::vector<int>, ItemUpdateValidator>(
            [](string name, int sellIn, int quality) {
            ItemUpdateValidator tmp(name, sellIn, quality);
            tmp.updateQuality();
            return tmp;
            },
            names, sellIns, qualities);
#endif
}
