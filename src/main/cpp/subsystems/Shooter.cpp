#include "subsystems/Shooter.h"

void Shooter::Periodic() {
    frc::SmartDashboard::PutNumber("Velocity", m_encoder.GetVelocity());
    m_pidCoeff.kP = frc::SmartDashboard::GetNumber("P Gain", m_pidCoeff.kP);
    m_pid.SetP(m_pidCoeff.kP);

    m_pidCoeff.kFF = frc::SmartDashboard::GetNumber("Feed Forward", m_pidCoeff.kFF);
    m_pid.SetFF(m_pidCoeff.kFF);
}

// Default command, stow the intake
frc2::CommandPtr Shooter::IdleCommand()
{
    return frc2::RunCommand([this] -> void
                            {
                                m_pid.SetReference(0.0, rev::CANSparkMax::ControlType::kVelocity);
                            },
                            {this})
        .ToPtr();
}

// Deploy the intake to collect a game piece
frc2::CommandPtr Shooter::ShootCommand()
{
    return frc2::RunCommand([this] -> void
                            {
                                m_pid.SetReference(1250.0, rev::CANSparkMax::ControlType::kVelocity);
                            },
                            {this})
        .ToPtr();
}
