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

    void optimizeBrain(Brain* best) {
        // Remove last 2 clicks
        // best->removeLastNthClicks(2);

        // Every 1000 ticks, restart from the beginning
        if (best->m_playLayer->m_gameState.m_currentProgress > m_restartTick) {
            m_restartTick += 1000;
            for (auto& c : best->m_clicks) {
                c.m_checkpoint = nullptr;
                c.m_fixPlayerCheckpoint = nullptr;
            }
            best->m_deaths.clear();
            best->m_farthestDeathTick = 0;
            best->m_lastDeathTick = 0;

            geode::log::debug("Restarting from the beginning");
            return;
        }

        // Remove depending on the death counter
        if (best->m_lastDeathTick <= best->m_farthestDeathTick) {
            auto steps = best->m_deaths[best->m_lastDeathTick];
            // geode::log::debug("Last death tick: {}, removing {} steps", best->m_lastDeathTick, steps);
            best->removeLastNthClicks(steps);
            // If the death counter step is above 500, remove all deaths
            if (steps > 100) {
                // geode::log::debug("Removing all deaths");
                best->m_deaths.clear();
                best->m_farthestDeathTick = best->m_lastDeathTick;
            }
        }
        // Means we reached the farthest death tick so we can remove all deaths
        else {
            // geode::log::debug("Farthest death tick reached: {}", best->m_farthestDeathTick);
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
        Brain* best = nullptr;
        for (auto& layer : m_playlayers) {
            if (best == nullptr || layer.second.m_clicks.size() > best->m_clicks.size()) {
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