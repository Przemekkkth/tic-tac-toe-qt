#ifndef TICTACTOE_H
#define TICTACTOE_H
#include <QPixmap>
#include <QList>
#include <QPair>
#include <QFont>
class GameScene;
class TicTacToe
{
public:
    enum class PLAYER_TYPE{NONE = 0, O = 10, X = 200, DRAW = 300};
    TicTacToe();

    void drawObjects(GameScene& gameScene);
    void drawWinner(GameScene& gameScene);
    void drawBoard(GameScene& gameScene);
    void proccess(QPoint clickedPoint);
    void checkWinner();
    void reset();
    QString currentPlayer() const;
private:
    QPixmap m_oPixmap, m_xPixmap, m_bgPixmap;
    static const int COLUMNS, ROWS;
    PLAYER_TYPE m_gameArray[3][3];
    PLAYER_TYPE m_winner, m_player;
    int m_gameSteps;
    QPoint m_linePoints[2];
    QList<QList<QPair<int, int>>> m_lineIndicesArray = {
        {{0, 0}, {0, 1}, {0, 2}},
        {{1, 0}, {1, 1}, {1, 2}},
        {{2, 0}, {2, 1}, {2, 2}},
        {{0, 0}, {1, 0}, {2, 0}},
        {{0, 1}, {1, 1}, {2, 1}},
        {{0, 2}, {1, 2}, {2, 2}},
        {{0, 0}, {1, 1}, {2, 2}},
        {{0, 2}, {1, 1}, {2, 0}}
    };
    QFont m_font;
};

#endif // TICTACTOE_H
