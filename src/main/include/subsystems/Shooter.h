#pragma once

#include <units/length.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc2/command/SubsystemBase.h>
#include <frc2/command/CommandPtr.h>
#include <frc2/command/Commands.h>
#include <frc2/command/RunCommand.h>
#include <frc2/command/ConditionalCommand.h>
#include "ctre/phoenix6/TalonFX.hpp"

class Shooter : public frc2::SubsystemBase
{
public:
    Shooter();

    void Periodic() override;

    // COMMANDS
    frc2::CommandPtr IdleCommand();
    frc2::CommandPtr ShootCommand();

private:
    static constexpr char const *CANBUS_NAME = "rio";
    ctre::phoenix6::hardware::TalonFX m_fx{21, CANBUS_NAME};

    /* Start at velocity 0, use slot 0 */
    ctre::phoenix6::controls::VelocityTorqueCurrentFOC m_velocityTorque = ctre::phoenix6::controls::VelocityTorqueCurrentFOC{0_tps}.WithSlot(0);
    /* Keep a neutral out so we can disable the motor */
    ctre::phoenix6::controls::NeutralOut m_brake{};

    units::angular_velocity::turns_per_second_t m_desiredRotationsPerSecond = 20.0_tps;
};