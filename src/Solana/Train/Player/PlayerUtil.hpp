#pragma once

class PlayerUtil {
public:
    enum GameMode {
        Cube,
        Ship,
        Ball,
        UFO,
        Wave,
        Robot,
        Spider,
        Swing
    };

    static GameMode getGameMode(PlayerObject* player) {
       if (player->m_isShip) {
            return Ship;
        } else if (player->m_isBall) {
            return Ball;
        } else if (player->m_isBird) {
            return UFO;
        } else if (player->m_isDart) {
            return Wave;
        } else if (player->m_isRobot) {
            return Robot;
        } else if (player->m_isSpider) {
            return Spider;
        } else if (player->m_isSwing) {
            return Swing;
        } else {
            return Cube;
        }
    }
};