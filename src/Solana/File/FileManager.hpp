#pragma once

#include "Solana/Click.hpp"

class FileManager {
public:
    static void save(std::string name, std::vector<Click> clicks) {
        // Create folder if it doesn't exist
        auto folder = geode::Mod::get()->getSaveDir() / "macros";
        
        if (!std::filesystem::exists(folder)) {
            std::filesystem::create_directory(folder);
        }

        // Hardcoded path to save directory
        auto path = geode::Mod::get()->getSaveDir() / "macros" / std::string(name + ".csv");

        if (clicks.empty()) {
            geode::log::warn("No clicks to save");
            return;
        }

        // Save as CSV
        std::ofstream file(path);
        for (auto& click : clicks) {
            file << click.gameTick << "," << click.action << "\n";
        }
        file.close();

        geode::log::info("Saved {} clicks to {}", clicks.size(), path);
    }

    static std::vector<Click> load(std::string name) {
        auto path = getFullPath(name);

        std::vector<Click> clicks;
        std::ifstream file(path);
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string gameTick;
            std::string action;
            std::getline(ss, gameTick, ',');
            std::getline(ss, action, ',');
            clicks.push_back(Click(std::stoi(gameTick), (PlayerAction)std::stoi(action)));
        }
        file.close();


        geode::log::info("Loaded {} clicks from {}", clicks.size(), path);

        return clicks;
    }

    static bool exists(std::string name) {
        auto path = getFullPath(name);

        std::ifstream file(path);
        return file.good();
    }

    static std::string getFullPath(std::string name) {

        // Backward compatibility, check if the file exists in the old directory
        auto oldPath = geode::dirs::getGameDir() / std::string(name + ".csv");
        if (std::ifstream(oldPath).good()) {
            return oldPath.string();
        }

        auto path = geode::Mod::get()->getSaveDir() / "macros" / std::string(name + ".csv");
        return path.string();
    }
};