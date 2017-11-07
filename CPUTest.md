
## 1. CPU 사용량 알아보기

## 2. CPU 사용량 계산은 어떻게 이뤄지는가?

    cpu의 계산은 1초당 0과 1을 왔다 갔다 하는 HZ를 단위로 사용 4Ghz는 초당 0~1을 1천만번 왔다갔다 하는것을 의미 
    cpu 사용량은 사용시간 대비 실제 cpu가 일한 시간

    사용량을 구하는 방식은 여러가지가 있다.
    Processor:% Processor Time
    System: %Total Processor Time
    Processor: % Privileged Time
    Processor: %User Time
    System: Processor Queue Length
    
    https://docs.microsoft.com/ko-kr/sql/relational-databases/performance-monitor/monitor-cpu-usage
## 3. 사용량을 알기 위한 방법

    pdh함수를 통한 카운터 데이터 사용

    pdh 장점 레지스트리 기능보다 사용하기 쉽고 현재 활동, 로그 파일의 데이터에 엑세스 하는데 사용가능

    1.쿼리 만들기

    2.쿼리에 카운터 추가

    3.성능데이터 수집

    4.실적데이터 표시

    5.쿼리 닫기


## 4. 코드



## 5. 개선 사항

cpu 온도 및 코어 수에 따른 cpu 사용량 계산 필요
1차 작업 cpu core 수 알아 내는방법 모색
processor count를 알아내는 방법은 kene32.h 에 있는듯
온도는 알아봐야함
