#include <QApplication>
#include "MainWindow.h"

// 프로그램 전체가 실행을 시작하는 지점
int main(int argc, char* argv[]) {
    QApplication app(argc, argv); // Qt 어플리케이션 관리 시스템 구동

    MainWindow window; // 우리가 디자인한 메인 인터페이스 창 객체 생성
    window.show();     // 화면에 모니터 창 띄우기

    return app.exec(); // 프로그램이 종료될 때까지 무한 루프(이벤트 대기 상태) 유지
}