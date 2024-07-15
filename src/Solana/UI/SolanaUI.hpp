#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Solana/File/FileManager.hpp>
#include <Solana/Solana.hpp>
#include <Grid.hpp>

using namespace geode::prelude;

class SolanaUI : public geode::Popup<GJGameLevel*> {
private:
    GJGameLevel* m_level;
protected:
    void play() {
        FMODAudioEngine::sharedEngine()->playEffect("playSound_01.ogg", 1.0, 0.0, 0.3);
        this->onClose(nullptr);
    }

    PlayLayer* createPlayLayer() {
        // auto c = new PlayLayer();
        // c->setObjType(CCObjectType::PlayLayer);
        // c->init(this->m_level, false, false);

        // // Speed up
        // c->m_gameState.m_timeWarp = Mod::get()->getSettingValue<double>("timewarp");

        return getChildOfType<PlayLayer>(PlayLayer::scene(this->m_level, false, false), 0);
    }

    void onReplay(CCObject* sender) {
        auto fileExists = FileManager::exists(this->m_level->m_levelName);
        if (!fileExists) {
            createQuickPopup(
                "File not found",
                "The replay file for this level was not found. Please train the level first.",
                "Ok", nullptr, nullptr);
            return;
        }

        auto replay = Solana::createReplayManager();
            
        // Read from file
        auto clicks = FileManager::load(this->m_level->m_levelName);

        if (clicks.empty()) {
            FLAlertLayer::create(
                "Error",
                "No clicks found",
                "OK"
            )->show();
            return;
        }

        replay->setClicks(clicks);

        Solana::setMode(Mode::Replay);

        auto c = createPlayLayer();

        auto scene = CCScene::create();
        scene->addChild(c);

        CCDirector::get()->pushScene(scene);
        this->play();
    }

    void onContinueTrainingFromFile(CCObject* sender) {
        auto fileExists = FileManager::exists(this->m_level->m_levelName);
        if (!fileExists) {
            createQuickPopup(
                "File not found",
                "The replay file for this level was not found. Please train the level first.",
                "Ok", nullptr, nullptr);
            return;
        }

        auto train = Solana::createTrainManager();
        Solana::setMode(Mode::TrainFromReplay);

        auto scene = CCScene::create();
        auto array = CCArray::create();

        auto clicks = FileManager::load(this->m_level->m_levelName);

        if (clicks.empty()) {
            FLAlertLayer::create(
                "Error",
                "No clicks found",
                "OK"
            )->show();
            return;
        }

        int rows = Mod::get()->getSettingValue<int64_t>("rows");
        int columns = Mod::get()->getSettingValue<int64_t>("columns");

        for (int i = 0; i < rows*columns; i++) {
            auto c = createPlayLayer();

            // Speed up
            c->m_gameState.m_timeWarp = Mod::get()->getSettingValue<double>("timewarp");

            c->m_isPracticeMode = true;

            train->addPlayer(c->m_player1->m_uID, c, Mode::TrainFromReplay);
            
            auto player = Solana::getTrainManager()->getPlayer(c->m_player1->m_uID);
            player->setClicks(clicks);
            train->setRestartTick(clicks.size());

            // Delete the last 20 clicks for good measure
            player->removeLastNthClicks(20);

            array->addObject(c);
        }

        train->setRestartTick(clicks.size());

        auto grid = Grid::create(rows, columns, array);
        scene->addChild(grid);

        CCDirector::get()->pushScene(scene);

        this->play();
    }

    void train() {
        auto train = Solana::createTrainManager();
        Solana::setMode(Mode::Train);

        auto scene = CCScene::create();
        auto array = CCArray::create();

        int rows = Mod::get()->getSettingValue<int64_t>("rows");
        int columns = Mod::get()->getSettingValue<int64_t>("columns");

        for (int i = 0; i < rows*columns; i++) {
            auto c = createPlayLayer();

            // Speed up
            c->m_gameState.m_timeWarp = Mod::get()->getSettingValue<double>("timewarp");

            c->m_isPracticeMode = true;

            train->addPlayer(c->m_player1->m_uID, c, Mode::Train);
            
            array->addObject(c);
        }

        auto grid = Grid::create(rows, columns, array);
        scene->addChild(grid);

        CCDirector::get()->pushScene(scene);

        this->play();
    }

    void onTrain(CCObject* sender) {
        if (FileManager::exists(this->m_level->m_levelName)) {
            createQuickPopup(
                "File already exists",
                "A replay file for this level already exists. Do you want to overwrite it?\n<cr>Warning: This action cannot be undone.</c>",
                "No", "Yes!", 
                [this](auto, auto btn2) {
                    if (btn2) {
                        this->train();
                        return;
                    }
                }
            );

            return;
        }

        this->train();
    }

    CCLabelBMFont* createLabel(const char* label, CCNode* node) {
        auto labelNode = CCLabelBMFont::create(label, "goldFont.fnt");
        labelNode->setScale(0.4f);
        labelNode->setPositionX(node->getContentSize().width / 2);
        labelNode->setPositionY(-10.f);
        node->addChild(labelNode);
        return labelNode;
    }

    void onConfig(CCObject* sender) {
        openSettingsPopup(Mod::get());
    }

    void onConfigInfo(CCObject* sender) {
        createQuickPopup(
            "Settings",
            "It's recommended to use the <cg>default settings</c>\n"
            "However, I encourage you to experiment with the settings to find the best fit.\n"
            "<cb>Recommendations:</c>\n"
            "* The higher the FPS while training, <co>the faster the training will be.</c>\n"
            "* Use <cg>Low graphics</c> for better performance.",
            "Ok", nullptr, nullptr
        );
    }
    
    bool setup(GJGameLevel* level) {
        this->m_level = level;

        auto winSize = m_mainLayer->getContentSize();

        this->setTitle("Solana");

        auto levelName = level->m_levelName.empty() ? "No level name" : level->m_levelName;

        auto levelNameLabel = CCLabelBMFont::create(levelName.c_str(), "bigFont.fnt");
        levelNameLabel->setPosition(winSize / 2);
        levelNameLabel->setPositionY(winSize.height - 50.f);

        auto hasReplayFile = FileManager::exists(levelName);

        this->m_mainLayer->addChild(levelNameLabel);

        auto buttonMenu = CCMenu::create();
        buttonMenu->setPosition(winSize / 2);

        auto layout = RowLayout::create();
        layout->setGap(20.f);

        buttonMenu->setLayout(layout);

        this->m_mainLayer->addChild(buttonMenu);

        auto replayButton = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_playBtn2_001.png"),
            this,
            menu_selector(SolanaUI::onReplay)
        );

        auto trainButton = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_practiceBtn_001.png"),
            this,
            menu_selector(SolanaUI::onTrain)
        );

        auto trainFromFileSprite = CCSprite::createWithSpriteFrameName("GJ_restartCheckBtn_001.png");
        trainFromFileSprite->setScale(1.5f);

        auto trainFromFile = CCMenuItemSpriteExtra::create(
            trainFromFileSprite,
            this,
            menu_selector(SolanaUI::onContinueTrainingFromFile)
        );

        if (hasReplayFile) {
            // Warning sprite
            auto warningSprite = CCSprite::createWithSpriteFrameName("exMark_001.png");
            warningSprite->setScale(0.5f);
            // Set top left of train button
            warningSprite->setPosition({ 10.0, 47.0 });

            trainButton->addChild(warningSprite);

            auto fullPath =  FileManager::getFullPath(levelName);
            auto pathLabel = CCLabelBMFont::create(CCString::createWithFormat("File: %s", fullPath.c_str())->getCString(), "chatFont.fnt");

            pathLabel->setPosition(pathLabel->getContentSize().width / 2 + 5, 15.0f);

            m_mainLayer->addChild(pathLabel);
        }
        else {
            replayButton->setColor(ccGRAY);
            replayButton->setSizeMult(1.0);

            trainFromFile->setColor(ccGRAY);
            trainFromFile->setSizeMult(1.0);
        }

        buttonMenu->addChild(trainButton);
        buttonMenu->addChild(replayButton);
        buttonMenu->addChild(trainFromFile);

        // Labels
        createLabel("Replay", replayButton);
        createLabel("Train", trainButton);
        createLabel("Train from replay", trainFromFile);

        buttonMenu->updateLayout();

        // Config button
        auto configButton = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png"),
            this,
            menu_selector(SolanaUI::onConfig)
        );

        // Config Info button
        auto configInfoButton = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png"),
            this,
            menu_selector(SolanaUI::onConfigInfo)
        );

        configInfoButton->setPositionY(configButton->getPositionY() + 40.f);

        auto configButtonMenu = CCMenu::create(configButton, configInfoButton, nullptr);
        configButtonMenu->setPositionX(winSize.width - 50.f);
        configButtonMenu->setPositionY(configButton->getContentSize().height / 2 + 10.f);

        m_mainLayer->addChild(configButtonMenu);


        buttonMenu->updateLayout();
        return true;
    };

public:
    static SolanaUI* create(GJGameLevel* level) {
        auto ret = new SolanaUI();
        if (ret->initAnchored(450.f, 270.f, level, "GJ_square05.png")) {
            ret->autorelease();
            return ret;
        }

        delete ret;
        return nullptr;
    }
};