#ifndef LED_STATUS_H_
#define LED_STATUS_H_

typedef enum
{
	E_Led_Start=1,
	E_Led_Reboot,
	E_Led_End
}Led_S;

class LedStatus
{
public:
	Led_S m_enStatus;
	int m_nCheck;
};

#endif/*LED_STATUS_H_*/