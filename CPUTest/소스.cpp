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
	//PDH_HQUERY :: 카운터를 추가 할 쿼리를 처리합니다. 이 핸들은 PdhOpenQuery 함수에 의해 반환됩니다.

	

	PDH_HCOUNTER    m_hCounter;
	
	PDH_HQUERY *m_q = new PDH_HQUERY[8];
	PDH_HCOUNTER *m_c = new PDH_HCOUNTER[8];
	//PDH_HCOUNTER :: 쿼리에 추가 된 카운터를 처리합니다. 후속 호출에서이 핸들을 참조해야 할 수도 있습니다.

public:
	CpuUsage() : m_hQuery(0), m_hCounter(0) {}
	~CpuUsage() { destroy(); }

	bool    init()
	{
		//1. 쿼리 만들기

		PDH_STATUS      status = PdhOpenQuery(0, 0, &m_hQuery);
		//PdhOpenQuery :: 성능 데이터 컬렉션을 관리하는 데 사용되는 새 쿼리를 만듭니다.
		/*
			parameters
			szDataSource[in]
			성능 데이터를 검색 할 로그 파일의 이름을 지정하는 널(null)로 끝나는 문자열.NULL이면 성능 데이터가 실시간 데이터 원본에서 수집됩니다.
			dwUserData[in]
			이 쿼리와 연결할 사용자 정의 값입니다.나중에 사용자 데이터를 검색하려면 PdhGetCounterInfo를 호출하고 PDH_COUNTER_INFO의 dwQueryUserData 멤버에 액세스하십시오.
			phQuery[out]
			쿼리를 처리합니다.후속 호출에서이 핸들을 사용합니다.
		*/
		if (status != ERROR_SUCCESS)
			return false;

		//2. 쿼리에 카운터 등록

		status = PdhAddCounter(m_hQuery, "\\Processor(_TOTAL)\\% Processor Time", 0, &m_hCounter);
		//status = PdhAddCounter(m_hQuery, "\\Processor(1)\\% Processor Time", 0, &m_hCounter); // 코어0번에 대한 사용률이 나옴


		//PdhAddCounter :: 지정된 카운터를 쿼리에 추가합니다.
		/*
			parameters
			hQuery [in]
			카운터를 추가 할 쿼리를 처리합니다. 이 핸들은 PdhOpenQuery 함수에 의해 반환됩니다.
			szFullCounterPath [in]
			카운터 경로가 포함 된 널 종료 문자열입니다. 카운터 경로의 형식에 대한 자세한 내용은 카운터 경로 지정을 참조하십시오. 카운터 경로의 최대 길이는 PDH_MAX_COUNTER_PATH입니다.
			dwUserData [in]
			사용자 정의 값. 이 값은 카운터 정보의 일부가됩니다. 이 값을 나중에 검색하려면 PdhGetCounterInfo 함수를 호출하고 PDH_COUNTER_INFO 구조체의 dwUserData 멤버에 액세스하십시오.
			phCounter [출력]
			쿼리에 추가 된 카운터를 처리합니다. 후속 호출에서이 핸들을 참조해야 할 수도 있습니다.
		*/

		if (status != ERROR_SUCCESS)
			return false;

		//3. 쿼리 데이터 수집
		status = PdhCollectQueryData(m_hQuery);
		// PdhCollectQueryData :: 지정된 쿼리의 모든 카운터에 대한 현재 원시 데이터 값을 수집하고 각 카운터의 상태 코드를 업데이트합니다.
		if (status != ERROR_SUCCESS)
		{
			return false;
		}

		return true;
	}

	void    destroy()
	{
		//5. 쿼리 닫기
		if (m_hQuery)
			PdhCloseQuery(m_hQuery);
		m_hQuery = 0;
		//쿼리에 대한 초기화 PdhCloseQuery로 해당 쿼리를 닫는듯하다.
	}

	bool    getCpuUsage(double * val)
	{
		//PDH_STATUS 는 해당 쿼리에 대한 상태가 어떻게 됬는지 확인을 위한 것 
		PDH_STATUS      status = PdhCollectQueryData(m_hQuery);
		//PdhCollectQueryData :: 지정된 쿼리의 모든 카운터에 대한 현재 원시 데이터 값을 수집하고 각 카운터의 상태 코드를 업데이트합니다.
		//쿼리에 대한 
		if (status != ERROR_SUCCESS)
			return false;

		//4. 데이터 실적 표시  

		PDH_FMT_COUNTERVALUE    value;
		// PDH_FMT_COUNTERVALUE의 구조는 카운터 및 상태의 계산 된 값을 포함한다.
		status = PdhGetFormattedCounterValue(m_hCounter, PDH_FMT_DOUBLE, 0, &value);
		//PdhGetFormattedCounterValue :: 지정된 카운터에 대한 표시 가능한 값을 계산합니다.
		/*
			parameter

			hCounter [in]
			표시 가능한 값을 계산하려는 카운터의 핸들입니다. PdhAddCounter의 기능이 핸들을 반환합니다.
			dwFormat [in]
			형식화 된 값의 데이터 형식을 결정합니다. 다음 값 중 하나를 지정하십시오.
			lpdwType [출력]
			카운터 유형을받습니다. 카운터 유형 목록은 Windows Server 2003 Deployment Kit 의 카운터 유형 섹션을 참조하십시오 . 이 매개 변수는 선택적입니다.
			pValue [출력]
			PDH_FMT_COUNTERVALUE의 카운터 값을 수신 구조.
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