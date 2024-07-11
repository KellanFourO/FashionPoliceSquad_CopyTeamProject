# Fashion Police Squad 모작 팀 프로젝트

Mopeful Games 회사의 FPS 장르인 Fashion Police Squad 라는 3D 게임을 팀으로 모작하였습니다.

## 사용된 기술
-   ![C](https://img.shields.io/badge/C%20-%232370ED.svg?&logo=c&logoColor=white)
 ![C++](https://img.shields.io/badge/C++%20-%2300599C.svg?&logo=c%2B%2B&logoColor=white)
 ![Direct 9](https://img.shields.io/badge/Direct%209%20-%236DA252.svg?&logo=Direct9&logoColor=white)
 ![HLSL](https://img.shields.io/badge/HLSL%20-%236DA252.svg?&logo=HLSL&logoColor=white)
 ![Imgui](https://img.shields.io/badge/Imgui%20-%23F94877.svg?&logo=Imgui&logoColor=white)

<br>
<br/>

<!--프로젝트 대문 -->
[![Video Label](https://ifh.cc/g/8m3ROa.jpg)](https://youtu.be/HiKJTmvurb8?t=17)

# 개발 기간 및 참여 인원
   
 개발기간 : 2023.09.08 ~ 2023.10.23
   
 참여 인원 : 3 people

# 중점 구현 사항

프로젝트에서 제가 맡은 파트는 UI, 플레이어, 몬스터, 보스 였습니다.

- 월드 스페이스상의 UI를 구현해보았습니다. UI들은 매니저를 통해 관리하였습니다.
  
- 내적과 외적을 이용하여 플레이어와 적군의 투사체 등을 처리하였습니다.
  
- 상태머신을 사용하여 플레이어와 적군 AI를 구현하였습니다.
  
- 컴포넌트 패턴을 적용하여 객체별로 필요한 기능을 구분지어 사용할 수 있었습니다.
  
- 프로토타입 패턴을 사용하였고 레퍼런스 카운트를 직접 관리하여 메모리에 대한 이해도를 높였습니다.

- 렌더러라는 모든 오브젝트에 렌더링을 담당하는 객체를 두어서 관리하였습니다.

- 객체를 구분짓는 레이어 개념을 사용하였습니다.

- 레이캐스트를 사용하여 픽킹과 플레이어의 사격지점과 상호작용이 필요한 오브젝트를 
구현했습니다.

- imgui 기반의 UI툴을 만들어 UI의 배치 및 파싱 하였습니다.

- 평면의 방정식을 이용하여 플레이어가 맵 위로 걸어다닐 수 있게 하였습니다.

- 보스는 3페이즈로 구성하여 페이즈에 따른 패턴추가에 의한 난이도 상승과 이펙트 추가를 넣어봤습니다.
