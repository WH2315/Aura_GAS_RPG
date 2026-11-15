#include "Character/AuraCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

AAuraCharacter::AAuraCharacter()
{
    // 让角色朝移动方向转身，而不是跟随控制器朝向（俯视角 RPG 常见做法）
    GetCharacterMovement()->bOrientRotationToMovement = true;
    // 转身速度：每秒最多转向 400 度（仅 Yaw）
    GetCharacterMovement()->RotationRate = FRotator(0.f, 400.0f, 0.f);
    // 把移动约束在平面内，避免角色有上下浮动
    GetCharacterMovement()->bConstrainToPlane = true;
    // 出生时立即贴合约束平面，防止初始位置偏移
    GetCharacterMovement()->bSnapToPlaneAtStart = true;
    
    // 关闭控制器的俯仰/翻滚/偏航旋转，角色朝向完全交由移动逻辑控制
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;
    bUseControllerRotationYaw = false;
}
