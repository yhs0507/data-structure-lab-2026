#include "MainWindow.h"
#include <QPixmap>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include <QPainter>
#include <QPen>
#include <cmath> // std::abs 사용

// 배경 지도 그림과 국가 간의 연결 선을 겹쳐서 깔끔하게 그리기 위한 전용 도화지 클래스
class MapCanvas : public QWidget {
private:
    int (*adjMatrix)[12]; // 메인 창이 가진 인접 행렬 데이터의 주소를 가리키는 포인터
    int* nodeX;           // 메인 창의 X 좌표 배열 주소
    int* nodeY;           // 메인 창의 Y 좌표 배열 주소
    int& startNodeIdx;    // 메인 창의 출발지 선택 번호를 실시간으로 연결한 참조 변수
    int& endNodeIdx;      // 메인 창의 도착지 선택 번호를 실시간으로 연결한 참조 변수

public:
    // 생성자: 메인 창이 가진 그래프 핵심 데이터들을 주소값으로 넘겨받아 동기화함
    MapCanvas(QWidget* parent, int matrix[12][12], int* x, int* y, int& start, int& end)
        : QWidget(parent), adjMatrix(matrix), nodeX(x), nodeY(y), startNodeIdx(start), endNodeIdx(end) {
        setFixedSize(520, 300); // 도화지의 크기를 가로 520, 세로 300으로 고정
    }

protected:
    // 도화지가 화면에 나타나거나 업데이트될 때 선을 그려주는 함수
    void paintEvent(QPaintEvent* event) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing); // 선을 부드럽게 그리는 옵션 켜기

        // 1. 바닥에 세계지도 배경 이미지 깔기
        QPixmap pixmap("world.jpg");
        if (!pixmap.isNull()) {
            painter.drawPixmap(0, 0, pixmap.scaled(520, 300, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        }
        else {
            // 이미지 파일이 없을 경우를 대비한 안전장치 배경색
            painter.fillRect(rect(), QColor("#1E293B"));
        }

        // 2. 인접 행렬 표를 검사해서 값이 1인 연결된 노선에만 그래프 선 그리기
        QPen defaultPen(QColor(0, 242, 254, 180), 2, Qt::SolidLine); // 기본 연결 선 (하늘색, 약간 투명)
        QPen activePen(QColor(0, 255, 200, 255), 4, Qt::SolidLine);  // 현재 선택된 구간 선 (밝은 민트색, 두껍게)

        for (int i = 0; i < 12; ++i) {
            for (int j = i + 1; j < 12; ++j) { // 무방향 그래프이므로 중복을 피해 절반만 검사
                if (adjMatrix[i][j] == 1) {
                    // 현재 검사하는 간선이 사용자가 선택한 출발지-도착지 구간인지 확인
                    if ((i == startNodeIdx && j == endNodeIdx) || (i == endNodeIdx && j == startNodeIdx)) {
                        painter.setPen(activePen); // 선택된 노선은 강조 선으로 지정
                    }
                    else {
                        painter.setPen(defaultPen); // 나머지는 일반 선으로 지정
                    }
                    // 두 국가의 좌표를 이어주는 실제 선을 화면에 그리기
                    painter.drawLine(nodeX[i], nodeY[i], nodeX[j], nodeY[j]);
                }
            }
        }
    }
};

// 출발지와 도착지에 따라 고유하면서도 무작위 느낌을 주는 4개 항공사의 스케줄을 자동 생성하는 함수
QVector<MapProjectFlightDetail> MainWindow::generateFlightSchedule(const QString& from, const QString& to, bool isReturn) {
    QVector<MapProjectFlightDetail> schedules;
    QString baseStr = from + to + (isReturn ? "Return" : "Out");

    // 글자들을 숫자로 바꾸어 고유한 임시 번호(시드값) 추출
    int baseSeed = 0;
    for (char c : baseStr.toStdString()) baseSeed += c;
    baseSeed = std::abs(baseSeed);

    QString airlines[4] = { "대한항공", "아시아나항공", "진에어", "제주항공" };
    QString codes[4] = { "KE", "OZ", "LJ", "7C" };

    // 4개 항공사의 데이터를 규칙성 있게 채워 넣음
    for (int i = 0; i < 4; ++i) {
        MapProjectFlightDetail f;
        f.airline = airlines[i];
        f.flightNum = codes[i] + QString("%1").arg(((baseSeed + i * 77) % 800) + 100); // 겹치지 않는 편명 생성

        int hour = (baseSeed + i * 7) % 24;
        int minute = (baseSeed + i * 15) % 2 == 0 ? 0 : 30; // 00분 또는 30분으로 마감

        f.time = QString("%1:%2")
            .arg(QString::number(hour).rightJustified(2, '0'))
            .arg(QString::number(minute).rightJustified(2, '0'));

        // 대형항공사와 저가항공사의 가격대를 차등 부여
        int priceBase = ((baseSeed % 5) + 6);
        if (i >= 2) f.price = (priceBase * 9000) + (i * 3500);
        else f.price = (priceBase * 15000) + (i * 8000);

        schedules.append(f);
    }
    return schedules;
}

// 생성자: 메인 창의 기본 레이아웃을 구성하고 데이터를 가득 채우는 메인 로직
MainWindow::MainWindow() {
    // 초기에는 아무것도 선택되지 않은 상태이므로 -1로 대입
    startNodeIdx = -1;
    endNodeIdx = -1;

    // 1. 12개 국가 명칭 데이터 등록
    QString tempCountries[12] = {
        "대한민국", "미국", "일본", "중국", "대만", "베트남",
        "러시아", "덴마크", "뉴질랜드", "스위스", "이탈리아", "호주"
    };
    for (int i = 0; i < 12; ++i) this->countries[i] = tempCountries[i];

    // 2. 그래프 인접 행렬 데이터 등록 (1인 행과 열끼리 노선 선이 연결됨)
    int tempMatrix[12][12] = {
        {0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0}, {1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0},
        {1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0}, {1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0},
        {1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0}, {1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0}, {0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0},
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, {0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1}, {0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0}
    };
    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 12; ++j) this->adjMatrix[i][j] = tempMatrix[i][j];
    }

    // 3. 지도 그림 위치에 맞춘 국가별 X, Y 픽셀 좌표 매핑 값
    int tempX[12] = { 422,  135,  443,  395,  419,  403,  340,  263,  485,  260,  272,  440 };
    int tempY[12] = { 113,  110,  108,  116,  133,  146,   68,   72,  242,   93,  101,  215 };
    for (int i = 0; i < 12; ++i) {
        this->nodeX[i] = tempX[i];
        this->nodeY[i] = tempY[i];
    }

    // 4. 창 테마 스타일 세팅
    setWindowTitle("Graph Network Air Reservation System");
    resize(560, 840);
    setStyleSheet("background-color: #121824; color: #E2E8F0; font-family: 'Segoe UI', Malgun Gothic, sans-serif;");

    auto* central = new QWidget(this);
    setCentralWidget(central);
    auto* mainLayout = new QVBoxLayout(central); // 컴포넌트들을 위에서 아래로 쌓는 수직 레이아웃 생성
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(12);

    // 위에서 정의한 독립 지도 도화지(MapCanvas) 생성
    auto* mapContainer = new MapCanvas(this, adjMatrix, nodeX, nodeY, startNodeIdx, endNodeIdx);

    // 지도 컴포넌트 위에 숫자 동그라미 버튼 12개를 생성하여 자식으로 결합
    for (int i = 0; i < 12; ++i) {
        QPushButton* nodeBtn = new QPushButton(QString::number(i + 1), mapContainer);
        nodeBtn->setGeometry(nodeX[i] - 12, nodeY[i] - 12, 24, 24); // 버튼을 해당 국가 좌표 중앙에 배치
        nodeButtons.append(nodeBtn);

        // 국가 버튼을 클릭했을 때 작동하는 슬롯 메커니즘
        connect(nodeBtn, &QPushButton::clicked, this, [this, mapContainer, i]() {
            // 아직 출발지가 없거나, 이미 둘 다 선택되어 있는 상태에서 새로 누르면 -> 출발지로 지정
            if (startNodeIdx == -1 || (startNodeIdx != -1 && endNodeIdx != -1)) {
                startNodeIdx = i;
                endNodeIdx = -1;
                outboundListWidget->clear();
                inboundListWidget->clear();
                statusLabel->setText(QString("🛫 출발지: %1 (Node %2) 지정됨 -> [도착지 노드]를 클릭하세요.").arg(countries[i]).arg(i + 1));
            }
            // 이미 선택한 출발지 버튼을 다시 누르면 -> 선택 취소
            else if (startNodeIdx == i) {
                startNodeIdx = -1;
                statusLabel->setText("✈️ 지도의 첫 번째 노드(출발지)를 선택하세요.");
            }
            // 출발지가 있는 상태에서 다른 버튼을 누르면 -> 도착지로 지정
            else {
                endNodeIdx = i;
                QString fromCountry = countries[startNodeIdx];
                QString toCountry = countries[endNodeIdx];
                statusLabel->setText(QString("✅ 구간 지정 완료: %1 ⇄ %2").arg(fromCountry).arg(toCountry));

                outboundListWidget->clear();
                inboundListWidget->clear();

                // 인접 행렬 확인 결과 직항 경로(1)가 연결되어 있는 경우에만 스케줄 자동 생성
                if (adjMatrix[startNodeIdx][endNodeIdx] == 1) {
                    QVector<MapProjectFlightDetail> outFlights = generateFlightSchedule(fromCountry, toCountry, false);
                    for (const auto& f : outFlights) {
                        QString text = QString("▶ [%1] %2 | 시간 %3 | %4원").arg(f.airline).arg(f.flightNum).arg(f.time).arg(f.price);
                        outboundListWidget->addItem(text);
                    }

                    QVector<MapProjectFlightDetail> inFlights = generateFlightSchedule(toCountry, fromCountry, true);
                    for (const auto& f : inFlights) {
                        QString text = QString("◀ [%1] %2 | 시간 %3 | %4원").arg(f.airline).arg(f.flightNum).arg(f.time).arg(f.price);
                        inboundListWidget->addItem(text);
                    }
                }
                else {
                    // 직항 간선이 없을 때 메시지 출력
                    outboundListWidget->addItem("❌ 두 국가 간 연결된 직항 출발편 그래프 간선이 없습니다.");
                    inboundListWidget->addItem("❌ 두 국가 간 연결된 직항 복편 그래프 간선이 없습니다.");
                }
            }
            updateNodeStyles();   // 상호작용 후 버튼 색상 갱신
            mapContainer->update(); // 도화지 화면 강제 새로고침 (선 다시 그리기)
            });
    }

    updateNodeStyles();
    mainLayout->addWidget(mapContainer);

    // 안내 상태 메시지 표시창 배치
    statusLabel = new QLabel("✈️ 지도의 첫 번째 노드(출발지)를 선택하세요.", this);
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("font-size: 13px; font-weight: bold; color: #38BDF8; padding: 2px;");
    mainLayout->addWidget(statusLabel);

    // UI 스타일 지정 (다크 테마 스킨 디자인)
    QString listStyle =
        "QListWidget {"
        "  background-color: #1E293B; border: 1px solid #334155; border-radius: 8px; color: #F1F5F9; font-size: 12px; padding: 4px;"
        "}"
        "QListWidget::item { padding: 8px; border-bottom: 1px solid #1E293B; color: #E2E8F0; }"
        "QListWidget::item:selected { background-color: #0369A1; color: #FFFFFF; font-weight: bold; border-radius: 4px; }";

    // 가는 편 파트 레이아웃 구성
    auto* outLabel = new QLabel("🛫 가는 편 노선 정보", this);
    outLabel->setStyleSheet("font-size: 11px; color: #94A3B8; font-weight: bold;");
    mainLayout->addWidget(outLabel);

    outboundListWidget = new QListWidget(this);
    outboundListWidget->setStyleSheet(listStyle);
    mainLayout->addWidget(outboundListWidget);

    // 오는 편 파트 레이아웃 구성
    auto* inLabel = new QLabel("🛬 오는 편 노선 정보", this);
    inLabel->setStyleSheet("font-size: 11px; color: #94A3B8; font-weight: bold;");
    mainLayout->addWidget(inLabel);

    inboundListWidget = new QListWidget(this);
    inboundListWidget->setStyleSheet(listStyle);
    mainLayout->addWidget(inboundListWidget);

    // [중요] 가는 편 리스트: 이미 고른 항공권을 한 번 더 누르면 선택을 지우고 마우스 초점도 완전 무효화하는 토글 이벤트 기능
    connect(outboundListWidget, &QListWidget::itemClicked, this, [this](QListWidgetItem* item) {
        static QListWidgetItem* lastOutClicked = nullptr;
        if (lastOutClicked == item) {
            outboundListWidget->clearSelection();
            item->setSelected(false);
            outboundListWidget->setCurrentItem(nullptr); // 잔상 포커스까지 깔끔하게 제거
            lastOutClicked = nullptr;
        }
        else {
            lastOutClicked = item;
        }
        });

    // [중요] 오는 편 리스트: 이미 고른 항공권을 한 번 더 누르면 선택을 지우고 마우스 초점도 완전 무효화하는 토글 이벤트 기능
    connect(inboundListWidget, &QListWidget::itemClicked, this, [this](QListWidgetItem* item) {
        static QListWidgetItem* lastInClicked = nullptr;
        if (lastInClicked == item) {
            inboundListWidget->clearSelection();
            item->setSelected(false);
            inboundListWidget->setCurrentItem(nullptr); // 잔상 포커스까지 깔끔하게 제거
            lastInClicked = nullptr;
        }
        else {
            lastInClicked = item;
        }
        });

    // 예매하기 버튼 구성
    bookButton = new QPushButton("선택 노선 항공권 예매 확정", this);
    bookButton->setStyleSheet(
        "QPushButton {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #0EA5E9, stop:1 #2563EB);"
        "  color: white; font-weight: bold; font-size: 14px; padding: 12px; border-radius: 8px; border: none;"
        "}"
        "QPushButton:hover { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #38BDF8, stop:1 #3B82F6); }"
    );
    mainLayout->addWidget(bookButton);

    // 예매 버튼을 클릭했을 때 최종 검증 및 영수증 창을 띄워주는 이벤트 처리 기능
    connect(bookButton, &QPushButton::clicked, this, [this]() {
        // 초점이 잡혀있더라도 사용자가 취소(isSelected가 거부)했으면 가짜 선택으로 판별해 제외함
        QListWidgetItem* outSel = (outboundListWidget->currentItem() && outboundListWidget->currentItem()->isSelected()) ? outboundListWidget->currentItem() : nullptr;
        QListWidgetItem* inSel = (inboundListWidget->currentItem() && inboundListWidget->currentItem()->isSelected()) ? inboundListWidget->currentItem() : nullptr;

        // 두 리스트 모두 선택한 항목이 전혀 없다면 에러 팝업창 띄우기
        if (!outSel && !inSel) {
            QMessageBox::warning(this, "Notification", "예매할 항공권을 목록에서 최소 하나 이상 선택해주세요!");
            return;
        }

        QString outText = outSel ? outSel->text() : "";
        QString inText = inSel ? inSel->text() : "";

        // 직항 노선이 없다는 안내 문구를 클릭하고 예매를 시도하는 상황 차단
        if (outText.contains("없습니다") || inText.contains("없습니다")) return;

        // 문자열에서 오직 숫자(금액) 정보만 쏙 추출해내는 로직
        auto parsePrice = [](const QString& text) {
            if (text.isEmpty()) return 0;
            QString res = "";
            QStringList tokens = text.split(" ");
            if (!tokens.isEmpty()) {
                QString lastToken = tokens.last();
                for (char c : lastToken.toStdString()) {
                    if (c >= '0' && c <= '9') res += c; // 0~9 사이의 글자만 이어 붙여 숫자로 변경
                }
            }
            return res.toInt();
            };

        int outPrice = parsePrice(outText);
        int inPrice = parsePrice(inText);
        int totalPrice = outPrice + inPrice; // 최종 최종 합산 금액 계산

        // 최종 예약 내역 영수증 메시지 박스 구현
        QMessageBox msg(this);
        msg.setWindowTitle("Reservation Complete");

        QString summary = "✨ [항공권 예매가 완료되었습니다!]\n\n"
            "──────────────────────\n";

        if (!outText.isEmpty()) {
            summary += "🛫 가는 편:\n " + outText.trimmed() + "\n\n";
        }
        if (!inText.isEmpty()) {
            summary += "🛬 오는 편:\n " + inText.trimmed() + "\n";
        }

        summary += "──────────────────────\n"
            "💵 총 결제 금액: " + QString::number(totalPrice) + " 원\n\n"
            "이용해 주셔서 감사합니다. 즐거운 여행 되세요!";

        msg.setText(summary);
        msg.setStyleSheet("background-color: #1E293B; color: #F1F5F9; font-size:12px; QPushButton { background-color: #0EA5E9; color: white; padding: 5px 15px; border-radius:4px; }");
        msg.exec(); // 알림 팝업 띄우기
        });
}

// 사용자가 누른 국가 버튼의 종류(출발지/도착지/일반)에 따라 유동적으로 버튼 스킨을 바꾸는 함수
void MainWindow::updateNodeStyles() {
    for (int i = 0; i < 12; ++i) {
        if (i == startNodeIdx) {
            // 출발지로 선택된 국가 버튼 스타일 (초록색 배경에 흰색 테두리)
            nodeButtons[i]->setStyleSheet(
                "QPushButton { background-color: #10B981; color: white; font-weight: bold; font-size: 12px; border-radius: 12px; border: 2px solid #FFFFFF; }"
            );
        }
        else if (i == endNodeIdx) {
            // 도착지로 선택된 국가 버튼 스타일 (황색 배경에 흰색 테두리)
            nodeButtons[i]->setStyleSheet(
                "QPushButton { background-color: #F59E0B; color: white; font-weight: bold; font-size: 12px; border-radius: 12px; border: 2px solid #FFFFFF; }"
            );
        }
        else {
            // 아직 선택받지 못한 일반 국가 버튼 스타일 (짙은 남색 배경에 네온 하늘색 테두리)
            nodeButtons[i]->setStyleSheet(
                "QPushButton {"
                "  background-color: #312E81; color: #00F2FE; font-weight: bold; font-size: 11px;"
                "  border-radius: 12px; border: 2px solid #00F2FE;"
                "}"
                "QPushButton:hover { background-color: #00F2FE; color: #0F172A; }" // 마우스 갖다 대면 색 전환
            );
        }
    }
}

void MainWindow::paintEvent(QPaintEvent* event) {
    QMainWindow::paintEvent(event); // 기본 창 기능의 그리기 이벤트 유지
}