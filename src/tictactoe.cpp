#include "tictactoe.h"
#include "gamescene.h"
#include "utils.h"
#include "fontmanager.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>
const int TicTacToe::COLUMNS = 3;
const int TicTacToe::ROWS    = 3;

TicTacToe::TicTacToe()
{
    reset();
    m_font = FontManager::Instance()->getFont(FontManager::FontID::GUI);
    m_font.setPixelSize(SETTINGS::CELL_SIZE.height()/3);
    m_bgPixmap.load(":/res/bg.png");
    m_oPixmap.load(":/res/o.png");
    m_xPixmap.load(":/res/x.png");
}

void TicTacToe::drawObjects(GameScene &gameScene)
{
    for (int x = 0; x < COLUMNS; ++x)
    {
        for (int y = 0; y < ROWS; ++y)
        {
            if (m_gameArray[x][y] == PLAYER_TYPE::O)
            {
                int posx = x * SETTINGS::CELL_SIZE.width();
                int posy = y * SETTINGS::CELL_SIZE.height();
                QGraphicsPixmapItem* pItem = new QGraphicsPixmapItem();
                pItem->setPos(posx, posy);
                pItem->setPixmap(m_oPixmap);
                gameScene.addItem(pItem);
            }
            else if (m_gameArray[x][y] == PLAYER_TYPE::X)
            {
                int posx = x * SETTINGS::CELL_SIZE.width();
                int posy = y * SETTINGS::CELL_SIZE.height();
                QGraphicsPixmapItem* pItem = new QGraphicsPixmapItem();
                pItem->setPos(posx, posy);
                pItem->setPixmap(m_xPixmap);
                gameScene.addItem(pItem);
            }
        }
    }
}

void TicTacToe::drawWinner(GameScene &gameScene)
{
    if (m_winner != PLAYER_TYPE::NONE)
    {
        QGraphicsLineItem* lItem = new QGraphicsLineItem(QLine(m_linePoints[0], m_linePoints[1]));
        lItem->setPen(QPen(COLOR_STYLE::GREEN3, SETTINGS::CELL_SIZE.width()/4));
        if (m_winner != PLAYER_TYPE::DRAW){
            gameScene.addItem(lItem);
        }

        QGraphicsRectItem * rItem = new QGraphicsRectItem(-SETTINGS::CELL_SIZE.width(),
                                                         -SETTINGS::CELL_SIZE.height()/2,
                                                         2*SETTINGS::CELL_SIZE.width(),
                                                         SETTINGS::CELL_SIZE.height());
        rItem->setPos(SCREEN::PHYSICAL_SIZE.width()/2,
                      SCREEN::PHYSICAL_SIZE.height()/2);
        rItem->setBrush(COLOR_STYLE::GREEN1);
        rItem->setPen(QPen(COLOR_STYLE::GREEN2, 5));

        QGraphicsSimpleTextItem *tItem = new QGraphicsSimpleTextItem(rItem);
        tItem->setFont(m_font);
        QString text;
        if (m_winner == PLAYER_TYPE::O)
        {
            text = "O wins";
        }
        else if (m_winner == PLAYER_TYPE::X)
        {
            text = "X wins";
        }
        else
        {
            text = "Draw";
        }

        tItem->setText(text);
        tItem->setBrush(COLOR_STYLE::GREEN0);
        tItem->setPen(QPen(COLOR_STYLE::GREEN3, 4));
        tItem->setPos(-tItem->boundingRect().width()/2, -tItem->boundingRect().height()/2);
        gameScene.addItem(rItem);
    }
}

void TicTacToe::drawBoard(GameScene &gameScene)
{
    QGraphicsPixmapItem* pItem = new QGraphicsPixmapItem();
    pItem->setPixmap(m_bgPixmap);
    gameScene.addItem(pItem);
}

void TicTacToe::proccess(QPoint clickedPoint)
{
    int x = clickedPoint.x()/SETTINGS::CELL_SIZE.width();
    int y = clickedPoint.y()/SETTINGS::CELL_SIZE.height();
    if (m_gameArray[x][y] == PLAYER_TYPE::NONE && m_winner == PLAYER_TYPE::NONE)
    {
        m_gameArray[x][y] = m_player;
        if(m_player == PLAYER_TYPE::O)
        {
            m_player = PLAYER_TYPE::X;
        }
        else
        {
            m_player = PLAYER_TYPE::O;
        }
        m_gameSteps++;
        checkWinner();
    }
}

void TicTacToe::checkWinner()
{
    for (const auto &lineIndices : m_lineIndicesArray) {
        int sum_line = 0;

        for (const auto &indexPair : lineIndices) {
            int i = indexPair.first;
            int j = indexPair.second;
            sum_line += int(m_gameArray[i][j]);
            if(sum_line == 3 * int(PLAYER_TYPE::O))
            {
                m_winner = PLAYER_TYPE::O;
                m_linePoints[0] = QPoint(lineIndices[0].first*SETTINGS::CELL_SIZE.width()+SETTINGS::CELL_SIZE.width()/2,
                                         lineIndices[0].second*SETTINGS::CELL_SIZE.width()+SETTINGS::CELL_SIZE.width()/2
                                         );
                m_linePoints[1] = QPoint(lineIndices[2].first*SETTINGS::CELL_SIZE.width()+SETTINGS::CELL_SIZE.width()/2,
                                         lineIndices[2].second*SETTINGS::CELL_SIZE.width()+SETTINGS::CELL_SIZE.width()/2
                                         );
                return;
            }
            else if(sum_line == 3 * int(PLAYER_TYPE::X))
            {
                m_winner = PLAYER_TYPE::X;
                m_linePoints[0] = QPoint(lineIndices[0].first*SETTINGS::CELL_SIZE.width()+SETTINGS::CELL_SIZE.width()/2,
                                         lineIndices[0].second*SETTINGS::CELL_SIZE.width()+SETTINGS::CELL_SIZE.width()/2
                                         );
                m_linePoints[1] = QPoint(lineIndices[2].first*SETTINGS::CELL_SIZE.width()+SETTINGS::CELL_SIZE.width()/2,
                                         lineIndices[2].second*SETTINGS::CELL_SIZE.width()+SETTINGS::CELL_SIZE.width()/2
                                         );
                return;
            }
        }
    }

    for(int x = 0; x < COLUMNS; ++x)
    {
        for(int y = 0; y < ROWS; ++y)
        {
            if(m_gameArray[x][y] == PLAYER_TYPE::NONE)
            {
                return;
            }
        }
    }

    m_winner = PLAYER_TYPE::DRAW;
}

void TicTacToe::reset()
{
    for(int x = 0; x < COLUMNS; ++x)
    {
        for(int y = 0; y < ROWS; ++y)
        {
            m_gameArray[x][y] = PLAYER_TYPE::NONE;
        }
    }
    m_winner = PLAYER_TYPE::NONE;
    m_player = PLAYER_TYPE::O;
    m_gameSteps = 0;
    m_linePoints[0] = QPoint(-1, -1);
    m_linePoints[1] = QPoint(-1, -1);
}

QString TicTacToe::currentPlayer() const
{
    if(m_player == PLAYER_TYPE::O)
    {
        return "Player O turn";
    }
    else if(m_player == PLAYER_TYPE::X)
    {
        return "Player X turn";
    }
    return "";
}
