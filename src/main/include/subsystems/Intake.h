#include <frc2/command/CommandPtr.h>
#include <frc2/command/Commands.h>
#include <frc2/command/RunCommand.h>
#include <frc2/command/ConditionalCommand.h>
#include <frc2/command/SubsystemBase.h>
#include "rev/ColorSensorV3.h"
#include <rev/CANSparkMax.h>
#include <frc/Timer.h>
#include "units/time.h"
#include "frc/DriverStation.h"
#include <map>

class Intake : public frc2::SubsystemBase
{
public:
    enum BALL_COLOR {
        kRED,
        kBLUE,
        kNONE
    };

    enum STATE {
        kIDLE,
        kACTIVE,
        kINTAKE,
        kEJECT
    } m_state;

    std::map<STATE, std::string> m_stateNames{{kIDLE, "IDLE"}, {kACTIVE, "ACTIVE"}, {kINTAKE, "INTAKE"}, {kEJECT, "EJECT"}} ;

    Intake();

    void Periodic() override;

    // COMMANDS
    frc2::CommandPtr ActiveCommand();
    frc2::CommandPtr IdleCommand();
    frc2::CommandPtr ProcessBallCommand();
    frc2::CommandPtr IntakeCommand();
    frc2::CommandPtr EjectCommand();

    bool IsProcessing() {
        return (m_state == kINTAKE) || (m_state == kEJECT);
    };

private:
    BALL_COLOR getBallColor();
    bool BallDetected();
    bool IsColorCorrect();

    const double TOLERANCE = 0.04;

    const double REDBALL_RED = 0.56;
    const double REDBALL_GREEN = 0.33;
    const double REDBALL_BLUE = 0.12;
    const double REDBALL_RED_MIN = REDBALL_RED - TOLERANCE;
    const double REDBALL_RED_MAX = REDBALL_RED + TOLERANCE;
    const double REDBALL_GREEN_MIN = REDBALL_GREEN - TOLERANCE;
    const double REDBALL_GREEN_MAX = REDBALL_GREEN + TOLERANCE;
    const double REDBALL_BLUE_MIN = REDBALL_BLUE - TOLERANCE;
    const double REDBALL_BLUE_MAX = REDBALL_BLUE + TOLERANCE;

    const double BLUEBALL_RED = 0.12;
    const double BLUEBALL_GREEN = 0.36;
    const double BLUEBALL_BLUE = 0.53;
    const double BLUEBALL_RED_MIN = BLUEBALL_RED - TOLERANCE;
    const double BLUEBALL_RED_MAX = BLUEBALL_RED + TOLERANCE;
    const double BLUEBALL_GREEN_MIN = BLUEBALL_GREEN - TOLERANCE;
    const double BLUEBALL_GREEN_MAX = BLUEBALL_GREEN + TOLERANCE;
    const double BLUEBALL_BLUE_MIN = BLUEBALL_BLUE - TOLERANCE;
    const double BLUEBALL_BLUE_MAX = BLUEBALL_BLUE + TOLERANCE;

    BALL_COLOR m_correctColor = kNONE;
    BALL_COLOR m_currentColor = kNONE;

    //rev::ColorSensorV3 m_colorSensor{frc::I2C::Port::kOnboard};
    //rev::CANSparkMax m_roller{2, rev::CANSparkMax::MotorType::kBrushless};
    rev::CANSparkMax m_hinge{3, rev::CANSparkMax::MotorType::kBrushless};
    rev::SparkRelativeEncoder m_encoder = m_hinge.GetEncoder();
    rev::SparkPIDController m_pid = m_hinge.GetPIDController();

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

    pidCoeff m_pidCoeff{0.1, 0.0, 0.0, 0.0, 0.0, -1.0, 1.0};
};