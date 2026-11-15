#include "Player/AuraPlayerController.h"
#include "Interaction/EnemyInterface.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
    // 开启网络复制，使该 Controller 能在客户端与服务端之间同步
    bReplicates = true;
}

// 每帧处理玩家相关逻辑
void AAuraPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);
    CursorTrace();
}

void AAuraPlayerController::BeginPlay()
{
    Super::BeginPlay();
    // AuraContext 需在蓝图子类中配置，缺失则直接断言失败
    check(AuraContext)

    // 获取本地玩家的 Enhanced Input 子系统，用于注册输入映射上下文
    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
        GetLocalPlayer());
    if (Subsystem)
    {
        // 注册输入映射上下文，优先级 0（数值越大优先级越高）
        Subsystem->AddMappingContext(AuraContext, 0);
    }

    // 显示鼠标光标，并使用系统默认光标样式
    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Default;

    // 同时接受游戏与 UI 输入：不锁定鼠标到视口，捕获时也不隐藏光标
    FInputModeGameAndUI InputModeData;
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputModeData.SetHideCursorDuringCapture(false);
    SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // 必须使用 EnhancedInputComponent（DefaultInput.ini 已配置），否则断言失败
    UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
    // MoveAction 需在蓝图子类中指定；Triggered 表示按键持续触发时每帧调用 Move
    EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const struct FInputActionValue& InputActionValue)
{
    // IA_Move 是二维轴输入：X 为左右，Y 为前后
    const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
    // 只取控制旋转的 Yaw，保证移动始终平行于地面（俯仰/翻滚不影响移动方向）
    const FRotator Rotation = GetControlRotation();
    const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

    // 由 Yaw 旋转矩阵换算出世界空间的前向与右向单位向量
    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    if (APawn* ControlledPawn = GetPawn<APawn>())
    {
        // 输入值作为缩放量：前后动作用 Y，左右平移用 X
        ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
        ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
    }
}

void AAuraPlayerController::CursorTrace()
{
    // 从光标位置向场景发射可见性射线，取第一个阻挡的物体
    FHitResult CursorHit;
    GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
    if (!CursorHit.bBlockingHit) return;

    // 上一帧的当前目标成为"旧目标"，本帧命中的 Actor 成为"新目标"
    LastActor = ThisActor;
    ThisActor = CursorHit.GetActor();

    // 分三种情况维护高亮状态
    if (LastActor == nullptr)
    {
        // 之前没有任何目标：新目标存在则点亮
        if (ThisActor != nullptr)
        {
            ThisActor->HighlightActor();
        }
    }
    else
    {
        if (ThisActor == nullptr)
        {
            // 光标离开所有可交互物体：熄灭旧目标
            LastActor->UnHighlightActor();
        }
        else
        {
            // 新旧目标不同：熄灭旧目标、点亮新目标；相同则不做处理
            if (LastActor != ThisActor)
            {
                LastActor->UnHighlightActor();
                ThisActor->HighlightActor();
            }
        }
    }
}
