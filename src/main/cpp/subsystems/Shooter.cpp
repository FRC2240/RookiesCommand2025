#include "subsystems/Shooter.h"
#include <iostream>

Shooter::Shooter()
{
    ctre::phoenix6::configs::TalonFXConfiguration configs{};

    /* Torque-based velocity does not require a feed forward, as torque will accelerate the rotor up to the desired velocity by itself */
    configs.Slot0.kS = 2.5; // To account for friction, add 2.5 A of static feedforward
    configs.Slot0.kP = 5;   // An error of 1 rotation per second results in 5 A output
    configs.Slot0.kI = 0;   // No output for integrated error
    configs.Slot0.kD = 0;   // No output for error derivative
    // Peak output of 40 A
    configs.TorqueCurrent.PeakForwardTorqueCurrent = 40;
    configs.TorqueCurrent.PeakReverseTorqueCurrent = -40;

    /* Retry config apply up to 5 times, report if failure */
    ctre::phoenix::StatusCode status = ctre::phoenix::StatusCode::StatusCodeNotInitialized;
    for (int i = 0; i < 5; ++i)
    {
        status = m_fx.GetConfigurator().Apply(configs);
        if (status.IsOK())
            break;
    }
    if (!status.IsOK())
    {
        std::cout << "Could not apply configs, error code: " << status.GetName() << std::endl;
    }
}

void Shooter::Periodic()
{
    frc::SmartDashboard::PutNumber("Shooter Velocity", m_fx.GetVelocity().GetValueAsDouble());
}

// Default command, stow the intake
frc2::CommandPtr Shooter::IdleCommand()
{
    return frc2::cmd::Run([this]
                            {
                                m_fx.SetControl(m_velocityTorque.WithVelocity(m_desiredRotationsPerSecond));
                                frc::SmartDashboard::PutString("Shooter", "NO"); },
                            {this}) ;
}

// Deploy the intake to collect a game piece
frc2::CommandPtr Shooter::ShootCommand()
{
    return frc2::cmd::Run([this]
                            {
                                m_fx.SetControl(m_brake);
                                frc::SmartDashboard::PutString("Shooter", "YES");
                            },
                            {this});
}
