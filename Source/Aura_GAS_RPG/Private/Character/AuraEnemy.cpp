#include "Character/AuraEnemy.h"
#include "Aura_GAS_RPG/Aura_GAS_RPG.h"

AAuraEnemy::AAuraEnemy()
{
    // 让角色网格阻挡 Visibility 通道，光标射线才能命中敌人而不是穿过它打到地面
    GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

void AAuraEnemy::HighlightActor()
{
    // 开启 CustomDepth 渲染并写入红色模板值，后处理材质据此绘制描边
    GetMesh()->SetRenderCustomDepth(true);
    GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
    Weapon->SetRenderCustomDepth(true);
    Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::UnHighlightActor()
{
    GetMesh()->SetRenderCustomDepth(false);
    Weapon->SetRenderCustomDepth(false);
}
