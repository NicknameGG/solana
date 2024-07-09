#pragma once

#include "Solana/FixPlayerCheckpoint.hpp"
enum PlayerAction {
    Hold,
    Release,
    None,
    Invalid
};

class Click {
public:
    int gameTick;
    PlayerAction action;
    geode::Ref<CheckpointObject> m_checkpoint = nullptr;
    FixPlayerCheckpoint* m_fixPlayerCheckpoint;

    Click(int gameTick, PlayerAction action) {
        this->gameTick = gameTick;
        this->action = action;
    }

    Click(int gameTick, PlayerAction action, geode::Ref<CheckpointObject> checkpoint, FixPlayerCheckpoint* fixPlayerCheckpoint) {
        this->gameTick = gameTick;
        this->action = action;
        this->m_checkpoint = checkpoint;
        this->m_fixPlayerCheckpoint = fixPlayerCheckpoint;
    }
};