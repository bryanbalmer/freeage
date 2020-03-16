#include "FreeAge/client/unit.hpp"

#include <cmath>

#include "FreeAge/common/logging.hpp"
#include "FreeAge/client/map.hpp"

bool ClientUnitType::Load(UnitType type, const std::filesystem::path& graphicsPath, const std::filesystem::path& cachePath, const Palettes& palettes) {
  animations.resize(static_cast<int>(UnitAnimation::NumAnimationTypes));
  
  // TODO: Automatically check how many animations (A, B) for each type are available.
  
  std::filesystem::path ingameUnitsPath =
      graphicsPath.parent_path().parent_path().parent_path().parent_path() / "widgetui" / "textures" / "ingame" / "units";
  
  bool ok = true;
  switch (type) {
  case UnitType::FemaleVillager:
    animations[static_cast<int>(UnitAnimation::Idle)].resize(1);
    ok = ok && LoadAnimation(0, "u_vil_female_villager_idleA_x1.smx", graphicsPath, cachePath, palettes, UnitAnimation::Idle);
    animations[static_cast<int>(UnitAnimation::Walk)].resize(1);
    ok = ok && LoadAnimation(0, "u_vil_female_villager_walkA_x1.smx", graphicsPath, cachePath, palettes, UnitAnimation::Walk);
    iconTexture.Load(ingameUnitsPath / "016_50730.DDS", GL_CLAMP, GL_LINEAR, GL_LINEAR);
    break;
  case UnitType::FemaleVillagerBuilder:
    animations[static_cast<int>(UnitAnimation::Idle)].resize(1);
    ok = ok && LoadAnimation(0, "u_vil_female_builder_idleA_x1.smx", graphicsPath, cachePath, palettes, UnitAnimation::Idle);
    animations[static_cast<int>(UnitAnimation::Walk)].resize(1);
    ok = ok && LoadAnimation(0, "u_vil_female_builder_walkA_x1.smx", graphicsPath, cachePath, palettes, UnitAnimation::Walk);
    animations[static_cast<int>(UnitAnimation::Task)].resize(1);
    ok = ok && LoadAnimation(0, "u_vil_female_builder_taskA_x1.smx", graphicsPath, cachePath, palettes, UnitAnimation::Task);
    iconTexture.Load(ingameUnitsPath / "016_50730.DDS", GL_CLAMP, GL_LINEAR, GL_LINEAR);  // TODO: Do not load the icon multiple times
    break;
  case UnitType::MaleVillager:
    animations[static_cast<int>(UnitAnimation::Idle)].resize(1);
    ok = ok && LoadAnimation(0, "u_vil_male_villager_idleA_x1.smx", graphicsPath, cachePath, palettes, UnitAnimation::Idle);
    animations[static_cast<int>(UnitAnimation::Walk)].resize(1);
    ok = ok && LoadAnimation(0, "u_vil_male_villager_walkA_x1.smx", graphicsPath, cachePath, palettes, UnitAnimation::Walk);
    iconTexture.Load(ingameUnitsPath / "015_50730.DDS", GL_CLAMP, GL_LINEAR, GL_LINEAR);
    break;
  case UnitType::MaleVillagerBuilder:
    animations[static_cast<int>(UnitAnimation::Idle)].resize(1);
    ok = ok && LoadAnimation(0, "u_vil_male_builder_idleA_x1.smx", graphicsPath, cachePath, palettes, UnitAnimation::Idle);
    animations[static_cast<int>(UnitAnimation::Walk)].resize(1);
    ok = ok && LoadAnimation(0, "u_vil_male_builder_walkA_x1.smx", graphicsPath, cachePath, palettes, UnitAnimation::Walk);
    animations[static_cast<int>(UnitAnimation::Task)].resize(1);
    ok = ok && LoadAnimation(0, "u_vil_male_builder_taskA_x1.smx", graphicsPath, cachePath, palettes, UnitAnimation::Task);
    iconTexture.Load(ingameUnitsPath / "015_50730.DDS", GL_CLAMP, GL_LINEAR, GL_LINEAR);  // TODO: Do not load the icon multiple times
    break;
  case UnitType::Militia:
    animations[static_cast<int>(UnitAnimation::Idle)].resize(2);
    ok = ok && LoadAnimation(0, "u_inf_militia_idleA_x1.smx", graphicsPath, cachePath, palettes, UnitAnimation::Idle);
    ok = ok && LoadAnimation(1, "u_inf_militia_idleB_x1.smx", graphicsPath, cachePath, palettes, UnitAnimation::Idle);
    animations[static_cast<int>(UnitAnimation::Walk)].resize(1);
    ok = ok && LoadAnimation(0, "u_inf_militia_walkA_x1.smx", graphicsPath, cachePath, palettes, UnitAnimation::Walk);
    iconTexture.Load(ingameUnitsPath / "008_50730.DDS", GL_CLAMP, GL_LINEAR, GL_LINEAR);
    break;
  case UnitType::Scout:
    animations[static_cast<int>(UnitAnimation::Idle)].resize(2);
    ok = ok && LoadAnimation(0, "u_cav_scout_idleA_x1.smx", graphicsPath, cachePath, palettes, UnitAnimation::Idle);
    ok = ok && LoadAnimation(1, "u_cav_scout_idleB_x1.smx", graphicsPath, cachePath, palettes, UnitAnimation::Idle);
    animations[static_cast<int>(UnitAnimation::Walk)].resize(1);
    ok = ok && LoadAnimation(0, "u_cav_scout_walkA_x1.smx", graphicsPath, cachePath, palettes, UnitAnimation::Walk);
    iconTexture.Load(ingameUnitsPath / "064_50730.DDS", GL_CLAMP, GL_LINEAR, GL_LINEAR);
    break;
  case UnitType::NumUnits:
    LOG(ERROR) << "Invalid unit type in ClientUnitType constructor: " << static_cast<int>(type);
    ok = false;
    break;
  }
  
  if (!ok) {
    return false;
  }
  
  maxCenterY = 0;
  const auto& animationVariants = animations[static_cast<int>(UnitAnimation::Idle)];
  for (usize variant = 0; variant < animationVariants.size(); ++ variant) {
    const auto& animation = animationVariants[variant];
    for (int frame = 0; frame < animation.sprite.NumFrames() / kNumFacingDirections; ++ frame) {
      maxCenterY = std::max(maxCenterY, animation.sprite.frame(frame).graphic.centerY);
    }
  }
  
  return true;
}

int ClientUnitType::GetHealthBarHeightAboveCenter() const {
  constexpr float kHealthBarOffset = 10;
  return maxCenterY + kHealthBarOffset;
}

bool ClientUnitType::LoadAnimation(int index, const char* filename, const std::filesystem::path& graphicsPath, const std::filesystem::path& cachePath, const Palettes& palettes, UnitAnimation type) {
  std::vector<SpriteAndTextures>& animationVector = animations[static_cast<int>(type)];
  SpriteAndTextures& item = animationVector[index];
  
  return LoadSpriteAndTexture(
      (graphicsPath / filename).c_str(),
      (cachePath / filename).c_str(),
      GL_CLAMP,
      GL_NEAREST,
      GL_NEAREST,
      &item.sprite,
      &item.graphicTexture,
      &item.shadowTexture,
      palettes);
}


ClientUnit::ClientUnit(int playerIndex, UnitType type, const QPointF& mapCoord, double creationServerTime)
    : ClientObject(ObjectType::Unit, playerIndex, creationServerTime),
      type(type),
      mapCoord(mapCoord),
      direction(rand() % kNumFacingDirections),
      currentAnimation(UnitAnimation::Idle),
      currentAnimationVariant(0),
      lastAnimationStartTime(-1),
      movementSegment(-1, mapCoord, QPointF(0, 0), UnitAction::Idle) {}

QPointF ClientUnit::GetCenterProjectedCoord(Map* map) {
  return map->MapCoordToProjectedCoord(mapCoord);
}

QRectF ClientUnit::GetRectInProjectedCoords(Map* map, double elapsedSeconds, bool shadow, bool outline) {
  auto& unitTypes = ClientUnitType::GetUnitTypes();
  
  const ClientUnitType& unitType = unitTypes[static_cast<int>(type)];
  const SpriteAndTextures& animationSpriteAndTexture = unitType.GetAnimations(currentAnimation)[currentAnimationVariant];
  const Sprite& sprite = animationSpriteAndTexture.sprite;
  
  QPointF centerProjectedCoord = GetCenterProjectedCoord(map);
  
  float framesPerSecond = 30.f;
  int framesPerDirection = sprite.NumFrames() / kNumFacingDirections;
  int frameIndex = direction * framesPerDirection + static_cast<int>(framesPerSecond * elapsedSeconds + 0.5f) % framesPerDirection;
  
  const Sprite::Frame::Layer& layer = shadow ? sprite.frame(frameIndex).shadow : sprite.frame(frameIndex).graphic;
  bool isGraphic = !shadow && !outline;
  return QRectF(
      centerProjectedCoord.x() - layer.centerX + (isGraphic ? 1 : 0),
      centerProjectedCoord.y() - layer.centerY + (isGraphic ? 1 : 0),
      layer.imageWidth + (isGraphic ? -2 : 0),
      layer.imageHeight + (isGraphic ? -2 : 0));
}

void ClientUnit::Render(
    Map* map,
    const std::vector<QRgb>& playerColors,
    SpriteShader* spriteShader,
    GLuint pointBuffer,
    float* viewMatrix,
    float zoom,
    int widgetWidth,
    int widgetHeight,
    double serverTime,
    bool shadow,
    bool outline) {
  auto& unitTypes = ClientUnitType::GetUnitTypes();
  const ClientUnitType& unitType = unitTypes[static_cast<int>(type)];
  const SpriteAndTextures& animationSpriteAndTexture = unitType.GetAnimations(currentAnimation)[currentAnimationVariant];
  const Sprite& sprite = animationSpriteAndTexture.sprite;
  const Texture& texture = shadow ? animationSpriteAndTexture.shadowTexture : animationSpriteAndTexture.graphicTexture;
  
  QPointF centerProjectedCoord = GetCenterProjectedCoord(map);
  
  // Update the animation.
  float framesPerSecond = 30.f;
  int framesPerDirection = sprite.NumFrames() / kNumFacingDirections;
  
  if (lastAnimationStartTime < 0) {
    // Initialize lastAnimationStartTime.
    lastAnimationStartTime = serverTime;
  }
  int frame;
  while (true) {
    frame = std::max(0, static_cast<int>(framesPerSecond * (serverTime - lastAnimationStartTime) + 0.5f));
    if (frame < framesPerDirection) {
      break;
    }
    
    // A new animation starts. Choose a random animation variant.
    lastAnimationStartTime = std::min(serverTime, lastAnimationStartTime + framesPerDirection / framesPerSecond);
    // TODO: The "currentAnimationVariant == 1" condition is special-case handling to make the scout unit look nicer. Check how this should be handled in general in other cases.
    if (currentAnimationVariant == 1) {
      currentAnimationVariant = 0;
    } else {
      currentAnimationVariant = rand() % unitType.GetAnimations(currentAnimation).size();
    }
  }
  int frameIndex = direction * framesPerDirection + frame;
  
  DrawSprite(
      sprite,
      texture,
      spriteShader,
      centerProjectedCoord,
      pointBuffer,
      viewMatrix,
      zoom,
      widgetWidth,
      widgetHeight,
      frameIndex,
      shadow,
      outline,
      playerColors,
      playerIndex);
}

void ClientUnit::SetCurrentAnimation(UnitAnimation animation, double serverTime) {
  if (currentAnimation == animation) {
    return;
  }
  
  auto& unitTypes = ClientUnitType::GetUnitTypes();
  const ClientUnitType& unitType = unitTypes[static_cast<int>(type)];
  
  currentAnimation = animation;
  lastAnimationStartTime = serverTime;
  currentAnimationVariant = rand() % unitType.GetAnimations(currentAnimation).size();
}

void ClientUnit::UpdateGameState(double serverTime) {
  // Update the unit's movment according to the movement segment.
  if (movementSegment.action == UnitAction::Building) {
    mapCoord = movementSegment.startPoint;
  } else {
    mapCoord = movementSegment.startPoint + (serverTime - movementSegment.serverTime) * movementSegment.speed;
  }
  
  // Update facing direction.
  if (movementSegment.speed != QPointF(0, 0)) {
    // This angle goes from (-3) * M_PI / 4 to (+5) * M_PI / 4, with 0 being the right direction in the projected view.
    double angle = -1 * (atan2(movementSegment.speed.y(), movementSegment.speed.x()) - M_PI / 4);
    if (angle < 0) {
      angle += 2 * M_PI;
    }
    direction = std::max(0, std::min(kNumFacingDirections, static_cast<int>(kNumFacingDirections * angle / (2 * M_PI) + 0.5f)));
    if (direction == kNumFacingDirections) {
      direction = 0;
    }
  }
  
  if (movementSegment.action == UnitAction::Building) {
    SetCurrentAnimation(UnitAnimation::Task, serverTime);
  } else if (movementSegment.speed == QPointF(0, 0)) {
    // If the movement is zero, set the unit to the final position and delete the segment.
    SetCurrentAnimation(UnitAnimation::Idle, serverTime);
    
    mapCoord = movementSegment.startPoint;
  } else {
    // Use moving animation
    SetCurrentAnimation(UnitAnimation::Walk, serverTime);
  }
}
