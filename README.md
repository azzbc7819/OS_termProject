# 운영체제 기말 프로젝트
--------------------
## 개요
- 소켓을 이용한 멀티프로세싱 프로그램으로 다중 클라이언트가 하나의 서버에 접속할 수 있는 채팅 프로그램

## 주요 기능
- 서버
> 부모 프로세스 : 클라이언트 요청 시 fork()를 통해 자식 프로세스 생성 및 자식 프로세스 관리
> 자식 프로세스 : 클라이언트와의 통신 역할로 소켓을 통해 클라이언트로부터 메시지를 받는다
- 클라이언트
> 사용자에게 입력을 받고 소켓을 통해 서버에게 메시지 전송

-----------
현재는 서버-클라이언트 간의 통신만 가능하고 클라이언트-클라이언트간의 통신은 어려운 상태