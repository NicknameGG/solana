#pragma once
// Main class
#include "Solana/Train/TrainManager.hpp"
#include "Solana/Replay//ReplayManager.hpp"
#include "Solana/Mode.hpp"

class Solana {
    static TrainManager* m_trainManager;
    static ReplayManager* m_replayManager;

public:
    static Mode m_mode;

public:
    static TrainManager* createTrainManager() {
        m_trainManager = new TrainManager();
        return m_trainManager;
    }

    static TrainManager* getTrainManager() {
        return m_trainManager;
    }

    static ReplayManager* createReplayManager() {
        m_replayManager = new ReplayManager();
        return m_replayManager;
    }

    static ReplayManager* getReplayManager() {
        return m_replayManager;
    }

    static void setMode(Mode mode) {
        m_mode = mode;
    }

    static Mode getMode() {
        return m_mode;
    }
};