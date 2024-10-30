// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "RobotContainer.h"

#include <frc2/command/Commands.h>
#include <frc2/command/CommandPtr.h>
#include <frc2/command/button/CommandXboxController.h>

RobotContainer::RobotContainer()
{
  ConfigureBindings();
  m_intake = new Intake();
  m_vision = new Vision();
  m_shooter = new Shooter(m_vision);
}

void RobotContainer::ConfigureBindings()
{
  //m_intake->SetDefaultCommand(m_intake->IdleCommand());
  m_shooter->SetDefaultCommand(m_shooter->IdleCommand());

  //m_stick.A().ToggleOnTrue(m_intake->ActiveCommand().Unless([this] -> bool { return this->m_intake->IsProcessing(); }));
  m_stick.RightTrigger().WhileTrue(m_shooter->ShootCommand());
}

frc2::CommandPtr RobotContainer::GetAutonomousCommand()
{
  return frc2::cmd::Print("No autonomous command configured");
}
