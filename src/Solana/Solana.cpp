#include <Solana/Solana.hpp>

TrainManager* Solana::m_trainManager = nullptr;
ReplayManager* Solana::m_replayManager = nullptr;
Mode Solana::m_mode = Mode::Replay;