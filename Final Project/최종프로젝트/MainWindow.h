#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QListWidget>
#include <QString>
#include <QVector>

// 항공편의 상세 정보를 하나로 묶어서 관리하는 구조체
struct MapProjectFlightDetail {
    QString airline;    // 항공사 이름 (예: 대한항공)
    QString flightNum;  // 항공 편명 (예: KE101)
    QString time;       // 출발 및 도착 시간 (예: 14:30)
    int price;          // 항공권 가격 (원 단위)
};

// 프로그램의 메인 화면을 담당하는 클래스
class MainWindow : public QMainWindow {
private:
    QLabel* statusLabel;             // 상단에 현재 상태(출발지/도착지 안내 등)를 보여주는 문구 창
    QComboBox* countryComboBox;
    QListWidget* outboundListWidget; // 가는 편 항공편 목록을 보여주는 상자 (출발지 -> 도착지)
    QListWidget* inboundListWidget;  // 오는 편 항공편 목록을 보여주는 상자 (도착지 -> 출발지)
    QPushButton* bookButton;         // 최종 예매 확정 버튼

    QString countries[12];           // 12개 국가의 이름이 저장되는 배열
    int adjMatrix[12][12];           // 국가 간의 직항 연결 여부를 저장하는 인접 행렬 (2차원 배열)

    int nodeX[12];                   // 지도 이미지 위 국가 버튼들의 X 좌표
    int nodeY[12];                   // 지도 이미지 위 국가 버튼들의 Y 좌표

    int startNodeIdx;                // 사용자가 선택한 출발 국가의 번호 (기본값 -1)
    int endNodeIdx;                  // 사용자가 선택한 도착 국가의 번호 (기본값 -1)
    QVector<QPushButton*> nodeButtons; // 화면에 뜨는 12개 버튼들의 스타일을 실시간으로 바꾸기 위한 배열

    // 선택된 국가 조합에 맞추어 실시간으로 4개의 항공편 일정을 자동으로 만들어주는 함수
    QVector<MapProjectFlightDetail> generateFlightSchedule(const QString& from, const QString& to, bool isReturn);

    // 출발지, 도착지 선택 상태에 따라 버튼 색상을 보기 좋게 바꿔주는 함수
    void updateNodeStyles();

protected:
    // 화면이 새로 그려질 때 자동으로 실행되는 Qt의 기본 그리기 함수
    void paintEvent(QPaintEvent* event) override;

public:
    MainWindow(); // 생성자: 프로그램이 처음 켜질 때 기본 데이터들을 초기화하는 곳
};