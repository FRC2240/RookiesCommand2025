#include "subsystems/Shooter.h"

Shooter::Shooter(Vision *visionPtr)
{
  this->visionPtr = visionPtr;

  frc::SmartDashboard::PutNumber("P Gain", m_pidCoeff.kP);
  m_pid.SetP(m_pidCoeff.kP);

  m_pidCoeff.kFF = frc::SmartDashboard::PutNumber("Feed Forward", m_pidCoeff.kFF);
  m_pid.SetFF(m_pidCoeff.kFF);
}

void Shooter::Periodic()
{
  frc::SmartDashboard::PutNumber("Velocity", m_encoder.GetVelocity());
  m_pidCoeff.kP = frc::SmartDashboard::GetNumber("P Gain", m_pidCoeff.kP);
  m_pid.SetP(m_pidCoeff.kP);

  m_pidCoeff.kFF = frc::SmartDashboard::GetNumber("Feed Forward", m_pidCoeff.kFF);
  m_pid.SetFF(m_pidCoeff.kFF);
}

frc2::CommandPtr Shooter::IdleCommand()
{
  return frc2::RunCommand([this] -> void
                          { m_pid.SetReference(0.0, rev::CANSparkMax::ControlType::kVelocity); },
                          {this})
      .ToPtr();
}

frc2::CommandPtr Shooter::ShootCommand()
{
  return frc2::RunCommand([this] -> void
                          { m_pid.SetReference(1250.0, rev::CANSparkMax::ControlType::kVelocity); },
                          {this})
      .OnlyIf([this] -> bool
              { 
                units::meter_t distance = visionPtr->getDistanceToTarget();
                return (distance >= 6.5_ft) && (distance <= 7.0_ft); });
}