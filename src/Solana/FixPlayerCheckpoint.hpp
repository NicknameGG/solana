#pragma once

// Copied from: https://github.com/EclipseMenu/EclipseMenu/blob/main/src/hacks/Bot/PracticeFix.cpp

class FixPlayerCheckpoint {
public:
  FixPlayerCheckpoint() = default;

  FixPlayerCheckpoint(PlayerObject *player) {
    m_unk518 = player->m_unk518;
    m_unk51c = player->m_unk51c;
    m_unk51d = player->m_unk51d;
    m_unk51e = player->m_unk51e;
    m_unk51f = player->m_unk51f;
    m_unk568 = player->m_unk568;
    unk_584 = player->unk_584;
    unk_588 = player->unk_588;
    m_rotationSpeed = player->m_rotationSpeed;
    m_unk5dc = player->m_unk5dc;
    m_isRotating = player->m_isRotating;
    m_unk5e1 = player->m_unk5e1;
    m_hasGlow = player->m_hasGlow;
    m_isHidden = player->m_isHidden;
    m_hasGhostTrail = player->m_hasGhostTrail;
    m_unk62c = player->m_unk62c;
    m_unk630 = player->m_unk630;
    m_unk634 = player->m_unk634;
    m_unk638 = player->m_unk638;
    m_unk63c = player->m_unk63c;
    m_unk640 = player->m_unk640;
    m_unk644 = player->m_unk644;
    m_unk648 = player->m_unk648;
    m_unk658 = player->m_unk658;
    m_unk659 = player->m_unk659;
    m_unk65a = player->m_unk65a;
    m_unk65b = player->m_unk65b;
    m_playEffects = player->m_playEffects;
    m_unk65d = player->m_unk65d;
    m_unk65e = player->m_unk65e;
    m_unk65f = player->m_unk65f;
    m_unk688 = player->m_unk688;
    m_unk68c = player->m_unk68c;
    m_gv0096 = player->m_gv0096;
    m_gv0100 = player->m_gv0100;
    // m_unk6c0 = player->m_unk6c0;
    // m_unk328 = player->m_unk328;
    m_unk70c = player->m_unk70c;
    m_unk710 = player->m_unk710;
    m_playerStreak = player->m_playerStreak;
    m_hasCustomGlowColor = player->m_hasCustomGlowColor;
    m_isShip = player->m_isShip;
    m_isBird = player->m_isBird;
    m_isBall = player->m_isBall;
    m_isDart = player->m_isDart;
    m_isRobot = player->m_isRobot;
    m_isSpider = player->m_isSpider;
    m_isUpsideDown = player->m_isUpsideDown;
    m_isDead = player->m_isDead;
    m_isOnGround = player->m_isOnGround;
    m_isGoingLeft = player->m_isGoingLeft;
    m_isSideways = player->m_isSideways;
    m_isSwing = player->m_isSwing;
    m_unk7c8 = player->m_unk7c8;
    m_unk7cc = player->m_unk7cc;
    m_unk7d0 = player->m_unk7d0;
    m_isDashing = player->m_isDashing;
    m_vehicleSize = player->m_vehicleSize;
    m_playerSpeed = player->m_playerSpeed;
    m_unk7e0 = player->m_unk7e0;
    m_unk7e4 = player->m_unk7e4;
    m_unk7e8 = player->m_unk7e8;
    m_unk7f0 = player->m_unk7f0;
    m_unk7f8 = player->m_unk7f8;
    m_isLocked = player->m_isLocked;
    m_unka2b = player->m_unka2b;
    m_lastGroundedPos = player->m_lastGroundedPos;
    m_unk814 = player->m_unk814;
    m_unk815 = player->m_unk815;
    m_gamevar0060 = player->m_gamevar0060;
    m_gamevar0061 = player->m_gamevar0061;
    m_gamevar0062 = player->m_gamevar0062;
    m_unk838 = player->m_unk838;
    m_isPlatformer = player->m_isPlatformer;
    m_unk8ec = player->m_unk8ec;
    m_unk8f0 = player->m_unk8f0;
    m_unk8f4 = player->m_unk8f4;
    m_unk8f8 = player->m_unk8f8;
    m_gravityMod = player->m_gravityMod;
    m_unk904 = player->m_unk904;
    m_unk918 = player->m_unk918;
    m_unk91c = player->m_unk91c;
    m_unk948 = player->m_unk948;
    m_iconRequestID = player->m_iconRequestID;
    m_unk974 = player->m_unk974;
    m_unk978 = player->m_unk978;
    m_unk979 = player->m_unk979;
    m_unk97a = player->m_unk97a;
    m_unk97b = player->m_unk97b;

#ifndef GEODE_IS_ANDROID
    m_unk6a4 = player->m_unk6a4;
    m_unk828 = player->m_unk828;
#else
    // gd::set, gd::unordered_set and gd::unordered_map are just type aliases of
    // arrays of void* on android until that is fixed, this is a workaround

    std::copy(std::begin(player->m_unk6a4), std::end(player->m_unk6a4),
              std::begin(m_unk6a4));
    std::copy(std::begin(player->m_unk828), std::end(player->m_unk828),
              std::begin(m_unk828));
#endif
    m_unk880 = player->m_unk880;
    m_unk910 = player->m_unk910;

    m_xVelocity = player->m_platformerXVelocity;
    m_yVelocity = player->m_yVelocity;
    m_xPosition = player->m_position.x;
    m_yPosition = player->m_position.y;
    m_rotationSpeed = player->m_rotationSpeed;
    m_rotation = player->getRotation();
    m_wasOnSlope = player->m_wasOnSlope;
    m_isOnSlope = player->m_isOnSlope;
    m_lastSnappedTo = player->m_objectSnappedTo;
    m_unk3c0 = player->m_unk3c0;
    m_unk3c8 = player->m_unk3c8;
    m_unk3d0 = player->m_unk3d0;
    m_unk3e0 = player->m_unk3e0;
    m_unk3e1 = player->m_unk3e1;
    m_unk3e2 = player->m_unk3e2;
    m_unk3e3 = player->m_unk3e3;
    m_affectedByForces = player->m_affectedByForces;
    m_holdingRight = player->m_holdingRight;
    m_holdingLeft = player->m_holdingLeft;
    m_platformerVelocityRelated = player->m_platformerVelocityRelated;
    m_slopeRelated = player->m_slopeRelated;
    m_slopeVelocityRelated = player->m_slopeVelocityRelated;
  }

  void apply(PlayerObject *player) {
    player->m_unk518 = m_unk518;
    player->m_unk51c = m_unk51c;
    player->m_unk51d = m_unk51d;
    player->m_unk51e = m_unk51e;
    player->m_unk51f = m_unk51f;
    player->m_unk568 = m_unk568;
    player->unk_584 = unk_584;
    player->unk_588 = unk_588;
    player->m_rotationSpeed = m_rotationSpeed;
    player->m_unk5dc = m_unk5dc;
    player->m_isRotating = m_isRotating;
    player->m_unk5e1 = m_unk5e1;
    player->m_hasGlow = m_hasGlow;
    player->m_isHidden = m_isHidden;
    player->m_hasGhostTrail = m_hasGhostTrail;
    player->m_unk62c = m_unk62c;
    player->m_unk630 = m_unk630;
    player->m_unk634 = m_unk634;
    player->m_unk638 = m_unk638;
    player->m_unk63c = m_unk63c;
    player->m_unk640 = m_unk640;
    player->m_unk644 = m_unk644;
    player->m_unk648 = m_unk648;
    player->m_unk658 = m_unk658;
    player->m_unk659 = m_unk659;
    player->m_unk65a = m_unk65a;
    player->m_unk65b = m_unk65b;
    player->m_playEffects = m_playEffects;
    player->m_unk65d = m_unk65d;
    player->m_unk65e = m_unk65e;
    player->m_unk65f = m_unk65f;
    player->m_unk688 = m_unk688;
    player->m_unk68c = m_unk68c;
    player->m_gv0096 = m_gv0096;
    player->m_gv0100 = m_gv0100;
    // player->m_unk6c0 = m_unk6c0;
    // player->m_unk328 = m_unk328;
    player->m_unk70c = m_unk70c;
    player->m_unk710 = m_unk710;
    player->m_playerStreak = m_playerStreak;
    player->m_hasCustomGlowColor = m_hasCustomGlowColor;
    player->m_isShip = m_isShip;
    player->m_isBird = m_isBird;
    player->m_isBall = m_isBall;
    player->m_isDart = m_isDart;
    player->m_isRobot = m_isRobot;
    player->m_isSpider = m_isSpider;
    player->m_isUpsideDown = m_isUpsideDown;
    player->m_isDead = m_isDead;
    player->m_isOnGround = m_isOnGround;
    player->m_isGoingLeft = m_isGoingLeft;
    player->m_isSideways = m_isSideways;
    player->m_isSwing = m_isSwing;
    player->m_unk7c8 = m_unk7c8;
    player->m_unk7cc = m_unk7cc;
    player->m_unk7d0 = m_unk7d0;
    player->m_isDashing = m_isDashing;
    player->m_vehicleSize = m_vehicleSize;
    player->m_playerSpeed = m_playerSpeed;
    player->m_unk7e0 = m_unk7e0;
    player->m_unk7e4 = m_unk7e4;
    player->m_unk7e8 = m_unk7e8;
    player->m_unk7f0 = m_unk7f0;
    player->m_unk7f8 = m_unk7f8;
    player->m_isLocked = m_isLocked;
    player->m_unka2b = m_unka2b;
    player->m_lastGroundedPos = m_lastGroundedPos;
    player->m_unk814 = m_unk814;
    player->m_unk815 = m_unk815;
    player->m_gamevar0060 = m_gamevar0060;
    player->m_gamevar0061 = m_gamevar0061;
    player->m_gamevar0062 = m_gamevar0062;
    player->m_unk838 = m_unk838;
    player->m_isPlatformer = m_isPlatformer;
    player->m_unk8ec = m_unk8ec;
    player->m_unk8f0 = m_unk8f0;
    player->m_unk8f4 = m_unk8f4;
    player->m_unk8f8 = m_unk8f8;
    player->m_gravityMod = m_gravityMod;
    player->m_unk904 = m_unk904;
    player->m_unk918 = m_unk918;
    player->m_unk91c = m_unk91c;
    player->m_unk948 = m_unk948;
    player->m_iconRequestID = m_iconRequestID;
    player->m_unk974 = m_unk974;
    player->m_unk978 = m_unk978;
    player->m_unk979 = m_unk979;
    player->m_unk97a = m_unk97a;
    player->m_unk97b = m_unk97b;

    player->m_unk880 = m_unk880;
    player->m_unk910 = m_unk910;

    player->m_platformerXVelocity = m_xVelocity;
    player->m_yVelocity = m_yVelocity;
    player->setPositionX(m_xPosition);
    player->setPositionY(m_yPosition);
    // player->m_position.x = m_xPosition;
    // player->m_position.y = m_yPosition;
    player->setRotation(m_rotation);
    player->m_rotationSpeed = m_rotationSpeed;
    player->m_wasOnSlope = m_wasOnSlope;
    player->m_isOnSlope = m_isOnSlope;
    player->m_objectSnappedTo = m_lastSnappedTo;
    player->m_unk3c0 = m_unk3c0;
    player->m_unk3c8 = m_unk3c8;
    player->m_unk3d0 = m_unk3d0;
    player->m_unk3e0 = m_unk3e0;
    player->m_unk3e1 = m_unk3e1;
    player->m_unk3e2 = m_unk3e2;
    player->m_unk3e3 = m_unk3e3;
    player->m_affectedByForces = m_affectedByForces;
    player->m_holdingRight = m_holdingRight;
    player->m_holdingLeft = m_holdingLeft;
    player->m_platformerVelocityRelated = m_platformerVelocityRelated;
    player->m_slopeRelated = m_slopeRelated;
    player->m_slopeVelocityRelated = m_slopeVelocityRelated;
  }

  void logall() {
  geode::log::info("m_unk518: {}", m_unk518);
  geode::log::info("m_unk51c: {}", m_unk51c);
  geode::log::info("m_unk51d: {}", m_unk51d);
  geode::log::info("m_unk51e: {}", m_unk51e);
  geode::log::info("m_unk51f: {}", m_unk51f);
  geode::log::info("m_unk568: {}", m_unk568);
  geode::log::info("unk_584: {}", unk_584);
  geode::log::info("unk_588: {}", unk_588);
  geode::log::info("m_rotationSpeed: {}", m_rotationSpeed);
  geode::log::info("m_unk5dc: {}", m_unk5dc);
  geode::log::info("m_isRotating: {}", m_isRotating);
  geode::log::info("m_unk5e1: {}", m_unk5e1);
  geode::log::info("m_hasGlow: {}", m_hasGlow);
  geode::log::info("m_isHidden: {}", m_isHidden);
  geode::log::info("m_hasGhostTrail: {}", m_hasGhostTrail);
  geode::log::info("m_unk62c: {}", m_unk62c);
  geode::log::info("m_unk630: {}", m_unk630);
  geode::log::info("m_unk634: {}", m_unk634);
  geode::log::info("m_unk638: {}", m_unk638);
  geode::log::info("m_unk63c: {}", m_unk63c);
  geode::log::info("m_unk640: {}", m_unk640);
  geode::log::info("m_unk644: {}", m_unk644);
  geode::log::info("m_unk648: {}", m_unk648);
  geode::log::info("m_unk658: {}", m_unk658);
  geode::log::info("m_unk659: {}", m_unk659);
  geode::log::info("m_unk65a: {}", m_unk65a);
  geode::log::info("m_unk65b: {}", m_unk65b);
  geode::log::info("m_playEffects: {}", m_playEffects);
  geode::log::info("m_unk65d: {}", m_unk65d);
  geode::log::info("m_unk65e: {}", m_unk65e);
  geode::log::info("m_unk65f: {}", m_unk65f);
  geode::log::info("m_unk688: {}", m_unk688);
  geode::log::info("m_unk68c: {}", m_unk68c);
  geode::log::info("m_gv0096: {}", m_gv0096);
  geode::log::info("m_gv0100: {}", m_gv0100);
  // geode::log::info("m_unk6c0: {}", m_unk6c0);
  // geode::log::info("m_unk328: {}", m_unk328);
  geode::log::info("m_unk70c: {}", m_unk70c);
  geode::log::info("m_unk710: {}", m_unk710);
  geode::log::info("m_playerStreak: {}", m_playerStreak);
  geode::log::info("m_hasCustomGlowColor: {}", m_hasCustomGlowColor);
  geode::log::info("m_isShip: {}", m_isShip);
  geode::log::info("m_isBird: {}", m_isBird);
  geode::log::info("m_isBall: {}", m_isBall);
  geode::log::info("m_isDart: {}", m_isDart);
  geode::log::info("m_isRobot: {}", m_isRobot);
  geode::log::info("m_isSpider: {}", m_isSpider);
  geode::log::info("m_isUpsideDown: {}", m_isUpsideDown);
  geode::log::info("m_isDead: {}", m_isDead);
  geode::log::info("m_isOnGround: {}", m_isOnGround);
  geode::log::info("m_isGoingLeft: {}", m_isGoingLeft);
  geode::log::info("m_isSideways: {}", m_isSideways);
  geode::log::info("m_isSwing: {}", m_isSwing);
  geode::log::info("m_unk7c8: {}", m_unk7c8);
  geode::log::info("m_unk7cc: {}", m_unk7cc);
  geode::log::info("m_unk7d0: {}", m_unk7d0);
  geode::log::info("m_isDashing: {}", m_isDashing);
  geode::log::info("m_vehicleSize: {}", m_vehicleSize);
  geode::log::info("m_playerSpeed: {}", m_playerSpeed);
  geode::log::info("m_unk7e0: {}", m_unk7e0);
  geode::log::info("m_unk7e4: {}", m_unk7e4);
  geode::log::info("m_unk7e8: {}", m_unk7e8);
  geode::log::info("m_unk7f0: {}", m_unk7f0);
  geode::log::info("m_unk7f8: {}", m_unk7f8);
  geode::log::info("m_isLocked: {}", m_isLocked);
  geode::log::info("m_unka2b: {}", m_unka2b);
  geode::log::info("m_lastGroundedPos: {}", m_lastGroundedPos);
  geode::log::info("m_unk814: {}", m_unk814);
  geode::log::info("m_unk815: {}", m_unk815);
  geode::log::info("m_gamevar0060: {}", m_gamevar0060);
  geode::log::info("m_gamevar0061: {}", m_gamevar0061);
  geode::log::info("m_gamevar0062: {}", m_gamevar0062);
  geode::log::info("m_unk838: {}", m_unk838);
  geode::log::info("m_isPlatformer: {}", m_isPlatformer);
  geode::log::info("m_unk8ec: {}", m_unk8ec);
  geode::log::info("m_unk8f0: {}", m_unk8f0);
  geode::log::info("m_unk8f4: {}", m_unk8f4);
  geode::log::info("m_unk8f8: {}", m_unk8f8);
  geode::log::info("m_gravityMod: {}", m_gravityMod);
  geode::log::info("m_unk904: {}", m_unk904);
  geode::log::info("m_unk918: {}", m_unk918);
  geode::log::info("m_unk91c: {}", m_unk91c);
  geode::log::info("m_unk948: {}", m_unk948);
  geode::log::info("m_iconRequestID: {}", m_iconRequestID);
  geode::log::info("m_unk974: {}", m_unk974);
  geode::log::info("m_unk978: {}", m_unk978);
  geode::log::info("m_unk979: {}", m_unk979);
  geode::log::info("m_unk97a: {}", m_unk97a);
  geode::log::info("m_unk97b: {}", m_unk97b);

  geode::log::info("m_unk880: {}", m_unk880);
  geode::log::info("m_unk910: {}", m_unk910);

  geode::log::info("m_xVelocity: {}", m_xVelocity);
  geode::log::info("m_yVelocity: {}", m_yVelocity);
  geode::log::info("m_xPosition: {}", m_xPosition);
  geode::log::info("m_yPosition: {}", m_yPosition);
  geode::log::info("m_rotation: {}", m_rotation);
  geode::log::info("m_rotationSpeed: {}", m_rotationSpeed);
  geode::log::info("m_wasOnSlope: {}", m_wasOnSlope);
  geode::log::info("m_isOnSlope: {}", m_isOnSlope);
  geode::log::info("m_lastSnappedTo: {}", m_lastSnappedTo);
  geode::log::info("m_unk3c0: {}", m_unk3c0);
  geode::log::info("m_unk3c8: {}", m_unk3c8);
  geode::log::info("m_unk3d0: {}", m_unk3d0);
  geode::log::info("m_unk3e0: {}", m_unk3e0);
  geode::log::info("m_unk3e1: {}", m_unk3e1);
  geode::log::info("m_unk3e2: {}", m_unk3e2);
  geode::log::info("m_unk3e3: {}", m_unk3e3);
  geode::log::info("m_affectedByForces: {}", m_affectedByForces);
  geode::log::info("m_holdingRight: {}", m_holdingRight);
  geode::log::info("m_holdingLeft: {}", m_holdingLeft);
  geode::log::info("m_platformerVelocityRelated: {}", m_platformerVelocityRelated);
  geode::log::info("m_slopeRelated: {}", m_slopeRelated);
  geode::log::info("m_slopeVelocityRelated: {}", m_slopeVelocityRelated);
}

private:
  float m_unk518;
  bool m_unk51c;
  bool m_unk51d;
  bool m_unk51e;
  bool m_unk51f;
  float m_unk568;
  float unk_584;
  int unk_588;
  float m_rotationSpeed;
  float m_unk5dc;
  bool m_isRotating;
  bool m_unk5e1;
  bool m_hasGlow;
  bool m_isHidden;
  int m_hasGhostTrail;
  float m_unk62c;
  int m_unk630;
  float m_unk634;
  int m_unk638;
  float m_unk63c;
  int m_unk640;
  float m_unk644;
  float m_unk648;
  bool m_unk658;
  bool m_unk659;
  bool m_unk65a;
  bool m_unk65b;
  bool m_playEffects;
  bool m_unk65d;
  bool m_unk65e;
  bool m_unk65f;
  float m_unk688;
  float m_unk68c;
  bool m_gv0096;
  bool m_gv0100;
  CheckpointObject *m_unk6c0;
  int m_unk328;
  float m_unk70c;
  float m_unk710;
  int m_playerStreak;
  bool m_hasCustomGlowColor;
  bool m_isShip;
  bool m_isBird;
  bool m_isBall;
  bool m_isDart;
  bool m_isRobot;
  bool m_isSpider;
  bool m_isUpsideDown;
  bool m_isDead;
  bool m_isOnGround;
  bool m_isGoingLeft;
  bool m_isSideways;
  bool m_isSwing;
  float m_unk7c8;
  float m_unk7cc;
  float m_unk7d0;
  bool m_isDashing;
  float m_vehicleSize;
  float m_playerSpeed;
  float m_unk7e0;
  float m_unk7e4;
  cocos2d::CCPoint m_unk7e8;
  cocos2d::CCPoint m_unk7f0;
  bool m_unk7f8;
  bool m_isLocked;
  bool m_unka2b;
  cocos2d::CCPoint m_lastGroundedPos;
  bool m_unk814;
  bool m_unk815;
  bool m_gamevar0060;
  bool m_gamevar0061;
  bool m_gamevar0062;
  float m_unk838;
  bool m_isPlatformer;
  int m_unk8ec;
  int m_unk8f0;
  int m_unk8f4;
  int m_unk8f8;
  float m_gravityMod;
  cocos2d::CCPoint m_unk904;
  float m_unk918;
  float m_unk91c;
  bool m_unk948;
  int m_iconRequestID;
  float m_unk974;
  bool m_unk978;
  bool m_unk979;
  bool m_unk97a;
  bool m_unk97b;

  gd::unordered_set<int> m_unk6a4;
  gd::unordered_set<int> m_unk828;
  gd::vector<float> m_unk880;
  gd::map<int, bool> m_unk910;
  // gd::map<int, bool> m_unk924; // THE CULPRIT

  double m_xVelocity;
  double m_yVelocity;
  float m_xPosition;
  float m_yPosition;
  float m_rotation;
  double m_unk3c0;
  double m_unk3c8;
  double m_unk3d0;
  bool m_unk3e0;
  bool m_unk3e1;
  bool m_unk3e2;
  bool m_unk3e3;
  float m_platformerVelocityRelated;
  float m_slopeVelocityRelated;

  GameObject *m_lastSnappedTo = nullptr;
  GameObject *m_lastSnappedTo2 = nullptr;

  bool m_isOnSlope;
  bool m_wasOnSlope;
  bool m_affectedByForces;
  bool m_holdingRight;
  bool m_holdingLeft;
  bool m_slopeRelated;
};