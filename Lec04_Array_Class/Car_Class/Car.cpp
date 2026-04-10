#include "SportsCar.h"

int main() {
    // 1. MyCar 객체 생성 (초기값: 속도 0, 이름 "MyCar", 기어 1)
    SportsCar myCar(0, "MyCar", 1);

    // 2. 결과 화면과 동일한 상태로 조작
    myCar.changeGear(3);   // 기어 3단으로 변경
    myCar.setTurbo(true);  // 터보 온 (속도 증가폭을 20으로 만듦)
    myCar.speedUp();       // speed += 20 실행

    // 3. 출력
    myCar.display();       // [MyCar] : Gear=3, Speed=20kmph 출력
    myCar.whereAmI();      // 주소값 출력

    return 0;
}