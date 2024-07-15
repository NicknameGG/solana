/* Refactor 2, hopefully this will be a better version of the bot lol */
#include <Geode/Geode.hpp>
#include <Geode/loader/SettingEvent.hpp>

#include <Geode/modify/EditLevelLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/UILayer.hpp>

#include <Solana/UI/SolanaUI.hpp>
#include <Solana/UI/PrematureDeathNotification.hpp>
#include <Solana/Solana.hpp>
#include <Solana/File/FileManager.hpp>

using namespace geode::prelude;

class $modify(UILayer) {
    void keyDown(cocos2d::enumKeyCodes p0) {
        if (p0 == cocos2d::enumKeyCodes::KEY_K) {
            // Get parent playlayer
            auto parent = dynamic_cast<PlayLayer*>(this->getParent());
            if (!parent) {
                UILayer::keyDown(p0);
                return;
            }

            parent->m_gameState.m_timeWarp = 0.25f;
        }

        UILayer::keyDown(p0);
    }
};

class $modify(PlayerObject) {
    void collidedWithObjectInternal(float p0, GameObject* p1, cocos2d::CCRect p2, bool p3) {
        if (Solana::getMode() == Mode::Replay || Solana::getMode() == Mode::Nothing) {
            PlayerObject::collidedWithObjectInternal(p0, p1, p2, p3);
            return;
        }

        // Find which player collided and set the global play layer
        auto player = Solana::getTrainManager()->getPlayer(this->m_uID);
        // Probably 2nd player
        if (!player) {
            PlayerObject::collidedWithObjectInternal(p0, p1, p2, p3);
            return;
        }

        auto old_playlayer = GameManager::get()->m_playLayer;

        GameManager::get()->m_playLayer = player->m_playLayer;
        PlayerObject::collidedWithObjectInternal(p0, p1, p2, p3);

        // Revert back to old play layer
        GameManager::get()->m_playLayer = old_playlayer;
    }

    void collidedWithSlopeInternal(float p0, GameObject* p1, bool p2) {
        if (Solana::getMode() == Mode::Replay || Solana::getMode() == Mode::Nothing) {
            PlayerObject::collidedWithSlopeInternal(p0, p1, p2);
            return;
        }

        // Find which player collided and set the global play layer
        auto player = Solana::getTrainManager()->getPlayer(this->m_uID);
        // Probably 2nd player
        if (!player) {
            PlayerObject::collidedWithSlopeInternal(p0, p1, p2);
            return;
        }

        auto old_playlayer = GameManager::get()->m_playLayer;

        GameManager::get()->m_playLayer = player->m_playLayer;
        PlayerObject::collidedWithSlopeInternal(p0, p1, p2);

        // Revert back to old play layer
        GameManager::get()->m_playLayer = old_playlayer;
    }

    void loadFromCheckpoint(PlayerCheckpoint* obj) {
        if (Solana::getMode() == Mode::Replay || Solana::getMode() == Mode::Nothing) {
            PlayerObject::loadFromCheckpoint(obj);
            return;
        }

        // Find which player collided and set the global play layer
        auto player = Solana::getTrainManager()->getPlayer(this->m_uID);
        // Probably 2nd player
        if (!player) {
            PlayerObject::loadFromCheckpoint(obj);
            return;
        }

        auto old_playlayer = GameManager::get()->m_playLayer;

        GameManager::get()->m_playLayer = player->m_playLayer;
        PlayerObject::loadFromCheckpoint(obj);

        // Revert back to old play layer
        GameManager::get()->m_playLayer = old_playlayer;
    
    }
};


std::vector<std::string> addreses_checked;

class $modify(GJBaseGameLayer) {
    void toggleFlipped(bool p0, bool p1) {
        if (Solana::getMode() == Mode::Replay || Solana::getMode() == Mode::Nothing) {
            GJBaseGameLayer::toggleFlipped(p0, p1);
            return;
        }

        // Nothing
    }

    void processCommands(float dt) {
        if (Solana::getMode() == Mode::Nothing) {
            GJBaseGameLayer::processCommands(dt);
            return;
        }

        if (Solana::getMode() == Mode::Replay) {
            if (this->m_player1->m_isDead) {
                GJBaseGameLayer::processCommands(dt);
                return;
            }

            if (!Solana::getReplayManager()) {
                geode::log::error("Replay manager is null");
                GJBaseGameLayer::processCommands(dt);
                return;
            }

            // Update the replay manager
            auto click = Solana::getReplayManager()->digest(this->m_gameState.m_currentProgress);

            if (click.action == PlayerAction::Hold) {
                this->handleButton(true, 1, true);
            }
            else if (click.action == PlayerAction::Release) {
                this->handleButton(false, 1, true);
            }

            GJBaseGameLayer::processCommands(dt);
            return;
        }

        if (Solana::getTrainManager()->isFirst(this->m_player1->m_uID)) {
            // Update the train manager
            Solana::getTrainManager()->update();
        }
        
        // Don't process if player is dead
        if (this->m_player1->m_isDead) {
            GJBaseGameLayer::processCommands(dt);
            return;
        }

        // Try digesting the click
        auto player = Solana::getTrainManager()->getPlayer(this->m_player1->m_uID);
        if (!player) {
            GJBaseGameLayer::processCommands(dt);
            return;
        }

        auto click = player->digest();

        // Process the click
        if (click.action == PlayerAction::Hold) {
            this->handleButton(true, 1, true);
        }
        else if (click.action == PlayerAction::Release) {
            this->handleButton(false, 1, true);
        }

        GJBaseGameLayer::processCommands(dt);
    }
    // // removeObjectFromSection crash fix; unstable as hell, use at your own risk
    // void removeObjectFromSection(GameObject *p0) {
    //     if (Solana::getMode() == Mode::Replay || Solana::getMode() == Mode::Nothing) {
    //         GJBaseGameLayer::removeObjectFromSection(p0);
    //         return;
    //     }
    //     /*
    //                 m_outerSectionIndex = p0->m_outerSectionIndex;
    //     if ( m_outerSectionIndex >= 0 )
    //     {
    //         m_middleSectionIndex = p0->m_middleSectionIndex;
    //         middleSection = *(*this->m_sections._Mypair._Myval2._Myfirst[m_outerSectionIndex] + 8 * m_middleSectionIndex);
    //         sectionSizes = this->m_sectionSizes._Mypair._Myval2._Myfirst[m_outerSectionIndex]->_Mypair._Myval2._Myfirst;
    //         currentSize = sectionSizes[m_middleSectionIndex];
    //         sectionSizes[m_middleSectionIndex] = currentSize - 1;
    //         *(*middleSection + 8i64 * p0->m_someOtherIndex) = *(*middleSection + 8i64 * currentSize - 8); <- Get this for crash check
    //         *(*(*middleSection + 8i64 * p0->m_someOtherIndex) + 624i64) = p0->m_someOtherIndex;
    //     */

    //     auto m_outerSectionIndex = p0->m_outerSectionIndex;
    //     if (m_outerSectionIndex >= 0)
    //     {
    //         auto m_middleSectionIndex = p0->m_middleSectionIndex;
            
    //         if (!this->m_sections[m_outerSectionIndex]) {
    //             // geode::log::error("Invalid size: {} > {}", m_outerSectionIndex, this->m_sections.size());
    //             return;
    //         }

    //         auto middleSection = (*this->m_sections[m_outerSectionIndex])[m_middleSectionIndex];

    //         if (!middleSection) {
    //             // geode::log::error("Invalid size: {} > {}", m_middleSectionIndex, middleSection.size());
    //             return;
    //         }

    //         if ((unsigned long long)&*middleSection >= 0x1000000000000000) {
    //             return;
    //         }
            
    //         auto sectionSizes = *this->m_sectionSizes[m_outerSectionIndex];
    //         auto currentSize = sectionSizes[m_middleSectionIndex];

    //         if (p0->m_someOtherIndex >= middleSection->size()) {
    //             // geode::log::error("Invalid size: {} > {}", p0->m_someOtherIndex, middleSection.size());
    //             return;
    //         }
            
    //         // // Move the last element to the position of p0->m_someOtherIndex
    //         // // middleSection[p0->m_someOtherIndex] = middleSection[currentSize];
    //         // // geode::log::info("Is alive: {}", middleSection[currentSize].empty());
    //         if (currentSize - 1 >= middleSection->size()) {
    //             // auto x = middleSection[currentSize - 1];
    //             // geode::log::error("Invalid size: {} > {}", currentSize - 1, middleSection.size());
    //             return;
    //         }

    //         /*
    //             middle = p0->m_middleSectionIndex;
    //             outer = p0->m_outerSectionIndex;
    //             v21 = this->m_unkSectionTest2._Mypair._Myval2._Myfirst[outer]->_Mypair._Myval2._Myfirst;
    //             v22 = v21[middle];
    //             v21[middle] = v22 - 1;
    //             v20->_Mypair._Myval2._Myfirst[p0->m_innerSectionIndex] = v20->_Mypair._Myval2._Myfirst[v22 - 1];
    //             *(v20->_Mypair._Myval2._Myfirst[p0->m_innerSectionIndex] + 157) = p0->m_innerSectionIndex;
    //          */
    //         if ( p0->m_objectType != GameObjectType::Decoration ) {
    //             if (!p0->m_hasExtendedCollision) {
    //                 auto m_middleSectionIndex = p0->m_middleSectionIndex;
                    
    //                 if (!this->m_sectionUnkTest[m_outerSectionIndex]) {
    //                     return;
    //                 }

    //                 auto middleSection = (*this->m_sectionUnkTest[m_outerSectionIndex])[m_middleSectionIndex];

    //                 if (!middleSection || middleSection == nullptr) {
    //                     return;
    //                 }

    //                 if ((unsigned long long)&*middleSection >= 0x1000000000000000) {
    //                     return;
    //                 }
                    
    //                 auto sectionSizes = *this->m_unkSectionTest2[m_outerSectionIndex];
    //                 auto currentSize = sectionSizes[m_middleSectionIndex];

    //                 // sectionSizes[m_middleSectionIndex] = t - 1;

    //                 // auto size_check = middleSection->size();

    //                 // if (p0 && p0->m_innerSectionIndex >= size_check) {
    //                 //     return;
    //                 // }

                    
                    
    //                 if (currentSize - 1 >= middleSection->size()) {
    //                     return;
    //                 }
    //             }
    //         }
    //     }

    //     GJBaseGameLayer::removeObjectFromSection(p0);
    // }
};

class $modify(PlayLayer) {
    void createObjectsFromSetupFinished() {
        // Call original add object if in replay mode
        if (Solana::getMode() == Mode::Replay || Solana::getMode() == Mode::Nothing) {
            PlayLayer::createObjectsFromSetupFinished();
            return;
        }

        for (auto object : CCArrayExt<GameObject*>(this->m_objects)) {
            // Set properties
            object->m_hasNoEffects = true;
            object->m_isHide = true;
            object->m_hasNoParticles = true;

            if (object->m_objectID == 45) {
                object->m_isNoTouch = true;
            }
        }

        // Add object
        PlayLayer::createObjectsFromSetupFinished();

    }

    void destroyPlayer(PlayerObject* player, GameObject* noClipSpike) {
        if (Solana::getMode() == Mode::Nothing) {
            PlayLayer::destroyPlayer(player, noClipSpike);
            return;
        }

        // Detect No Clip spike
        if (noClipSpike) {
            if (this->m_unk3688) {
				if (noClipSpike->m_uniqueID == this->m_unk3688->m_uniqueID) {
                    // Yeahhh, I have no idea.
                    int v7 = *reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(this) + 13024);
                    *reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(this) + 14040) = v7;
                    float v8 = (rand() / 32767.0) * 1000.0;
                    *reinterpret_cast<float*>(reinterpret_cast<std::uintptr_t>(this) + 14036i64) = v8;
                    *reinterpret_cast<float*>(reinterpret_cast<std::uintptr_t>(this) + 14032i64) = v7 + v8;
                    *reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(this) + 14044i64) = 1;
                    return;
				}
			}
        }

        // Skip if player matches second player's id
        if (this->m_player2 && this->m_player2->m_uID == player->m_uID) {
            return;
        }

        if (Solana::getMode() == Mode::Replay) {
            if (!player->m_isDead) {
                // Player died, just set to dead
                player->m_isDead = true;
                // Check if player died before the last tick
                auto lastTick = Solana::getReplayManager()->m_clicks.back().gameTick;
                if (this->m_gameState.m_currentProgress < lastTick) {
                    geode::log::error("Player died before expected. Expected: {}, Actual: {}", lastTick, this->m_gameState.m_currentProgress);
                    PrematureDeathNotification::create("Player died before expected")->show();
                }
            }

            return;
        }

        // Player died
        auto plr = Solana::getTrainManager()->getPlayer(player->m_uID);
        if (!plr) {
            return;
        }

        if (!player->m_isDead) {
            if (this->m_player1->m_uID != player->m_uID) {
                geode::log::warn("IDs don't match: {} != {}", this->m_player1->m_uID, player->m_uID);
                // Crash to see stacktrace
                int* crash = nullptr;
                *crash = 1;
                
                // Kill both for good measure
                auto selfPlr = Solana::getTrainManager()->getPlayer(this->m_player1->m_uID);

                selfPlr->die(this->m_gameState.m_currentProgress);
                selfPlr->m_isDead = true;
            }

            plr->die(this->m_gameState.m_currentProgress);
            player->m_isDead = true;
        }
    }

    void updateVisibility(float dt) {
        // Call original update visibility if in replay mode
        if (Solana::getMode() == Mode::Replay || Solana::getMode() == Mode::Nothing) {
            PlayLayer::updateVisibility(dt);
            return;
        }

        // // Update global play layer
        // auto old_playlayer = GameManager::get()->m_playLayer;
        // GameManager::get()->m_playLayer = this;

        // // Update visibility
        // PlayLayer::updateVisibility(dt);

        // // Revert back to old play layer
        // GameManager::get()->m_playLayer = old_playlayer;
    }

    void resetLevel() {
        if (Solana::getMode() == Mode::Replay || Solana::getMode() == Mode::Nothing) {
            // Call original reset level if in replay mode
            PlayLayer::resetLevel();
            return;
        }

        auto player = Solana::getTrainManager()->getPlayer(this->m_player1->m_uID);
        if (!player) {
            PlayLayer::resetLevel();
            return;
        }


        auto resetData = player->consumeResetData();

        if (resetData.m_checkpoint) {
            // Set global play layer
            auto old_playlayer = GameManager::get()->m_playLayer;
            GameManager::get()->m_playLayer = this;

            // Add checkpoint to array
            this->storeCheckpoint(resetData.m_checkpoint);

            PlayLayer::resetLevel();

            // this->m_checkpointArray->removeObject(resetData.m_checkpoint, true);

            // Set timewarp
            this->m_gameState.m_timeWarp = Mod::get()->getSettingValue<double>("timewarp");

            if (Solana::getMode() == Mode::TrainFromReplay) {
                if (this->m_player1->m_position.x < 2) {
                    // Set timewarp
                    this->m_gameState.m_timeWarp = 5.0f;
                }
            }

            resetData.m_fixPlayerCheckpoint->apply(this->m_player1);
            this->m_gameState = resetData.m_checkpoint->m_gameState;

            // Farthest death tick > 1000
            // if (Solana::getTrainManager()->getPlayer(this->m_player1->m_uID)->m_farthestDeathTick > 761) {
            //     resetData.m_fixPlayerCheckpoint->logall();
            //     exit(0);
            // }


            // this->m_isPracticeMode = false;

            // Revert back to old play layer
            GameManager::get()->m_playLayer = old_playlayer;
        }
        else {
            // geode::log::warn("No checkpoint found for player: {}", this->m_player1->m_uID);
            // Reset level
            PlayLayer::resetLevel();

            // Set timewarp
            this->m_gameState.m_timeWarp = Mod::get()->getSettingValue<double>("timewarp");
        }
    }

    void levelComplete() {
        // Call original level complete if in replay mode
        if (Solana::getMode() == Mode::Replay || Solana::getMode() == Mode::Nothing) {
            PlayLayer::levelComplete();
            return;
        }

        // Save to CSV file
        auto layer = Solana::getTrainManager()->getPlayer(this->m_player1->m_uID);

        geode::log::info("Training complete");

        // Save to CSV
        FileManager::save(this->m_level->m_levelName, layer->m_clicks);

        // Set global
        auto old_playlayer = GameManager::get()->m_playLayer;
        GameManager::get()->m_playLayer = this;

        PlayLayer::levelComplete();

        // Revert back to old play layer
        GameManager::get()->m_playLayer = old_playlayer;

        Solana::getTrainManager()->finishTraining();
    }

    void onQuit() {
        if (Solana::getMode() == Mode::Replay || Solana::getMode() == Mode::Nothing) {
            // Call original on quit if in replay mode
            PlayLayer::onQuit();
            return;
        }

        Solana::setMode(Mode::Nothing);

        geode::log::info("Training complete");

        // Get best brain
        auto best = Solana::getTrainManager()->getBestBrain(false);
        // Save to file
        FileManager::save(this->m_level->m_levelName, best->m_clicks);

        // Set to replay mode
        PlayLayer::onQuit();

        Solana::setMode(Mode::Replay);
    }

    void postUpdate(float dt) {
        // Call original post update if in replay mode
        if (Solana::getMode() == Mode::Replay || Solana::getMode() == Mode::Nothing) {
            PlayLayer::postUpdate(dt);
            return;
        }

        auto layer = Solana::getTrainManager()->getPlayer(this->m_player1->m_uID);
        if (layer->m_reset) {
            this->resetLevel();
            layer->m_reset = false;
        }

        auto old_playlayer = GameManager::get()->m_playLayer;
        GameManager::get()->m_playLayer = this;

        PlayLayer::postUpdate(dt);

        GameManager::get()->m_playLayer = old_playlayer;
    }
};

class $modify(MyEditLevelLayer, EditLevelLayer) {
    void openSolanaUI(CCObject*) {
        auto ui = SolanaUI::create(this->m_level);
        ui->show();
    }

    bool init(GJGameLevel* level) {
		if (!EditLevelLayer::init(level)) return false;

        auto menu = this->getChildByID("level-actions-menu");

        auto solana_ui = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_ncsLibraryBtn_001.png"),
            this,
            menu_selector(MyEditLevelLayer::openSolanaUI)
        );

        menu->addChild(solana_ui);

		menu->updateLayout();

        return true;
    }
};