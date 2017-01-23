# 위치 공유 시스템
## 개요
단말기의 위치정보를 server에서 RT 단위로 upload하여 user로 하여금 단말기의 위치정보를 파악하기 위함.

## 개발 및 구성 환경
- 시스템 : Docker - Ubuntu 16.04 RTS
  > 어느 플랫폼에서든 동일한 개발환경을 지원하고, 개발시 성능의 저하를 최소화 하기 위해서 도입.

- 개발 언어
  - **Python 3.x**
  - **C/C++**
  - **Rust**
    > C/C++은 병렬성, 메모리 관리, 구조 등의 문제가 많다.  
    이를 해결하기 위해 low-level language를 찾던 중 대체 언어로 Go와 Rust를 고려함.  
    두 언어 다 매력이 있었으나 다음과 같은 사항을 준수해야함.

    >- 문법이 python과 비슷한 등급일정도로 심플해야한다.
    - 언어를 습득하는데 있어 오랜 시간이 걸리면 안된다.
    - 속도, 병렬성, 메모리 관리 등에 있어 합리적이어야 한다.
    - 많은 개발자들이 사용하며, document가 충분해야 한다.
    - 배포 과정이 복잡해서는 안된다.

    > 무엇보다도 많은 개발자들이 Rust를 Go보다 추천하는 것으로 파악. Rust는 Go에 비해 document나 개발자는 월등히 적지만, 지금 개발하려는 프로그램의 수준과 학습에 필요한 한글화, 문서 등은 충분하다고 판단.

## 예상되는 삽입 기술정보
- 암호화 기술 : AES, RSA
- 통신 기술 : TLS(openSSL)
- 인증, 무결성, 비가역성 기술 : Block chain

## 예상되는 Data 정보
- 단말 정보 : UUID
- 시간 정보 : Time stamp
- 위치 정보 :   
  - 위치 정보 시스템 종류
  - 위치 정보 raw data

## 그외
- [version](version.md)
- [설계]()
