#pragma comment(lib, "pdh.lib")


#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <iostream>

using namespace std;

class CpuUsage
{
private:
	PDH_HQUERY              m_hQuery;
	//PDH_HQUERY :: ī���͸� �߰� �� ������ ó���մϴ�. �� �ڵ��� PdhOpenQuery �Լ��� ���� ��ȯ�˴ϴ�.

	

	PDH_HCOUNTER    m_hCounter;
	
	PDH_HQUERY *m_q = new PDH_HQUERY[8];
	PDH_HCOUNTER *m_c = new PDH_HCOUNTER[8];
	//PDH_HCOUNTER :: ������ �߰� �� ī���͸� ó���մϴ�. �ļ� ȣ�⿡���� �ڵ��� �����ؾ� �� ���� �ֽ��ϴ�.

public:
	CpuUsage() : m_hQuery(0), m_hCounter(0) {}
	~CpuUsage() { destroy(); }

	bool    init()
	{
		//1. ���� �����

		PDH_STATUS      status = PdhOpenQuery(0, 0, &m_hQuery);
		//PdhOpenQuery :: ���� ������ �÷����� �����ϴ� �� ���Ǵ� �� ������ ����ϴ�.
		/*
			parameters
			szDataSource[in]
			���� �����͸� �˻� �� �α� ������ �̸��� �����ϴ� ��(null)�� ������ ���ڿ�.NULL�̸� ���� �����Ͱ� �ǽð� ������ �������� �����˴ϴ�.
			dwUserData[in]
			�� ������ ������ ����� ���� ���Դϴ�.���߿� ����� �����͸� �˻��Ϸ��� PdhGetCounterInfo�� ȣ���ϰ� PDH_COUNTER_INFO�� dwQueryUserData ����� �׼����Ͻʽÿ�.
			phQuery[out]
			������ ó���մϴ�.�ļ� ȣ�⿡���� �ڵ��� ����մϴ�.
		*/
		if (status != ERROR_SUCCESS)
			return false;

		//2. ������ ī���� ���

		status = PdhAddCounter(m_hQuery, "\\Processor(_TOTAL)\\% Processor Time", 0, &m_hCounter);
		//status = PdhAddCounter(m_hQuery, "\\Processor(1)\\% Processor Time", 0, &m_hCounter); // �ھ�0���� ���� ������ ����


		//PdhAddCounter :: ������ ī���͸� ������ �߰��մϴ�.
		/*
			parameters
			hQuery [in]
			ī���͸� �߰� �� ������ ó���մϴ�. �� �ڵ��� PdhOpenQuery �Լ��� ���� ��ȯ�˴ϴ�.
			szFullCounterPath [in]
			ī���� ��ΰ� ���� �� �� ���� ���ڿ��Դϴ�. ī���� ����� ���Ŀ� ���� �ڼ��� ������ ī���� ��� ������ �����Ͻʽÿ�. ī���� ����� �ִ� ���̴� PDH_MAX_COUNTER_PATH�Դϴ�.
			dwUserData [in]
			����� ���� ��. �� ���� ī���� ������ �Ϻΰ��˴ϴ�. �� ���� ���߿� �˻��Ϸ��� PdhGetCounterInfo �Լ��� ȣ���ϰ� PDH_COUNTER_INFO ����ü�� dwUserData ����� �׼����Ͻʽÿ�.
			phCounter [���]
			������ �߰� �� ī���͸� ó���մϴ�. �ļ� ȣ�⿡���� �ڵ��� �����ؾ� �� ���� �ֽ��ϴ�.
		*/

		if (status != ERROR_SUCCESS)
			return false;

		//3. ���� ������ ����
		status = PdhCollectQueryData(m_hQuery);
		// PdhCollectQueryData :: ������ ������ ��� ī���Ϳ� ���� ���� ���� ������ ���� �����ϰ� �� ī������ ���� �ڵ带 ������Ʈ�մϴ�.
		if (status != ERROR_SUCCESS)
		{
			return false;
		}

		return true;
	}

	void    destroy()
	{
		//5. ���� �ݱ�
		if (m_hQuery)
			PdhCloseQuery(m_hQuery);
		m_hQuery = 0;
		//������ ���� �ʱ�ȭ PdhCloseQuery�� �ش� ������ �ݴµ��ϴ�.
	}

	bool    getCpuUsage(double * val)
	{
		//PDH_STATUS �� �ش� ������ ���� ���°� ��� ����� Ȯ���� ���� �� 
		PDH_STATUS      status = PdhCollectQueryData(m_hQuery);
		//PdhCollectQueryData :: ������ ������ ��� ī���Ϳ� ���� ���� ���� ������ ���� �����ϰ� �� ī������ ���� �ڵ带 ������Ʈ�մϴ�.
		//������ ���� 
		if (status != ERROR_SUCCESS)
			return false;

		//4. ������ ���� ǥ��  

		PDH_FMT_COUNTERVALUE    value;
		// PDH_FMT_COUNTERVALUE�� ������ ī���� �� ������ ��� �� ���� �����Ѵ�.
		status = PdhGetFormattedCounterValue(m_hCounter, PDH_FMT_DOUBLE, 0, &value);
		//PdhGetFormattedCounterValue :: ������ ī���Ϳ� ���� ǥ�� ������ ���� ����մϴ�.
		/*
			parameter

			hCounter [in]
			ǥ�� ������ ���� ����Ϸ��� ī������ �ڵ��Դϴ�. PdhAddCounter�� ����� �ڵ��� ��ȯ�մϴ�.
			dwFormat [in]
			����ȭ �� ���� ������ ������ �����մϴ�. ���� �� �� �ϳ��� �����Ͻʽÿ�.
			lpdwType [���]
			ī���� �������޽��ϴ�. ī���� ���� ����� Windows Server 2003 Deployment Kit �� ī���� ���� ������ �����Ͻʽÿ� . �� �Ű� ������ �������Դϴ�.
			pValue [���]
			PDH_FMT_COUNTERVALUE�� ī���� ���� ���� ����.
		*/
		if (status != ERROR_SUCCESS)
			return false;

		*val = value.doubleValue;
		return true;
	}

};


int main(int argc, char * argv[])
{
	CpuUsage        cpuUsage;
	if (cpuUsage.init() == false)
		return 1;

	while (true)
	{
		double  val = 0.0;
		if (cpuUsage.getCpuUsage(&val))
		{
			cout << "CPU Total Usage : " << val << "%\n";
		}
		Sleep(600);
		system("cls");
	}
	//cout << "\t\thttp://fehead.tistory.com\n";

	return 0;
}