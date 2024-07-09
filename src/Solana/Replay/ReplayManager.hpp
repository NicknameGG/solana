#pragma once

#include "Solana/Click.hpp"
class ReplayManager {
public:
    std::vector<Click> m_clicks;

    /*
    * @brief Returns a click based on the game tick
    */
    Click digest(int gameTick) {
        for (auto& click : m_clicks) {
            if (click.gameTick == gameTick) {
                geode::log::debug("ReplayManager::digest: Returning click at gametick {}", gameTick);
                return click;
            }
        }

        return Click(gameTick, PlayerAction::None);
    }

    void setClicks(std::vector<Click> clicks) {
        m_clicks = clicks;
    }
};