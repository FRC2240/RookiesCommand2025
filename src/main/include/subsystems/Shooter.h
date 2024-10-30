#include <frc2/command/CommandPtr.h>
#include <frc2/command/Commands.h>
#include <frc2/command/RunCommand.h>
#include <frc2/command/ConditionalCommand.h>
#include <frc2/command/SubsystemBase.h>
#include <units/length.h>
#include <rev/CANSparkMax.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include "subsystems/Vision.h"

class Shooter : public frc2::SubsystemBase
{
public:

  enum STATE {
    kACTIVE,
    kIDLE
  } m_state = kIDLE;

  Shooter(Vision *visionPtr);

  void Periodic() override;

  frc2::CommandPtr IdleCommand();
  frc2::CommandPtr ShootCommand();


private:
  rev::CANSparkMax m_left{2, rev::CANSparkMax::MotorType::kBrushless};
  rev::SparkRelativeEncoder m_encoder = m_left.GetEncoder();
  rev::SparkPIDController m_pid = m_left.GetPIDController();

  struct pidCoeff
  {
    double kP;
    double kI;
    double kD;
    double kIz;
    double kFF;
    double kMinOutput;
    double kMaxOutput;
  };

  void setP(double pGain) { m_pidCoeff.kP = pGain; }

  pidCoeff m_pidCoeff{0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 1.0};

  Vision* visionPtr;
};