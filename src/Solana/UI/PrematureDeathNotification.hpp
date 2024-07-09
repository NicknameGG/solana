#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

/**
* @brief A non intrusive bottom left notification that is displayed when the player dies prematurely. Clicking on it will prompt the player to restart the level.
*/
class PrematureDeathNotification : public CCLayer {
public:
    static PrematureDeathNotification* create(const char* message) {
        auto pRet = new PrematureDeathNotification();
        if (pRet && pRet->init(message)) {
            pRet->autorelease();
            return pRet;
        }
        CC_SAFE_DELETE(pRet);
        return nullptr;
    }

    void onRestart(CCObject* sender) {
        createQuickPopup(
            "Info",
            "Player died before reaching last tick. Use Train From File to continue training.",
            "Ok",
            nullptr,
            nullptr
        );
    }

    bool init(const char* message) {
        if (!CCLayer::init()) {
            return false;
        }

        auto bg = CCScale9Sprite::create("GJ_square01.png");
        auto label = CCLabelBMFont::create(message, "bigFont.fnt");

        bg->addChild(label);
        bg->setContentSize(CCSizeMake(label->getContentSize().width + 20, label->getContentSize().height + 20));

        bg->setScale(0.4f);

        label->setPosition(bg->getContentSize() / 2);

        auto button = CCMenuItemSpriteExtra::create(
            bg,
            this,
            menu_selector(PrematureDeathNotification::onRestart)
        );

        // Bottom left corner
        button->setPosition(button->getContentSize() / 2);

        auto menu = CCMenu::create(button, nullptr);
        menu->addChild(button);

        menu->setPosition(0, 0);


        this->addChild(menu);

        return true;
    }

    void show() {
        CCDirector::get()->getRunningScene()->addChild(this, 1000);
    }
};