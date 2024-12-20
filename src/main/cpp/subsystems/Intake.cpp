#include "subsystems/Intake.h"
#include <frc/smartdashboard/SmartDashboard.h>

Intake::Intake()
{
    frc::SmartDashboard::PutNumber("red", 0);
    frc::SmartDashboard::PutNumber("green", 0);
    frc::SmartDashboard::PutNumber("blue", 0);

    m_pid.SetP(m_pidCoeff.kP);
}

// This method will be called once per scheduler run
void Intake::Periodic()
{
    std::optional<frc::DriverStation::Alliance> alliance = frc::DriverStation::GetAlliance();

    if (alliance == frc::DriverStation::Alliance::kRed)
    {
        m_correctColor = kRED;
    }
    if (alliance == frc::DriverStation::Alliance::kBlue)
    {
        m_correctColor = kBLUE;
    }

    m_currentColor = getBallColor();

    frc::SmartDashboard::PutString("state", m_stateNames[m_state]);
}

bool Intake::IsColorCorrect()
{
    return (m_correctColor == m_currentColor);
}

Intake::BALL_COLOR Intake::getBallColor()
{
    //auto color = m_colorSensor.GetColor();
    
    frc::Color color = {
        frc::SmartDashboard::GetNumber("red", 0),
        frc::SmartDashboard::GetNumber("green", 0),
        frc::SmartDashboard::GetNumber("blue", 0)
    };

    //double sensorColors[3] = {color.red, color.green, color.blue};
    //frc::SmartDashboard::PutNumberArray("Sensor Color", sensorColors);

    if (((color.red > REDBALL_RED_MIN) && (color.red < REDBALL_RED_MAX)) &&
        ((color.green > REDBALL_GREEN_MIN) && (color.green < REDBALL_GREEN_MAX)) &&
        ((color.blue > REDBALL_BLUE_MIN) && (color.blue < REDBALL_BLUE_MAX)))
    {
        return kRED;
    }

    if (((color.red > BLUEBALL_RED_MIN) && (color.red < BLUEBALL_RED_MAX)) &&
        ((color.green > BLUEBALL_GREEN_MIN) && (color.green < BLUEBALL_GREEN_MAX)) &&
        ((color.blue > BLUEBALL_BLUE_MIN) && (color.blue < BLUEBALL_BLUE_MAX)))
    {
        return kBLUE;
    }

    return kNONE;
}

bool Intake::BallDetected()
{
    return m_currentColor != kNONE;
}

//
// COMMANDS
//

// Default command, stow the intake
frc2::CommandPtr Intake::IdleCommand()
{
    return frc2::RunCommand([this] -> void
                            {
                                m_state = kIDLE;
                                m_roller.Set(0.0);
                                m_pid.SetReference(0.0, rev::CANSparkMax::ControlType::kPosition);
                            },
                            {this})
        .ToPtr();
}

// Deploy the intake to collect a game piece
frc2::CommandPtr Intake::ActiveCommand()
{
    return frc2::RunCommand([this] -> void
                            {
                                m_state = kACTIVE;
                                m_roller.Set(0.05);
                                m_pid.SetReference(80.5, rev::CANSparkMax::ControlType::kPosition);
                            },
                            {this})
        .Until([this] -> bool
               { return BallDetected(); })
        .AndThen(ProcessBallCommand());
}

// Eject the game piece
frc2::CommandPtr Intake::EjectCommand()
{
    return frc2::RunCommand([this] -> void
                            {
                                m_state = kEJECT;
                                m_roller.Set(-0.05);
                                m_pid.SetReference(80.5, rev::CANSparkMax::ControlType::kPosition);
                            },
                            {this})
        .WithTimeout(5.0_s);
        //.AndThen(ActiveCommand());
}

// Intake the game piece
frc2::CommandPtr Intake::IntakeCommand()
{
    return frc2::RunCommand([this] -> void
                            {
                                m_state = kINTAKE;
                                m_roller.Set(0.05);
                                m_pid.SetReference(80.5, rev::CANSparkMax::ControlType::kPosition);
                            },
                            {this})
        .WithTimeout(5.0_s);
}

// Either intake or eject the game piece
frc2::CommandPtr Intake::ProcessBallCommand()
{
    return frc2::cmd::Either(
        IntakeCommand(),
        EjectCommand(),
        [this] -> bool
        { return IsColorCorrect(); });
}