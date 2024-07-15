#pragma once

#include "Solana/Train/Brain/Brain.hpp"
#include "Solana/Mode.hpp"

#include <Geode/loader/Log.hpp>

class TrainManager {
public:
    // id, brain
    std::map<int, Brain> m_playlayers;
    int m_restartTick = 1000;

    TrainManager() {
        m_playlayers = {};
    }

    void setRestartTick(int tick) {
        m_restartTick = tick;
    }

    void onQuit() {
        for (auto& layer : m_playlayers) {
            layer.second.m_playLayer->onQuit();
            layer.second.m_playLayer->m_uiLayer->removeFromParentAndCleanup(true);
        }

        m_playlayers.clear();
    }

    void addPlayer(int id, PlayLayer* playLayer, Mode mode) {
        auto brain = Brain::create(playLayer, mode);
        m_playlayers[id] = brain;
    }

    Brain* getPlayer(int id) {
        auto player = m_playlayers.find(id);
        if (player != m_playlayers.end()) {
            return &player->second;
        }

        return nullptr;
    }

    bool isFirst(int id) {
        return m_playlayers.find(id) == m_playlayers.begin();
    }

    bool allDead() {
        for (auto& layer : m_playlayers) {
            if (!layer.second.m_isDead) {
                return false;
            }
        }

        return true;
    }

    void finishTraining() {
        for (auto& layer : m_playlayers) {
            layer.second.m_finished = true;
            layer.second.m_playLayer->m_player1->m_isDead = true;
        }
    }

    void optimizeBrain(Brain* best) {
        // Remove depending on the death counter
        auto steps = best->m_deaths[best->m_lastDeathTick];
        best->removeLastNthClicks(steps);

        // If the death counter step is above 200, remove all deaths
        if (steps > 200) {
            geode::log::debug("Removing all deaths");
            best->m_deaths.clear();
            best->m_farthestDeathTick = best->m_lastDeathTick;
        }

        if (best->m_playLayer->m_player1->m_position.y >= best->m_maxLevelY) {
            geode::log::debug("Void death, removing until ground");
            for (int i = best->m_clicks.size() - 1; i >= 0; i--) {
                auto& checkpoint = best->m_clicks[i].m_checkpoint;
                if (checkpoint != nullptr) {
                    if (!checkpoint->m_player1Checkpoint->m_isOnGround) {
                        best->removeLastNthClicks(best->m_clicks.size() - i);
                    }
                    else {
                        best->removeLastNthClicks(20);
                        break;
                    }
                }
            }
        }


        best->m_resetData = best->generateResetData();
    }

    Brain* getBestBrain(bool optimize = true) {
        Brain* best = &m_playlayers.begin()->second;
        for (auto& layer : m_playlayers) {
            if (layer.second.m_clicks.size() > best->m_clicks.size()) {
                // Check if player is around void and ignore
                if (layer.second.m_playLayer->m_player1->m_position.y >= layer.second.m_maxLevelY) {
                    continue;
                }
                best = &layer.second;
            }
        }

        if (optimize) {
            this->optimizeBrain(best);
        }

        return best;
    }

    void update() {
        if (this->allDead()) {
            // geode::log::debug("All players dead");
            Brain* best = this->getBestBrain();
            // Do something
            for (auto& layer : m_playlayers) {
                layer.second.m_isDead = false;
                layer.second.m_reset = true;
                layer.second.copy(best);
            }
        }
    }
};