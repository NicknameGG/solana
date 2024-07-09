#pragma once

#include "Solana/Click.hpp"
class FileManager {
public:
    static void save(std::string name, std::vector<Click> clicks) {
        if (clicks.empty()) {
            geode::log::warn("No clicks to save");
            return;
        }

        // Save as CSV
        std::ofstream file(name + ".csv");
        for (auto& click : clicks) {
            file << click.gameTick << "," << click.action << "\n";
        }
        file.close();

        geode::log::info("Saved {} clicks to {}", clicks.size(), name + ".csv");
    }

    static std::vector<Click> load(std::string name) {
        std::vector<Click> clicks;
        std::ifstream file(name + ".csv");
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


        geode::log::info("Loaded {} clicks from {}", clicks.size(), name + ".csv");

        return clicks;
    }

    static bool exists(std::string name) {
        std::ifstream file(name + ".csv");
        return file.good();
    }

    static std::string getFullPath(std::string name) {
        return name + ".csv";
    }
};