#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

/**
 * @brief A custom row-horizontal (grid) layout
 */
class Grid : public CCLayer {
public:
    static Grid* create(int rows, int cols, CCArray* items) {
        auto ret = new Grid();
        if (ret && ret->init(rows, cols, items)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool init(int rows, int cols, CCArray* items) {
        if (!CCLayer::init()) return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        
        // Calculate item size and gap
        auto first = dynamic_cast<CCNode*>(items->objectAtIndex(0));
        float itemWidth = winSize.width / (cols + 1);  // Leave space for margins
        float itemHeight = winSize.height / (rows + 1);  // Leave space for margins
        float scaleX = itemWidth / first->getContentSize().width;
        float scaleY = itemHeight / first->getContentSize().height;
        float scale = fmin(scaleX, scaleY) * 0.9f; // 90% to leave some gap

        // Calculate the total size
        float totalWidth = cols * itemWidth;
        float totalHeight = rows * itemHeight;

        // Set the content size
        this->setContentSize(CCSizeMake(totalWidth, totalHeight));

        // Setup the items
        for (int i = 0; i < items->count(); i++) {
            auto item = dynamic_cast<CCNode*>(items->objectAtIndex(i));
            
            int row = i / cols;
            int col = i % cols;

            // Calculate the position
            float x = col * itemWidth + itemWidth / 2;
            float y = totalHeight - (row * itemHeight + itemHeight / 2); // Flip Y-axis

            item->setPosition(ccp(x, y));
            item->setScale(scale);
            this->addChild(item);
		}
        return true;
    }
};