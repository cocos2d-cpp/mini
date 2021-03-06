#ifndef __TABLEVIEWTESTSCENE_H__
#define __TABLEVIEWTESTSCENE_H__

#include "BaseTest.h"

DEFINE_TEST_SUITE(TableViewTests);

class TableViewTest : public TestCase, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    static TableViewTest* create()
    {
        auto ret = new TableViewTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual bool init() override;  
    
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView*)override {}
    virtual void scrollViewDidZoom(cocos2d::extension::ScrollView*)override {}
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;
};

#endif // __TABLEVIEWTESTSCENE_H__
