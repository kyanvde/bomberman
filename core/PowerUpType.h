#ifndef BOMBERMAN_CORE_POWERUPTYPE_H
#define BOMBERMAN_CORE_POWERUPTYPE_H

namespace core {

/**
 * @brief The kind of permanent stat boost a PowerUp grants when picked up: Fire increases blast
 * radius, ExtraBomb increases bomb capacity, Skates increases movement speed.
 */
enum class PowerUpType { Fire, ExtraBomb, Skates };

} // namespace core

#endif // BOMBERMAN_CORE_POWERUPTYPE_H
