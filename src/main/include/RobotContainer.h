// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc2/command/CommandPtr.h>
#include <frc2/command/button/CommandXboxController.h>

#include "subsystems/Intake.h"
#include "subsystems/Shooter.h"
#include "subsystems/Vision.h"

class RobotContainer
{
public:
  RobotContainer();

  frc2::CommandPtr GetAutonomousCommand();

private:
  void ConfigureBindings();
  
  frc2::CommandXboxController m_stick{0};

  Intake  m_intake;
  Shooter m_shooter;
  Vision  m_vision;
};
