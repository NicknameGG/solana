#pragma once

#include <Solana/Click.hpp>
#include <Solana/Train/Player/PlayerUtil.hpp>
#include "Solana/Mode.hpp"

class Brain {
public:
    std::vector<Click> m_clicks;
    bool m_isDead = false;
    bool m_reset = false;
    // gametick, deaths
    std::map<int, int> m_deaths;
    int m_farthestDeathTick = 0;
    int m_lastDeathTick = 0;

    PlayLayer* m_playLayer;

    struct ResetData {
        geode::Ref<CheckpointObject> m_checkpoint = nullptr;
        FixPlayerCheckpoint* m_fixPlayerCheckpoint = nullptr;
    } m_resetData;

    Mode m_mode = Mode::Train;
    float m_maxLevelY = 0;

    std::map<PlayerUtil::GameMode, int> m_bias = {
        {PlayerUtil::GameMode::Cube, 8},
        {PlayerUtil::GameMode::Ship, 12},
        {PlayerUtil::GameMode::Ball, 8},
        {PlayerUtil::GameMode::UFO, 8},
        {PlayerUtil::GameMode::Wave, 4},
        {PlayerUtil::GameMode::Robot, 16},
        {PlayerUtil::GameMode::Spider, 8},
        {PlayerUtil::GameMode::Swing, 8}
    };

    struct Chance {
        int hold;
        int release;
    };

    std::map<PlayerUtil::GameMode, Chance> m_chance = {
        {PlayerUtil::GameMode::Cube, {4, 5}},
        {PlayerUtil::GameMode::Ship, {4, 4}},
        {PlayerUtil::GameMode::UFO, {1, 5}},
        {PlayerUtil::GameMode::Wave, {5,5}},
        {PlayerUtil::GameMode::Robot, {9, 1}},
        {PlayerUtil::GameMode::Ball, {2, 8}},
        {PlayerUtil::GameMode::Spider, {3, 7}},
        {PlayerUtil::GameMode::Swing, {1, 1}}
    };  

    Brain() {
        m_clicks = {};
        m_deaths = {};
        m_farthestDeathTick = 0;
        m_lastDeathTick = 0;
    }

    static Brain create(PlayLayer* playLayer, Mode mode) {
        auto brain = Brain();
        brain.m_playLayer = playLayer;
        brain.m_mode = mode;

        cocos2d::CCArray* objects = brain.m_playLayer->m_objects;
        for (auto& object : geode::cocos::CCArrayExt<GameObject*>(objects)) {
            // object->m_hasNoEffects = true;
            // object->m_isHide = true;
            // object->m_hasNoParticles = true;

            if (object->getPositionY() > brain.m_maxLevelY) {
               brain.m_maxLevelY = object->getPositionY();
            }

            // // Mirror portal            
            // if (object->m_objectID == 45) {
            //     geode::log::info("Brain::create: Found mirror portal");
            //     object->setOpacity(0);
            //     continue;
            // }
        }

        return brain;
    }

    void setMode(Mode mode) {
        this->m_mode = mode;
    }

    ResetData createCheckpoint() {
        int gameTick = this->m_playLayer->m_gameState.m_currentProgress;
        bool canCreateCheckpoint = false;

        auto plr = this->m_playLayer->m_player1;
        auto gamemode = PlayerUtil::getGameMode(plr);

        // Create a checkpoint based on the criteria
        if (gamemode == PlayerUtil::GameMode::Cube ||
            gamemode == PlayerUtil::GameMode::Ball ||
            gamemode == PlayerUtil::GameMode::Robot) {
            if (plr->m_isOnGround) {
                canCreateCheckpoint = true;
            }
        }
        else if (gamemode == PlayerUtil::GameMode::Ship ||
                 gamemode == PlayerUtil::GameMode::UFO) {
            if (gameTick % 20 == 0) {
                canCreateCheckpoint = true;
            }
        }
        // Testing
        else {
            canCreateCheckpoint = true;
        }

        if (canCreateCheckpoint) {
            // Set global play layer
            auto old_playlayer = GameManager::get()->m_playLayer;
            GameManager::get()->m_playLayer = this->m_playLayer;

            auto checkpoint = geode::Ref(this->m_playLayer->createCheckpoint());

            auto fixPlayerCheckpoint = new FixPlayerCheckpoint(plr);

            // Revert back to old play layer
            GameManager::get()->m_playLayer = old_playlayer;
            return ResetData{checkpoint, fixPlayerCheckpoint};
        }

        return ResetData();
    }

    /*
    * @brief Returns a click based on the game tick
    */
    Click digest() {
        if (m_mode == Mode::Nothing) {
            return Click(0, PlayerAction::None);
        }

        int gameTick = this->m_playLayer->m_gameState.m_currentProgress;

        for (auto& click : m_clicks) {
            if (click.gameTick == gameTick) {
                auto data = this->createCheckpoint();
                click.m_checkpoint = data.m_checkpoint;
                click.m_fixPlayerCheckpoint = data.m_fixPlayerCheckpoint;

                return click;
            }
        }

        auto plr = this->m_playLayer->m_player1;
        auto gamemode = PlayerUtil::getGameMode(plr);

        if (gameTick % m_bias[gamemode] != 0) {
            Click click = Click(gameTick, PlayerAction::None);
            // m_clicks.push_back(click);
            return click;
        }

        auto data = this->createCheckpoint();

        Chance chance = m_chance[gamemode];

        if (plr->m_isDashing) {
            chance.hold = 9;
            chance.release = 1;
        }

        // Random click % bias
        Click click = Click(gameTick, 
                    rand() % 10 < chance.hold ? PlayerAction::Hold : PlayerAction::Release, 
                            data.m_checkpoint, data.m_fixPlayerCheckpoint);

        m_clicks.push_back(click);
        return click;
    }

    void removeLastNthClicks(int n) {
        if (m_clicks.size() < n) {
            m_clicks.clear();
            return;
        }

        m_clicks.erase(m_clicks.end() - n, m_clicks.end());
    }

    void die(int gameTick) {
        m_isDead = true;
        m_deaths[gameTick] = m_deaths[gameTick] == 0 ? 1 : m_deaths[gameTick] + 1;

        m_lastDeathTick = gameTick;
    }

    ResetData generateResetData() {
        for (int i = m_clicks.size() - 1; i >= 0; i--) {
            auto& checkpoint = m_clicks[i].m_checkpoint;
            if (checkpoint != nullptr) {
                return ResetData{checkpoint, m_clicks[i].m_fixPlayerCheckpoint};
            }
        }

        // geode::log::error("Brain::generateResetData: No checkpoint found");

        return ResetData();
    }

    ResetData consumeResetData() {
        return m_resetData;
    }

    void copy(Brain* brain) {
        this->m_clicks = brain->m_clicks;
        this->m_resetData = brain->m_resetData;
    }

    void setClicks(std::vector<Click> clicks) {
        this->m_clicks = clicks;
    }
};