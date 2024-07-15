#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

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
        float itemWidth = first->getContentSize().width;
        float itemHeight = first->getContentSize().height;
        float gapX = itemWidth * 0.1f; // 10% gap
        float gapY = itemHeight * 0.1f; // 10% gap

        // Calculate the total size of the grid
        float totalWidth = cols * (itemWidth + gapX) - gapX;
        float totalHeight = rows * (itemHeight + gapY) - gapY;

        // Calculate scale to fit within the window
        float scaleX = (winSize.width * 0.9f) / totalWidth;
        float scaleY = (winSize.height * 0.9f) / totalHeight;
        float scale = fmin(scaleX, scaleY);

        // Apply scaling
        itemWidth *= scale;
        itemHeight *= scale;
        gapX *= scale;
        gapY *= scale;
        totalWidth *= scale;
        totalHeight *= scale;

        // Create a container node to hold all items
        CCNode* container = CCNode::create();
        container->setAnchorPoint(ccp(0.5f, 0.5f)); // Set anchor point to center
        this->addChild(container);

        // Setup the items
        for (int i = 0; i < items->count(); i++) {
            auto item = dynamic_cast<CCNode*>(items->objectAtIndex(i));
            
            int row = i / cols;
            int col = i % cols;

            // Calculate the position within the container
            float x = col * (itemWidth + gapX) - totalWidth / 2 + itemWidth / 2;
            float y = totalHeight / 2 - (row * (itemHeight + gapY) + itemHeight / 2); // Flip Y-axis

            item->setPosition(ccp(x, y));
            item->setScale(scale);
            container->addChild(item);
        }

        // Position the container in the center of the layer
        container->setPosition(0,0);

        return true;
    }
};